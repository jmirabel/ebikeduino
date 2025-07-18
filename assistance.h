#pragma once
#include "config.h"
#include "Arduino.h"
#include "throttle.h"

constexpr unsigned short MAX_LEVEL = 4;
const int LEVEL_MAP[MAX_LEVEL+1] = {0, 1, 2, 3, 4};

struct Assistance {
  
  unsigned short level = 0;
  bool changed = false;
  unsigned long footModeDeactivationTime = 0;
  bool footMode = false;

  Config const* config;

  Assistance(Config const* cfg)
    : config(cfg) {}

  void loop(unsigned long time, Throttle const& throttle) {
    changed = false;

    if (time > footModeDeactivationTime) {
      disableFootMode();
    }
    if (footMode && !throttle.isLowest()) {
      enableFootMode(time);
    }
  }

  void increment() {
    if (level == MAX_LEVEL) return;

    ++level;
    changed = true;
  }

  void decrement() {
    if (level == 0) return;

    --level;
    changed = true;
  }

  void stop() {
    disableFootMode();
    level = 0;
    changed = true;
  }

  void enableFootMode(unsigned long time) {
    footModeDeactivationTime = time + (unsigned long)(config->foot_mode_timeout_sec) * 1000;
    footMode = true;
    changed = true;
  }

  void disableFootMode() {
    footModeDeactivationTime = millis();
    footMode = false;
    changed = true;
  }

  int map(int toLow, int toHigh) {
    return ::map(LEVEL_MAP[level], LEVEL_MAP[0], LEVEL_MAP[MAX_LEVEL], toLow, toHigh);
  }
};