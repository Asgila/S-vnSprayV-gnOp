#ifndef KEY_TRACKER_H
#define KEY_TRACKER_H

class keyTracker {
    private:
        bool _hasActivity = false;

    public:

    void update() {
        if (Serial.available() > 0) {
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
            return _hasActivity;
        }
};

#endif