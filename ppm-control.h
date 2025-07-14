#include <Servo.h>

struct PpmControl {
  static constexpr int PpmMinPulseWidth = 1000; // us
  static constexpr int PpmMaxPulseWidth = 2000; // us

  static constexpr int low = PpmMinPulseWidth; // us
  static constexpr int high = PpmMaxPulseWidth; // us

  Servo servo;
  int current;

  void setup(uint8_t pin) {
    servo.attach(pin);
    stop();
  }

  void stop() {
    servo.writeMicroseconds(PpmMinPulseWidth);
    current = PpmMinPulseWidth;
  }

  void write(int pulse_us) {
    servo.writeMicroseconds(pulse_us);
    current = pulse_us;
  }
};