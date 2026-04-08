#ifndef ULTRASONIC_HELPER_H
#define ULTRASONIC_HELPER_H




class Ultrasonic {
  private:
    int _trigPin;
    int _echoPin;
    float lastDistance = 0.0;

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
      
      // distance in centimeters
      float distance = (duration * 0.0343) / 2;
      if (distance > 0){
        lastDistance=distance;
      }

      return lastDistance;
    }
};

#endif