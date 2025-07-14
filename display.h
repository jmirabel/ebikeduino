#pragma once

#include <rgb_lcd.h>

template<class Derived>
class DisplayBase {
public:
  void set_pedaling_state(bool pedaling) {
    derived().set_pedaling_state(pedaling);
  }
  void set_assistance_level(unsigned short level) {
    derived().set_assistance_level(level);
  }
  void set_brake_state(bool braking) {
    derived().set_brake_state(braking);
  }

  void set_contol(int control) {
    derived().set_control(control);
  }

  Derived& derived() {
    return static_cast<Derived&>(*this);
  }
};

class I2C16x2Display : public DisplayBase<I2C16x2Display> {
private:
  rgb_lcd lcd;

public:
  void setup() {
    lcd.begin(16, 2);
    lcd.clear();
  }

  void loop() {}

  void set_brake_state(bool braking) {
    lcd.setCursor(0, 0);
    if (braking)
      lcd.print('B');
    else
      lcd.print('_');
  }


  void set_pedaling_state(bool pedaling) {
    lcd.setCursor(1, 0);
    if (pedaling)
      lcd.print('P');
    else
      lcd.print('_');
  }

  void set_assistance_level(unsigned short level) {
    lcd.setCursor(0, 1);
    lcd.print(level);
  }

  void set_contol(int control) {
    lcd.setCursor(2, 1);
    lcd.print(control);
  }
};


class UARTDisplay : public DisplayBase<I2C16x2Display> {
private:
  char buffer[20] = "___\0";
  unsigned long nextPrint = 0;

public:
  static unsigned long printPeriodMs;
  
  void setup() {}

  void loop() {
    if (Serial) {
      if (millis() > nextPrint) {
        nextPrint = millis() + printPeriodMs;
        Serial.println(buffer);
      }
    }
  }

  void set_brake_state(bool braking) {
    buffer[0] = braking ? 'B' : '_';
  }


  void set_pedaling_state(bool pedaling) {
    buffer[1] = pedaling ? 'B' : '_';
  }

  void set_assistance_level(unsigned short level) {
    switch (level) {
      case 0: buffer[2] = '0'; break;
      case 1: buffer[2] = '1'; break;
      case 2: buffer[2] = '2'; break;
      case 3: buffer[2] = '3'; break;
      case 4: buffer[2] = '4'; break;
      default: buffer[2] = '?'; break;
    }
    return;
  }

  void set_contol(int control) {
    sprintf(&buffer[3], "%d", control);
  }
};

unsigned long UARTDisplay::printPeriodMs = 512;