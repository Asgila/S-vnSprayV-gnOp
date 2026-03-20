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

int lightState = 0; 

void setup() {
    delay(500); // Short delay for stability
Serial.begin(115200);        
    myBuzzer.begin();
    pinMode(LED_PIN_red, OUTPUT);
    pinMode(LED_PIN_yellow, OUTPUT);
    pinMode(LED_PIN_green, OUTPUT);
    
    setupDisplay();
    DisplayText("System Online");
    setup_wifi();
    delay(2000);
    mqqt_setup();
}

void loop() {
    // looks for updates
    tracker.update();
    myBuzzer.update();
    updateServo();

    unsigned long currentTime = millis();
// DO NOT DELETE BEFORE THIS!!!

    // if keypress turn on green light 
    if (tracker.wasKeyPressed()) {
        GreenOn();
    } else {
        GreenOff();
    }


    // Servo every 5 seconds
    if (currentTime - lastServoTime >= 5000) {
        startServoCycle();
        lastServoTime = currentTime;
    }

    // Red and yellow blinking
    if (currentTime - lastLightTime >= 1000) {
        if (lightState == 0) {
            RedOn();
            YellowOn();
            lightState = 1;
        } else {
            RedOff();
            YellowOff();
            lightState = 0;
        }
        lastLightTime = currentTime;
    }

    // display distance every 5 seconds
    if (currentTime - lastSensorTime >= 500) {
        float distance = distSensor.GetDistance();
        String distanceMessage = "Dist: " + String(distance) + " cm";
        DisplayText(distanceMessage);
        lastSensorTime = currentTime;
    }

    if (currentTime - lastBuzzerTime >=8000){
        myBuzzer.beep(50);
        lastBuzzerTime = currentTime;
    }
    
    if (!client.connected()) {
        mqtt_reconnect(); 
    }
    client.loop();
}