#include "cinttypes"
#include "esphome/core/log.h"

#include "tion_3s_vport.h"

namespace esphome {
namespace tion {

static const char *const TAG = "tion_3s_vport";

#define RTC_PAIR_STATE fnv1_hash("tion_3s")
#define RTC_MAC_ADDRESS fnv1_hash("tion_3s_mac_address")

bool Tion3sBleIO::ble_reg_for_notify() const {
  if (this->vport_ == nullptr) {
    ESP_LOGE(TAG, "vport is not set");
    return true;
  }

  // при постоянном подключении требуется переподключение, иначе мы никогде не получим notify
  // поэтому всегда подписываемся
  if (this->vport_->is_persistent_connection()) {
    return true;
  }

  return this->vport_->is_paired();
}

void Tion3sBleVPort::setup() {
  if (this->api_ == nullptr) {
    ESP_LOGE(TAG, "api is not configured");
    this->mark_failed();
    return;
  }

  this->rtc_ = global_preferences->make_preference<int8_t>(RTC_PAIR_STATE, true);
  int8_t loaded{};
  if (this->rtc_.load(&loaded)) {
    this->pair_state_ = loaded;
  }
#ifdef TION_ENABLE_MAC_CHAHGE
  uint64_t address{};
  auto pref = global_preferences->make_preference<uint64_t>(RTC_MAC_ADDRESS, true);
  if (pref.load(&address)) {
    this->io_->set_address(address);
    ESP_LOGI(TAG, "MAC address loaded from flash: %12" PRIX64, address);
  }
#endif
}
#ifdef TION_ENABLE_MAC_CHAHGE
void Tion3sBleVPort::save_mac_address(const std::string &mac_address) {
  ESP_LOGD(TAG, "Saving MAC to flash: %s", mac_address.c_str());
  std::string copy = mac_address;
  copy.erase(std::remove(copy.begin(), copy.end(), ':'), copy.end());
  uint16_t address = std::strtoul(copy.c_str(), nullptr, 16);
  if (address > 0) {
    this->io_->set_address(address);
    global_preferences->make_preference<uint64_t>(RTC_MAC_ADDRESS, true).save(&address);
    ESP_LOGI(TAG, "MAC address saved to flash: %12" PRIX64, address);
  } else {
    ESP_LOGW(TAG, "Invalid MAC: %s", mac_address.c_str());
  }
}
#endif

void Tion3sBleVPort::dump_config() {
  TION_VPORT_BLE_LOG("Tion 3S BLE");
  ESP_LOGCONFIG(TAG, "  Always Pair (experimental): %s", ONOFF(this->experimental_always_pair_));
  ESP_LOGCONFIG(TAG, "  Paired: %s", ONOFF(this->is_paired()));
}

void Tion3sBleVPort::write(const Tion3sBleIO::frame_spec_type &frame, size_t size) {
  TionVPortBLEComponent::write(frame, size);
}

// FIXME implement or remove
// void Tion3sBleVPort::update() {
// if (!this->io_->is_connected() || this->is_paired()) {
//   TionBLEVPortBase::update();
// } else {
//   ESP_LOGW(TAG, "Pairing required. [pair_state: %d, is_connected: %s", this->pair_state_,
//            YESNO(this->io_->is_connected()));
//   this->io_->disconnect();
// }
//}

void Tion3sBleVPort::pair() {
  this->pair_state_ = -1;
  if (this->io_->is_connected()) {
    this->pair_();
  } else {
    this->io_->connect();
  }
}

void Tion3sBleVPort::reset_pair() {
  this->pair_state_ = 0;
  this->rtc_.save(&this->pair_state_);
  this->io_->disconnect();
}

void Tion3sBleVPort::on_ready_3s_() {
  this->pair_();
  if (this->experimental_always_pair_) {
    this->api_->pair();
  }
  this->on_ready_();
}

void Tion3sBleVPort::pair_() {
  // pairing in progress
  if (this->pair_state_ < 0) {
    bool res = this->api_->pair();
    if (res) {
      this->pair_state_ = 1;
      this->rtc_.save(&this->pair_state_);
    }
    ESP_LOGD(TAG, "Pairing complete: %s", YESNO(res));
  } else {
    ESP_LOGW(TAG, "Pairing was not started");
  }
}

}  // namespace tion
}  // namespace esphome
