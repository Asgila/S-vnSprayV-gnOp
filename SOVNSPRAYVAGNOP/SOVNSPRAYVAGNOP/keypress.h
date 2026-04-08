#ifndef KEY_TRACKER_H
#define KEY_TRACKER_H

#include <Arduino.h>

class keyTracker {
  private:
    bool _hasActivity = false;

  public:
    void update() {
        // Read buffer 
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
        // Save current state
        bool currentState = _hasActivity;
        
        // Clear state after reading it!
        
        _hasActivity = false; 
        
        // Return the saved state
        return currentState;
    }
};

#endif