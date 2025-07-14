#include "HardwareSerial.h"
struct Throttle {
  // Calibration procedure ?
  // static constexpr int max = 1024;
  // static constexpr int min = 0;
  static constexpr int max = 510;
  static constexpr int min = 188;

  uint8_t pin;
  int value;
  int min_ = max;
  int max_ = min;

  Throttle(uint8_t pin) : pin(pin) {}

  inline void read() {
    value = constrain(analogRead(pin), min, max);
    // if (value < min_) {
    //   min_ = value;
    //   Serial.print(min_);
    //   Serial.print(' ');
    //   Serial.println(max_);
    // }
    // if (value > max_) {
    //   max_ = value;
    //   Serial.print(min_);
    //   Serial.print(' ');
    //   Serial.println(max_);
    // }
  }

  inline int map(int toLow, int toHigh) {
    return ::map(value, min, max, toLow, toHigh);
  }
};