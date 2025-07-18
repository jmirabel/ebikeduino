#include "Arduino.h"
#include "HardwareSerial.h"
#include "config.h"

struct PasSensor {
  // static constexpr unsigned int pas_n_magnet = 12;
  // static constexpr unsigned int min_pedaling_rpm = 20; // 3 sec per rotation
  // static constexpr unsigned int pas_detection_time_ms = 60000 / (min_pedaling_rpm * pas_n_magnet);

  static PasSensor* _instanceForInterrupt;
  volatile unsigned long lastPASRisingEdge = 0;
  volatile unsigned long period = 0;
  volatile uint8_t risingEdgeCount = 0;

  bool pedaling = false;
  bool changed = false;

  Config const* config;

  PasSensor(Config const* cfg)
    : config(cfg) {}

  void setup(uint8_t pin) {
    pinMode(pin, INPUT);
    _instanceForInterrupt = this;
    attachInterrupt(digitalPinToInterrupt(pin), PasSensor::interruptPedalSensor, RISING);
  }

  void handleRisingPAS() {
    unsigned long time = millis();
    period = time - lastPASRisingEdge;
    lastPASRisingEdge = time;
    if (risingEdgeCount < 32)
        risingEdgeCount++;
  }

  static void interruptPedalSensor() {
    // if (_instanceForInterrupt != NULL) {
    _instanceForInterrupt->handleRisingPAS();
    // }
  }

  void loop() {
    // Default value is true. When pas_detection_time_ms is 0, the PAS sensor is disabled and `pedaling`
    // should be set to true.
    bool newPedaling = true;
    if (config->pas_detection_time_ms > 0) {
      bool within_timeout = ((millis() - lastPASRisingEdge) < config->pas_detection_time_ms);
      if (!within_timeout) {
        risingEdgeCount = 0;
        newPedaling = false;
      } else {
        newPedaling = (risingEdgeCount >= config->pas_rising_edge_count);
      }
    }
    changed = (newPedaling != pedaling);
    pedaling = newPedaling;
  }
};

PasSensor* PasSensor::_instanceForInterrupt = NULL;