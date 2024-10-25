#include "Motors.h"
#include <Arduino.h>
#include <ArduinoSTL.h>
#include <SPI.h>


Motors::Motors(int m1p1, int m1p2, int m1spd, int m2p1, int m2p2, int m2spd) {

    // Assign Pin Vars
    pinM1p1 = m1p1;
    pinM1p2 = m1p2;
    pinM1spd = m1spd;
    pinM2p1 = m2p1;
    pinM2p2 = m2p2;
    pinM2spd = m2spd;

    // Initilize Motor 1 Pins
    pinMode(pinM1p1, OUTPUT);
    pinMode(pinM1p2, OUTPUT);
    pinMode(pinM1spd, OUTPUT);

    // Initilize Motor 2 Pins
    pinMode(pinM2p1, OUTPUT);
    pinMode(pinM2p2, OUTPUT);
    pinMode(pinM2spd, OUTPUT);

}

void Motors::f(int spd) {

    // Set Motor 1
    digitalWrite(pinM1p1, HIGH);
    digitalWrite(pinM1p2, LOW);
    analogWrite(pinM1spd, spd);

    // Set Motor 2

    digitalWrite(pinM2p1, HIGH);
    digitalWrite(pinM2p2, LOW);
    analogWrite(pinM2spd, spd);
    
}
