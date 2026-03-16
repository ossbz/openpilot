#!/usr/bin/env python3
import sys
import pathlib
import codecs
import pickle
from typing import Any

from tinygrad.nn.onnx import OnnxPBParser


class MetadataOnnxPBParser(OnnxPBParser):
  def _parse_ModelProto(self) -> dict:
    obj: dict[str, Any] = {"graph": {"input": [], "output": []}, "metadata_props": []}
    for fid, wire_type in self._parse_message(self.reader.len):
      match fid:
        case 7:
          obj["graph"] = self._parse_GraphProto()
        case 14:
          obj["metadata_props"].append(self._parse_StringStringEntryProto())
        case _:
          self.reader.skip_field(wire_type)
    return obj


def get_name_and_shape(value_info: dict[str, Any] | tuple[str, Any]) -> tuple[str, tuple[int, ...]]:
  if isinstance(value_info, tuple):
    name, parsed_type = value_info
  else:
    name, parsed_type = value_info["name"], value_info["parsed_type"]
  shape = tuple(int(dim) if isinstance(dim, int) else 0 for dim in parsed_type.shape)
  return name, shape


def get_metadata_value_by_name(model: dict[str, Any], name: str) -> str | Any:
  for prop in model["metadata_props"]:
    if isinstance(prop, tuple):
      key, value = prop
    else:
      key, value = prop["key"], prop["value"]
    if key == name:
      return value
  return None


if __name__ == "__main__":
  model_path = pathlib.Path(sys.argv[1])
  model = MetadataOnnxPBParser(model_path).parse()
  output_slices = get_metadata_value_by_name(model, 'output_slices')
  assert output_slices is not None, 'output_slices not found in metadata'

  metadata = {
    'model_checkpoint': get_metadata_value_by_name(model, 'model_checkpoint'),
    'output_slices': pickle.loads(codecs.decode(output_slices.encode(), "base64")),
    'input_shapes': dict(get_name_and_shape(x) for x in model["graph"]["input"]),
    'output_shapes': dict(get_name_and_shape(x) for x in model["graph"]["output"]),
  }

  metadata_path = model_path.parent / (model_path.stem + '_metadata.pkl')
  with open(metadata_path, 'wb') as f:
    pickle.dump(metadata, f)

  print(f'saved metadata to {metadata_path}')
