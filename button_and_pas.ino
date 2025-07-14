#include <Arduino.h>
#include <rgb_lcd.h>
#include "button.h"
#include "assistance.h"
#include "throttle.h"
#include "ppm-control.h"
#include "pas-sensor.h"
#include "display.h"
#include "serial-commands.h"


rgb_lcd lcd;
Buttons buttons(4, 5);  // red then green
Assistance assistance;
Throttle throttle(A1);  // TODO
constexpr uint8_t PAS_SENSOR_PIN = 2;
PasSensor pasSensor;
constexpr uint8_t PPM_PIN = 6;
PpmControl control;

UARTDisplay display;
// I2C16x2Display display;

Button brakeSensor(7);

void setup() {
  serialCommands.readSerial();

  buttons.setup();
  brakeSensor.setup();
  control.setup(PPM_PIN);
  display.setup();

  pasSensor.setup(PAS_SENSOR_PIN);

  Serial.begin(9600);

  display.set_assistance_level(assistance.level);
  display.set_brake_state(false);
  display.set_pedaling_state(false);
  display.set_contol(control.low);
}

void loop() {
  buttons.read();
  brakeSensor.read(millis());
  assistance.loop();
  pasSensor.loop();
  throttle.read();

  if (buttons.red.clicked) {
    assistance.increment();
  }

  if (buttons.green.clicked) {
    assistance.decrement();
  }
  bool braking = (brakeSensor.state == Button::STATE_DOWN);
  display.set_brake_state(braking);

  if (assistance.changed)
    display.set_assistance_level(assistance.level);
  if (pasSensor.changed)
    display.set_pedaling_state(pasSensor.pedaling);

  if (!braking && pasSensor.pedaling) {
    int v0 = assistance.map(control.low, control.high);
    int v1 = throttle.map(control.low, control.high);
    control.write(max(v0, v1));
    display.set_contol(max(v0, v1));
  } else {
    control.stop();
    display.set_contol(control.low);
  }

  display.loop();
  serialCommands.readSerial();

  delay(10);
}
