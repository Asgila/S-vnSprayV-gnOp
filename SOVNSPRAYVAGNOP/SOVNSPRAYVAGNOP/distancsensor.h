#ifndef ULTRASONIC_HELPER_H
#define ULTRASONIC_HELPER_H




class Ultrasonic {
  private:
    int _trigPin;
    int _echoPin;

  public:
    Ultrasonic(int trigPin, int echoPin) {
      _trigPin = trigPin;
      _echoPin = echoPin;
      pinMode(_trigPin, OUTPUT);
      pinMode(_echoPin, INPUT);
    }

    // The GetDistance function
    float GetDistance() {
      digitalWrite(_trigPin, LOW);
      delayMicroseconds(2);

      digitalWrite(_trigPin, HIGH);
      delayMicroseconds(10);
      digitalWrite(_trigPin, LOW);

      long duration = pulseIn(_echoPin, HIGH,25000);
      if (duration == 0) {
        return 999.0; // Return a huge distance so the alarm doesn't trigger
      }
      // distance in centimeters
      float distance = (duration * 0.0343) / 2;

      return distance;
    }
};

#endif