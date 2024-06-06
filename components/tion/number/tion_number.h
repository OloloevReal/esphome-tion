#pragma once

#include "esphome/core/defines.h"
#include "esphome/core/helpers.h"
#include "esphome/core/component.h"
#include "esphome/core/log.h"

#include "esphome/components/number/number.h"

#include "../tion_component.h"
#include "../tion_properties.h"

namespace esphome {
namespace tion {

// C - PropertyController
template<class C> class TionNumber : public number::Number, public Component, public Parented<TionApiComponent> {
  using TionState = dentra::tion::TionState;
  using PC = property_controller::Controller<C>;
  friend class property_controller::Controller<C>;
  constexpr static const auto *TAG = "tion_number";

 public:
  explicit TionNumber(TionApiComponent *api) : Parented(api) {}

  float get_setup_priority() const override { return setup_priority::LATE; }

  void dump_config() override {
    if (this->is_failed()) {
      return;
    }
    LOG_NUMBER("", "Tion Number", this);
  }

  void setup() override {
    ESP_LOGD(TAG, "Setting up %s...", this->get_name().c_str());

    if (std::isnan(this->traits.get_min_value())) {
      this->traits.set_min_value(C::get_min(this->parent_));
    }
    if (std::isnan(this->traits.get_max_value())) {
      this->traits.set_max_value(C::get_max(this->parent_));
    }

    if (this->traits.get_min_value() == this->traits.get_max_value()) {
      PC::mark_unsupported(this);
      return;
    }

    if constexpr (PC::checker().has_api_set()) {
      if (this->restore_value_) {
        this->pref_ = global_preferences->make_preference<float>(this->get_object_id_hash());
        float value;
        if (this->pref_.load(&value)) {
          ESP_LOGI(TAG, "Restored %s: %f", this->get_name().c_str(), value);
          this->control(value);
        }
      }
    }

    this->parent_->add_on_state_callback([this](const TionState *state) {
      if (!PC::publish_state(this, state)) {
        this->has_state_ = false;
      }
    });
  }

  void set_restore_value(bool restore_value) { this->restore_value_ = restore_value; }

  void set_initial_value(float value) {
    if constexpr (PC::checker().has_api_set()) {
      ESP_LOGD(TAG, "Set %s initial value: %f", this->get_name().c_str(), value);
      this->control(value);
    }
  }

 protected:
  bool restore_value_{};
  ESPPreferenceObject pref_;
  void control(float value) override {
    if (!std::isnan(value)) {
      PC::control(this, value);
      if (this->restore_value_) {
        this->pref_.save(&value);
      }
    }
  }
};

}  // namespace tion
}  // namespace esphome
