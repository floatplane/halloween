int RELAY_PINS[]  = {32, 33, 34, 35};
int DELAY = 5000;
int SWITCH_PINS[] = {12, 11, 10, 9};
long ATTRACT_TIME = 30000;

class DebouncedButton {
  public:
  DebouncedButton(long onDelay, long offDelay) {
    this->onDelay = onDelay;
    this->offDelay = offDelay;
    this->onAt = -1;
    this->offAt = -1;
  }

  bool update(bool isPressed) {
    long now = millis();
    if (isPressed) {
      if (this->onAt == -1) {
        this->onAt = now + this->onDelay;
      }
      this->offAt = now + this->offDelay;
    }

    if (now >= this->onAt && now < this->offAt) {
      return true;
    } else {
      if (now >= this->offAt) {
        this->onAt = -1;
      }
      return false;
    }
  }

  long lastOn() {
    return this->offAt == -1 ? -1 : millis() - this->offAt;
  }

  private:
  long onDelay;
  long offDelay;
  long onAt;
  long offAt;
};

DebouncedButton buttons[4] = {
  DebouncedButton(0, 25000),
  DebouncedButton(0, 25000),
  DebouncedButton(0, 25000),
  DebouncedButton(0, 25000)
};

// the setup function runs once when you press reset or power the board
void setup() {
  // Serial.begin(9600); // opens serial port, sets data rate to 9600 bps
  for (int i : RELAY_PINS) {
    pinMode(i, OUTPUT);
  }
  for (int i : SWITCH_PINS) {
    pinMode(i, INPUT_PULLUP);
  }
  pinMode(LED_BUILTIN, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  for (int i = 0; i < 4; i++) {
    bool pressed = digitalRead(SWITCH_PINS[i]) == LOW;
    if (pressed) {
      // Serial.print("pin pressed: ");
      // Serial.println(i);
    }
    bool debounced = buttons[i].update(pressed);
    if (!debounced && buttons[i].lastOn() > ATTRACT_TIME && (i == 2 || i == 0)) {
      debounced = buttons[i].update(true);
    }
    digitalWrite(RELAY_PINS[i], debounced ? LOW : HIGH);
    if (i == 3) {
      digitalWrite(LED_BUILTIN, debounced ? HIGH : LOW);
    }
  }
}
