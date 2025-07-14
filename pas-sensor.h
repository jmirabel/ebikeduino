#include "HardwareSerial.h"
struct PasSensor {
  static constexpr unsigned int pas_n_magnet = 12;
  static constexpr unsigned int min_pedaling_rpm = 20; // 3 sec per rotation
  static constexpr unsigned int pas_detection_time_ms = 60000 / (min_pedaling_rpm * pas_n_magnet);

  static PasSensor* _instanceForInterrupt;

  bool rising = false;
  bool pedaling = false;
  bool changed = false;

  volatile unsigned long lastPASRisingEdge = 0;

  void setup(uint8_t pin) {
    pinMode(pin, INPUT_PULLUP);
    _instanceForInterrupt = this;
    attachInterrupt(digitalPinToInterrupt(pin), PasSensor::interruptPedalSensor, RISING);
  }
  
  void handleRisingPAS() {
    lastPASRisingEdge = millis();
    rising = true;
  }

  static void interruptPedalSensor() {
    if (_instanceForInterrupt != NULL) {
      _instanceForInterrupt->handleRisingPAS();
    }
  }

  void loop() {
    if (rising) {
      // Serial.println(lastPASRisingEdge);
      rising = false;
    }
    bool newPedaling = ((millis() - lastPASRisingEdge) < pas_detection_time_ms);
    changed = (newPedaling != pedaling);
    pedaling = newPedaling;
  }

};

PasSensor* PasSensor::_instanceForInterrupt = NULL;