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
unsigned long lastTime = 0;
unsigned long lastLightTime = 0;
unsigned long lastAlarmTime = 0;
unsigned long lastBuzzerTime = 0;
int distanceMultiplier=50;
unsigned long maxDistance=300;
int secondsNoPress = 0;
int lightState = 0; 
int alarmTime = 3;

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
    if (!client.connected()) {
        mqtt_reconnect();
    }
    tracker.update();
    myBuzzer.update();
    updateServo();
    client.loop();
    delay(50);//Delay to add stability
    unsigned long currentTime = millis();

    
    if (dashboardSwitchState){//If node-red is on 
        GreenOn();

        bool keyPressed = tracker.wasKeyPressed();

        float threshold = (float)dashboardSliderValue * (float)distanceMultiplier;
        float currentDist = distSensor.GetDistance();
        Serial.printf("the distance now is %f\n");

        if (threshold>currentDist){//If it is inside distance
            if (!keyPressed && (currentTime-lastTime)>1010){//If no key was pressed and a second has passed
                secondsNoPress += 1;
                lastTime=currentTime;
            }
            else if (keyPressed){//If a key has been pressed
                secondsNoPress = 0;
                RedOff();
                YellowOff();
                DisplayText("Great Job");

            }

            

            if (secondsNoPress>alarmTime){//If alarm and spray should be on
                RedOn();
                DisplayText("GET BACK TO WORK");
                if (currentTime-lastAlarmTime>2200){
                    lastAlarmTime = currentTime;
                    startServoCycle();
                    myBuzzer.beep(400);
                }


            }
            else if (secondsNoPress>alarmTime/2){//if the yellow led should be on 
                YellowOn();
                DisplayText("Get working");

            }

            else if (secondsNoPress<alarmTime/2){//if the yellow led should be off
                YellowOff();
                RedOff();
            }


        }
        else if(threshold<currentDist){//If it is out of distance
            DisplayText("Come Closer");
            secondsNoPress = 0;
            Serial.print("ouside distance\n");
            RedOn();
            YellowOn();
            myBuzzer.beep(25);
        }

        

    }
    else {
    //No led to signify off
        GreenOff();
        RedOff();
        YellowOff();
        secondsNoPress = 0;
        Serial.print("NODE-RED OFF\n");
        DisplayText("Machine Off");
        

    }

}
