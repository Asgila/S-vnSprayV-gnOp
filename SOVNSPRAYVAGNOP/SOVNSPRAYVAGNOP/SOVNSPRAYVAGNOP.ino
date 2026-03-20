#include <Arduino.h>
#include "servo.h"
#include "ledcontrol.h"
#include "display.h"
#include "distancsensor.h"
#include "keypress.h"
#include "buzzer.h"
#include "mqtt.h"

keyTracker tracker;
Ultrasonic distSensor(18, 19);
Buzzer myBuzzer(21);

// Timing variables
unsigned long lastServoTime = 0;
unsigned long lastLightTime = 0;
unsigned long lastSensorTime = 0;
unsigned long lastBuzzerTime = 0;
unsigned long lastKeyTime = 0;

int dist = 0;
int lightState = 0; 

void setup() {
    delay(500); // Short delay for stability
    Serial.begin(115200);        
    myBuzzer.begin();
    pinMode(LED_PIN_red, OUTPUT);
    pinMode(LED_PIN_yellow, OUTPUT);
    pinMode(LED_PIN_green, OUTPUT);
    
    setupDisplay();
    setup_wifi();
    delay(2000);
    mqqt_setup();
}

void buzz(int interval, int duration, bool status) {
    unsigned long currentTime = millis();
    if (status && (currentTime - lastBuzzerTime >= interval)) {
        myBuzzer.beep(duration);
        lastBuzzerTime = currentTime;
    }
}

void active() {
    RedOff();
    YellowOff();
    GreenOn();
    DisplayText("Keep going !");
}

void inactive() {
    RedOff();
    YellowOn();
    GreenOff();
    DisplayText("Get back to work");
}

void attack() {
    RedOn();
    YellowOff();
    GreenOff();
    DisplayText("Fuck You !!!!!!!");
}






void loop() {
    // looks for updates
    tracker.update();
    myBuzzer.update();
    updateServo();
    float distance = distSensor.GetDistance();
    unsigned long currentTime = millis();
    lastKeyTime = tracker.wasKeyPressed();
// DO NOT DELETE BEFORE THIS!!!

    switch (dashboardSwitchState)
    {
    case true:
        
        DisplayText("System Online");
        dist = dashboardSliderValue;
        if (distance < dist) {

            GreenOff();
            YellowOff();
            RedOn();
            DisplayText("Too far away");
            buzz(1000, 500, true); // Beep every 1 second for 500ms
        }

        if (currentTime - lastKeyTime < 1000) { // If key was pressed in the last second
            active();
        }

        if ((currentTime - lastKeyTime >= 1000) && (currentTime - lastKeyTime < 5000)) { // If key was pressed in the last second
            inactive();
            buzz(1000, 500, true); // Beep every 1 second for 500ms
        }

        if (currentTime - lastKeyTime >= 5000) { // If key was pressed in the last second
            attack();
            buzz(500, 300, false); // Beep every 1 second for 500ms
        }

        break;
    
    default:
    
        DisplayText("System Offline");
        break;
    } 




    if (!client.connected()) {
        mqtt_reconnect(); 
    }
    client.loop();
}