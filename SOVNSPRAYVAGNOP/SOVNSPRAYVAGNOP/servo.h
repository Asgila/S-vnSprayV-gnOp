#ifndef SERVO_CONTROL_H
#define SERVO_CONTROL_H

#include <ESP32Servo.h>

const int SERVO_PIN = 15;
Servo myServo;

// Variables to track state
unsigned long servoMoveStartTime = 0;
bool servoIsMoving = false;
int servoStep = 0; // 0: Idle, 1: Moving to 180, 2: Moving to 0

void startServoCycle() {
    if (!servoIsMoving) {
        myServo.attach(SERVO_PIN);
        myServo.write(180); 
        servoMoveStartTime = millis();
        servoIsMoving = true;
        servoStep = 1;
    }
}

void updateServo() {
    if (!servoIsMoving) return;

    unsigned long currentTime = millis();

    // wait 1 s
    if (servoStep == 1 && (currentTime - servoMoveStartTime >= 1000)) {
        myServo.write(0);
        servoMoveStartTime = currentTime;
        servoStep = 2;
    }
    // wait 1 s
    else if (servoStep == 2 && (currentTime - servoMoveStartTime >= 1000)) {
        myServo.detach();
        servoIsMoving = false;
        servoStep = 0;
    }
}

#endif