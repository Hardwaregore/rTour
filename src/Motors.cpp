#include "Motors.h"
#include <Arduino.h>
#include <ArduinoSTL.h>
#include <SPI.h>


Motors::Motors(int m1p1, int m1p2, int m1spd, int m2p1, int m2p2, int m2spd, bool useDebugMode) {

    // Assign Pin Vars
    pinM1p1 = m1p1;
    pinM1p2 = m1p2;
    pinM1spd = m1spd;
    pinM2p1 = m2p1;
    pinM2p2 = m2p2;
    pinM2spd = m2spd;
    debugMode = useDebugMode;

    // Initilize Motor 1 Pins
    pinMode(pinM1p1, OUTPUT);
    pinMode(pinM1p2, OUTPUT);
    pinMode(pinM1spd, OUTPUT);

    // Initilize Motor 2 Pins
    pinMode(pinM2p1, OUTPUT);
    pinMode(pinM2p2, OUTPUT);
    pinMode(pinM2spd, OUTPUT);

}

bool Motors::f(long m1c, long m2c, double until) {
    int avg = (m1c + m2c) / 2;
    double distance = avg / (2750.0);
    if (debugMode) {
        Serial.println("Average: " + String(avg) + " (M1: " + String(m1c) + ", M2: " + String(m2c) + ") | Revolutions: " + String(distance));
    }
    if (distance >= (until)) {

        // Set Motor 1
        digitalWrite(pinM1p1, HIGH);
        digitalWrite(pinM1p2, LOW);
        analogWrite(pinM1spd, 0);

        // Set Motor 2
        digitalWrite(pinM2p1, HIGH);
        digitalWrite(pinM2p2, LOW);
        analogWrite(pinM2spd, 0);

        return true;

    } else {

        // Set Motor 2

        digitalWrite(pinM2p1, HIGH);
        digitalWrite(pinM2p2, LOW);
        analogWrite(pinM2spd, 255);

        // delay(100);

        // Set Motor 1
        digitalWrite(pinM1p1, HIGH);
        digitalWrite(pinM1p2, LOW);
        analogWrite(pinM1spd, 250);

        return false;
    }
}

bool Motors::l(int numRotationsM1, int numRotationsM2) {
    if (numRotationsM1 / 1550 == 1 && numRotationsM2 / 1550 == 1) {
        // Set Motor 1
        digitalWrite(pinM1p1, HIGH);
        digitalWrite(pinM1p2, LOW);
        analogWrite(pinM1spd, 0);

        // Set Motor 2

        digitalWrite(pinM2p1, HIGH);
        digitalWrite(pinM2p2, LOW);
        analogWrite(pinM2spd, 0);

        return false;
    } else {
        // Set Motor 1
        digitalWrite(pinM1p1, HIGH);
        digitalWrite(pinM1p2, LOW);
        analogWrite(pinM1spd, 250);

        // Set Motor 2

        digitalWrite(pinM2p1, LOW);
        digitalWrite(pinM2p2, HIGH);
        analogWrite(pinM2spd, 250);

        return true;
    }
}

bool Motors::r(int numRotationsM1, int numRotationsM2) {
    if (numRotationsM1 / 1550 == 1 && numRotationsM2 / 1550 == 1) {
        // Set Motor 1
        digitalWrite(pinM1p1, HIGH);
        digitalWrite(pinM1p2, LOW);
        analogWrite(pinM1spd, 0);

        // Set Motor 2

        digitalWrite(pinM2p1, HIGH);
        digitalWrite(pinM2p2, LOW);
        analogWrite(pinM2spd, 0);

        return false;
    } else {
        // Set Motor 1
        digitalWrite(pinM1p1, LOW);
        digitalWrite(pinM1p2, HIGH);
        analogWrite(pinM1spd, 252);

        // Set Motor 2

        digitalWrite(pinM2p1, HIGH);
        digitalWrite(pinM2p2, LOW);
        analogWrite(pinM2spd, 255);

        return true;
    }
}