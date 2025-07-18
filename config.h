#pragma once

#include <EEPROM.h>

struct Config {
  static constexpr int eepromAddress = 0;

  int throttle_min = 188;
  int throttle_max = 510;

  uint8_t pas_detection_time_ms = 250; // Set to 0 to disable PAS (always pedaling)
  uint8_t pas_rising_edge_count = 4;

  uint8_t foot_mode_timeout_sec = 60;

  void eepromUpdate() {
    EEPROM.put(eepromAddress, *this);
  }

  void eepromRead() {
    EEPROM.get(eepromAddress, *this);
  }

  void setDefault() {
    *this = Config();
  }
};

Config config;