#pragma once

#include "tion_switch.h"

namespace esphome {
namespace tion {

class TionBoostSwitch : public TionSwitch<property_controller::switch_::Boost> {
 public:
  explicit TionBoostSwitch(TionApiComponent *api) : TionSwitch(api) {}

  void set_boost_time(uint8_t boost_time) { property_controller::number::BoostTime::set(this->parent_, boost_time); }
  void set_boost_heater_state(bool heater_state) { this->parent_->api()->set_boost_heater_state(heater_state); }
  void set_boost_target_temperture(int8_t target_temperture) {
    this->parent_->api()->set_boost_target_temperture(target_temperture);
  }
};

}  // namespace tion
}  // namespace esphome
