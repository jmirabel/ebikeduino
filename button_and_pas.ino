#include <Arduino.h>
#include <avr/power.h>
#include <avr/sleep.h>
#include "config.h"
#include "button.h"
#include "assistance.h"
#include "throttle.h"
#include "ppm-control.h"
#include "pas-sensor.h"
#include "display.h"
#include "serial-commands.h"

Buttons buttons(4, 5);  // red then green
Assistance assistance(&config);
Throttle throttle(A1, &config);
constexpr uint8_t PAS_SENSOR_PIN = 2;
PasSensor pasSensor(&config);
constexpr uint8_t PPM_PIN = 6;
PpmControl control;

UARTDisplay display;
// Enable I2C display. Note that I2C is disabled in the `setup` function to save power.
// I2C16x2Display display;

Button brakeSensor(7);

// More energy efficient sleep.
void sleep_for(uint16_t ms) {
  for (uint16_t i = 0; i < ms; i += 1) {
    set_sleep_mode(SLEEP_MODE_IDLE);
    sleep_enable();
    delayMicroseconds(1000);  // Approx 1 ms
    sleep_cpu();              // Sleep during this time
    sleep_disable();
  }
}

void setup() {
  // Disable SPI
  PRR |= (1 << PRSPI);

  // Disable TWI (I2C)
  PRR |= (1 << PRTWI);

  // Disable Analog Comparator
  ACSR |= (1 << ACD);

  config.eepromRead();

  buttons.setup();
  brakeSensor.setup();
  control.setup(PPM_PIN);
  display.setup();

  pasSensor.setup(PAS_SENSOR_PIN);

  display.set_assistance_level(assistance.level);
  display.set_brake_state(false);
  display.set_pedaling_state(false);
  display.set_contol(control.low);
}

void loop() {
  auto now = millis();
  buttons.read();
  brakeSensor.read(now);
  pasSensor.loop();
  throttle.read();
  assistance.loop(now, throttle);

  if (buttons.red.clicked) {
    assistance.increment();
  }

  if (buttons.green.clicked) {
    assistance.decrement();
  }

  if (buttons.red.longClicked && buttons.green.longClicked) {
    // Enable debug mode using UART over USB.
    Serial.begin(9600);
  }

  if (buttons.red.longClicked) {
    // Throttle can now be used without pedaling
    assistance.enableFootMode(now);
  }

  if (buttons.green.longClicked) {
    // Throttle cannot be used without pedaling. Not that a timer also deactivate this.
    // Also set the assistance level to 0;
    assistance.stop();
  }

  bool braking = (brakeSensor.state == Button::STATE_DOWN);
  display.set_brake_state(braking);

  if (assistance.changed) {
    display.set_assistance_level(assistance.level);
    display.set_foot_mode(assistance.footMode);
  }
  if (pasSensor.changed)
    display.set_pedaling_state(pasSensor.pedaling);

  display.set_throttle_value(throttle.value);
  if (!braking && (assistance.footMode || pasSensor.pedaling)) {
    int v0;
    if (assistance.footMode && !pasSensor.pedaling) {
      v0 = control.low;
    } else {
      v0 = assistance.map(control.low, control.high);
    }
    int v1 = throttle.map(control.low, control.high);
    control.write(max(v0, v1));
    display.set_contol(max(v0, v1));
  } else {
    control.stop();
    display.set_contol(control.low);
  }

  display.loop();
  if (Serial)
    serialCommands.readSerial();

  // delay(10);
  sleep_for(10);
}
