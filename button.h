struct Button {
  static constexpr bool STATE_UP = true; // button not pressed
  static constexpr bool STATE_DOWN = false; // button pressed
  static constexpr unsigned long longPressInterval = 1000; // ms
  Button(uint8_t pin)
    : pin(pin) {}

  void setup() {
    pinMode(pin, INPUT_PULLUP);
  }

  void read(unsigned long now) {
    bool nextState = digitalRead(pin);
    if (nextState == state) {
      setSteady();
      return;
    }
    state = nextState;
    if (state) {
      released = true;
      if (now > longPressTime) {
        longClicked = true;
      } else {
        clicked = true;
      }
    }
    else {
      longPressTime = now + longPressInterval;
      pressed = true;
    }
  }

  void setSteady() {
    pressed = false;
    released = false;
    clicked = false;
    longClicked = false;
  }

  const uint8_t pin;
  bool state = true;
  bool pressed = false; // state goes from STATE_UP to STATE_DOWN
  bool released = false; // state goes from STATE_DOWN to STATE_UP
  bool clicked = false; // pressed then released in a short time.
  bool longClicked = false; // pressed then released in a long time.
  unsigned long longPressTime = 0;
};

struct Buttons {
  Button red;
  Button green;

  static const unsigned long readInterval = 50;
  unsigned long nextRead_;

  Buttons(uint8_t redPin, uint8_t greenPin)
    : red(redPin), green(greenPin) {}

  void setup() {
    red.setup();
    green.setup();
  }

  void read() {
    unsigned long now = millis();
    if (now < nextRead_) {
      red.setSteady();
      green.setSteady();
      return;
    }

    nextRead_ = now + readInterval;
    red.read(now);
    green.read(now);
  }
};