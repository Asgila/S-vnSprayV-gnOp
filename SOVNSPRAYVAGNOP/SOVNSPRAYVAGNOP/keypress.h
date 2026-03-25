#ifndef KEY_TRACKER_H
#define KEY_TRACKER_H

#include <Arduino.h>

class keyTracker {
  private:
    bool _hasActivity = false;

  public:
    void update() {
        // Read everything available in the buffer so we don't get backlogged
        while (Serial.available() > 0) {
            String input = Serial.readStringUntil('\n');
            input.trim(); 

            if (input == "key_pressed") {
                _hasActivity = true;
            }
            else if (input == "no_key_pressed"){
                _hasActivity = false;
            }
        }
    }

    bool wasKeyPressed() {
        // 1. Save the current state
        bool currentState = _hasActivity;
        
        // 2. Clear the state immediately after reading it!
        // This prevents the system from getting stuck if the PC misses a message.
        _hasActivity = false; 
        
        // 3. Return the saved state
        return currentState;
    }
};

#endif