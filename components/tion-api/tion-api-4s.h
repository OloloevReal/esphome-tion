#pragma once

#include "tion-api.h"

namespace dentra {
namespace tion {

#pragma pack(push, 1)
struct tion4s_state_t {
  enum HeaterMode : uint8_t {
    HEATER_MODE_HEATING = 0,
    HEATER_MODE_TEMPERATURE_MAINTENANCE = 1,
  };
  // air intake: indoor, outdoor, mixed
  enum GatePosition : uint8_t {
    // приток
    GATE_POSITION_INFLOW = 0,
    // рециркуляция
    GATE_POSITION_RECIRCULATION = 1,
  };

  enum HeaterPresent : uint16_t {
    HEATER_PRESENT_NONE = 0,
    HEATER_PRESENT_1000W = 1,
    HEATER_PRESENT_1400W = 2,
  };

  // Байт 0-1.
  struct {
    // Байт 0, бит 0. Состояние (power state).
    bool power_state : 1;
    // Байт 0, бит 1. Состояние звуковых оповещений.
    bool sound_state : 1;
    // Байт 0, бит 2. Состояние световых оповещений.
    bool led_state : 1;
    // Байт 0, бит 3. Сотояние обогревателя.
    bool heater_state : 1;
    // Байт 0, бит 4. Режим обогрева.
    HeaterMode heater_mode : 1;
    // Байт 0, бит 5.
    uint8_t last_com_source : 1;
    // Байт 0, бит 6. Предупреждение о необходимости замены фильтра.
    bool filter_warnout : 1;
    // Байт 0, бит 7. Мощность тэна: 0 - 0 kW, 1 - 1 kW, 2 - 1.4 kW.
    HeaterPresent heater_present : 3;
    // Байт 1, бит 2. Состояния подключения MagicAir.
    bool ma_connect : 1;
    // Байт 1, бит 3. MagicAir auto control.
    bool ma_auto : 1;
    // Байт 1, бит 4.
    bool active_timer : 1;
    // зарезервированно.
    uint8_t reserved : 3;
  } flags;
  // Байт 2. settings: gate position (0 - inflow, 1 - recirculation).
  GatePosition gate_position;
  // Байт 3. settings: target temperature.
  int8_t target_temperature;
  // Байт 4. settings: fan speed 1-6.
  uint8_t fan_speed;
  // Байт 5. sensor: outdoor temperature.
  int8_t outdoor_temperature;
  // Байт 6.  sensor: current temperature.
  int8_t current_temperature;
  // sensor: ctrl pcb temperature.
  int8_t pcb_ctl_temperature;
  // sensor: pwr pcb temperature.
  int8_t pcb_pwr_temperature;
  // counters.
  tion_state_counters_t<tion_dev_info_t::BR4S> counters;
  // errors.
  uint32_t errors;
  // fan speed limit.
  uint8_t max_fan_speed;
  // Heater power in %.
  // use heater_power() to get actual consumption in W.
  uint8_t heater_var;

  float heater_power() const;
  bool is_initialized() const { return this->counters.work_time != 0; }
  bool filter_warnout() const { return this->flags.filter_warnout; }
};

struct tion4s_turbo_t {
  uint8_t is_active;
  uint16_t turbo_time;  // time in seconds
  uint8_t err_code;
};

#ifdef TION_ENABLE_SCHEDULER
struct tion4s_timers_state_t {
  enum { TIMERS_COUNT = 12u };
  struct {
    bool active : 8;
  } timers[TIMERS_COUNT];
};

/// Timer settings. Used for get and set operations.
struct tion4s_timer_t {
  struct {
    bool monday : 1;
    bool tuesday : 1;
    bool wednesday : 1;
    bool thursday : 1;
    bool friday : 1;
    bool saturday : 1;
    bool sunday : 1;
    bool reserved : 1;
    uint8_t hours;
    uint8_t minutes;
  } schedule;
  struct {
    bool power_state : 1;
    bool sound_state : 1;
    bool led_state : 1;
    tion4s_state_t::HeaterMode heater_mode : 1;
    // on/off timer.
    bool timer_state : 1;
    uint8_t reserved : 3;
  };
  int8_t target_temperature;
  uint8_t fan_state;
  uint8_t device_mode;
};
#endif
#pragma pack(pop)

class TionApi4s : public TionApiBase<tion4s_state_t> {
  /// Callback listener for response to request_turbo command request.
  using on_turbo_type = etl::delegate<void(const tion4s_turbo_t &turbo, const uint32_t request_id)>;
#ifdef TION_ENABLE_SCHEDULER
  /// Callback listener for response to request_time command request.
  using on_time_type = etl::delegate<void(const time_t time, uint32_t request_id)>;
  /// Callback listener for response to request_timer command request.
  using on_timer_type =
      etl::delegate<void(const uint8_t timer_id, const tion4s_timer_t &timers_state, uint32_t request_id)>;
  /// Callback listener for response to request_timers_state command request.
  using on_timers_state_type = etl::delegate<void(const tion4s_timers_state_t &timers_state, uint32_t request_id)>;
#endif
 public:
  void read_frame(uint16_t frame_type, const void *frame_data, size_t frame_data_size);

  uint16_t get_state_type() const;
  bool request_dev_info() const;
  bool request_state() const;

  bool write_state(const tion4s_state_t &state, const uint32_t request_id) const;
  bool reset_filter(const tion4s_state_t &state, const uint32_t request_id = 1) const;
  bool factory_reset(const tion4s_state_t &state, const uint32_t request_id = 1) const;

#ifdef TION_ENABLE_PRESETS
  bool request_turbo() const;

  /// Callback listener for response to request_turbo command request.
  on_turbo_type on_turbo{};
  bool set_turbo(const uint16_t time, const uint32_t request_id = 1) const;
#endif

#ifdef TION_ENABLE_HEARTBEAT
  bool send_heartbeat() const;
#endif

#ifdef TION_ENABLE_SCHEDULER
  bool request_time(const uint32_t request_id = 1) const;

  /// Callback listener for response to request_time command request.
  on_time_type on_time{};
  bool set_time(const time_t time, const uint32_t request_id) const;

  /// Callback listener for response to request_timer command request.
  on_timer_type on_timer{};
  bool request_timer(const uint8_t timer_id, const uint32_t request_id = 1) const;

  /// Request all timers.
  bool request_timers(const uint32_t request_id = 1) const;

  bool write_timer(const uint8_t timer_id, const tion4s_timer_t &timer, const uint32_t request_id = 1) const;

  bool request_timers_state(const uint32_t request_id = 1) const;
  /// Callback listener for response to request_timers_state command request.
  on_timers_state_type on_timers_state{};

#endif
#ifdef TION_ENABLE_DIAGNOSTIC
  bool request_errors() const;
  bool request_test() const;
#endif
};

}  // namespace tion
}  // namespace dentra
