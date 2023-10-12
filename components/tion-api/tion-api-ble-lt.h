#pragma once

#include "tion-api.h"

namespace dentra {
namespace tion {

class TionLtBleProtocol : public TionProtocol<tion_any_ble_frame_t> {
 public:
  bool read_data(const uint8_t *data, size_t size);

  bool write_frame(uint16_t type, const void *data, size_t size);

  const char *get_ble_service() const;
  const char *get_ble_char_tx() const;
  const char *get_ble_char_rx() const;

 protected:
  std::vector<uint8_t> rx_buf_;

  bool write_packet_(const void *data, uint16_t size) const;
  bool read_frame_(const void *data, uint32_t size);
};

}  // namespace tion
}  // namespace dentra
