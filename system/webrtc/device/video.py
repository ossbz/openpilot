import asyncio
import time

import av
from teleoprtc.tracks import TiciVideoStreamTrack

from cereal import messaging
from openpilot.common.realtime import DT_MDL, DT_DMON


class LiveStreamVideoStreamTrack(TiciVideoStreamTrack):
  camera_to_sock_mapping = {
    "driver": "livestreamDriverEncodeData",
    "wideRoad": "livestreamWideRoadEncodeData",
    "road": "livestreamRoadEncodeData",
  }

  def __init__(self, camera_type: str):
    dt = DT_DMON if camera_type == "driver" else DT_MDL
    super().__init__(camera_type, dt)

    sock_name = self.camera_to_sock_mapping[camera_type]
    print(f"[LiveStream] init: camera={camera_type}, subscribing to {sock_name}")
    self._sock = messaging.sub_sock(sock_name, conflate=True)
    self._pts = 0
    self._t0_ns = time.monotonic_ns()

  # def _recv_blocking(self):
  #   while True:
  #     msg = messaging.recv_one(self._sock)
  #     if msg is not None:
  #       return msg

  # async def recv(self):
  #   loop = asyncio.get_running_loop()
  #   msg = await loop.run_in_executor(None, self._recv_blocking)

  async def recv(self):
    poll_count = 0
    t_start = time.monotonic()
    while True:
      msg = messaging.recv_one_or_none(self._sock)
      if msg is not None:
        break
      poll_count += 1
      if poll_count % 200 == 0:  # every ~1s
        print(f"[LiveStream] recv: waiting for frame... ({poll_count} polls, {time.monotonic() - t_start:.1f}s)")
      await asyncio.sleep(0.005)

    evta = getattr(msg, msg.which())
    print(f"[LiveStream] recv: got msg type={msg.which()}, header={len(evta.header)}B, data={len(evta.data)}B")

    try:
      packet = av.Packet(evta.header + evta.data)
      packet.time_base = self._time_base
    except Exception as e:
      print(f"[LiveStream] recv: ERROR creating av.Packet: {e}")
      raise

    self._pts =  ((time.monotonic_ns() - self._t0_ns) * self._clock_rate) // 1_000_000_000
    packet.pts = self._pts
    print(f"[LiveStream] recv: sending frame pts={self._pts}")

    return packet

  def codec_preference(self) -> str | None:
    return "H265"
