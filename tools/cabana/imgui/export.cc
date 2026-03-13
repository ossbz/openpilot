#include "tools/cabana/imgui/export.h"

#include <cstdio>
#include <fstream>

#include "tools/cabana/imgui/dbcmanager.h"
#include "tools/cabana/imgui/stream.h"
#include "tools/cabana/imgui/util.h"

namespace utils {

void exportToCSV(const std::string &file_name, std::optional<MessageId> msg_id) {
  std::ofstream file(file_name);
  if (!file.is_open()) return;

  file << "time,addr,bus,data\n";
  for (auto e : msg_id ? can->events(*msg_id) : can->allEvents()) {
    char time_buf[32], addr_buf[32];
    snprintf(time_buf, sizeof(time_buf), "%.3f", can->toSeconds(e->mono_time));
    snprintf(addr_buf, sizeof(addr_buf), "0x%x", e->address);
    std::string hex_data = "0x" + utils::toHex({e->dat, e->dat + e->size});

    file << time_buf << "," << addr_buf << "," << (int)e->src << "," << hex_data << "\n";
  }
}

void exportSignalsToCSV(const std::string &file_name, const MessageId &msg_id) {
  auto msg = dbc()->msg(msg_id);
  if (!msg || msg->sigs.empty()) return;

  std::ofstream file(file_name);
  if (!file.is_open()) return;

  file << "time,addr,bus";
  for (auto s : msg->sigs)
    file << "," << s->name;
  file << "\n";

  for (auto e : can->events(msg_id)) {
    char time_buf[32], addr_buf[32];
    snprintf(time_buf, sizeof(time_buf), "%.3f", can->toSeconds(e->mono_time));
    snprintf(addr_buf, sizeof(addr_buf), "0x%x", e->address);

    file << time_buf << "," << addr_buf << "," << (int)e->src;
    for (auto s : msg->sigs) {
      double value = 0;
      s->getValue(e->dat, e->size, &value);
      char val_buf[64];
      snprintf(val_buf, sizeof(val_buf), "%.*f", s->precision, value);
      file << "," << val_buf;
    }
    file << "\n";
  }
}

}  // namespace utils
