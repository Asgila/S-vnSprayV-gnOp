#ifndef BUZZER_H
#define BUZZER_H

#include <Arduino.h>

class Buzzer {
  private:
    int _pin;
    unsigned long _stopTime;
    bool _isActive;

  public:
    Buzzer(int pin) {
      _pin = pin;
      _isActive = false;
      _stopTime = 0;
    }

    void begin() {
      pinMode(_pin, OUTPUT);
      digitalWrite(_pin, LOW);
    }

    // Beep for a specific duration 
    void beep(int duration = 100) {
      digitalWrite(_pin, HIGH);
      _stopTime = millis() + duration;
      _isActive = true;
    }

    void update() {
      if (_isActive && millis() >= _stopTime) {
        digitalWrite(_pin, LOW);
        _isActive = false;
      }
    }
};

#endif
