#include "esphome/core/log.h"

#include "tion_3s_proxy.h"

namespace esphome {
namespace tion_3s_proxy {

static const char *const TAG = "tion_3s_proxy";

void TionApi3sProxy::read_frame(uint16_t frame_type, const void *frame_data, size_t frame_data_size) {
  auto data8 = static_cast<const uint8_t *>(frame_data);
  ESP_LOGD(TAG, "RX (%04X): %s", frame_type, format_hex_pretty(data8, frame_data_size).c_str());
  this->tx_->write_frame(frame_type, frame_data, frame_data_size);
}

void Tion3sProxy::on_frame_(const dentra::tion::TionUartProtocol3s::frame_spec_type &frame, size_t size) {
  auto frame_data_size = size - frame.head_size();
  ESP_LOGD(TAG, "TX (%04X): %s", frame.type, format_hex_pretty(frame.data, frame_data_size).c_str());
  this->rx_->write_frame(frame.type, frame.data, frame_data_size);
}

void Tion3sProxy::dump_config() { ESP_LOGCONFIG(TAG, "Tion 3S Proxy"); }

}  // namespace tion_3s_proxy
}  // namespace esphome
