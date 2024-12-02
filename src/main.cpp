#include <Arduino.h>
#include <ArduinoSTL.h>
#include <SPI.h>
#include <SD.h>
#include <String>
#include <array>
#include <Wire.h>
#include <Adafruit_VL53L0X.h>
#include <vector>

// Import Class Files
#include "Motors.h"

// Pin Vars (motors)
int m1p1 = 24; 
int m1p2 = 25;
int m1spd = 23;
int m2p1 = 26;
int m2p2 = 27;
int m2spd = 22;

// Motor Encoder Pins
int m1e = 18;
int m2e = 19;

// Motor Encoder Vars
int m1c = 0;
int m2c = 0;

// Motor Encoder Functions
void tickm1();
void tickm2();

// Name of the config file (DO NOT TOUCH)
String fileName = "config.txt";

// Debug Mode (Turn off if no serial out)
bool debugMode = true;

// SD Card
File config;

// Time of Flight Sensor
Adafruit_VL53L0X lidar = Adafruit_VL53L0X();

// Initilize Motors
Motors car(m1p1, m1p2, m1spd, m2p1, m2p2, m2spd);

int lineCount;
std::vector<String> instructions;

void setup() {

  if (debugMode) Serial.begin(9600);

  Wire.begin();

  // SD Card
  if (!SD.begin(53)) {
    if (debugMode) {
      Serial.println("SD Card failed to Init");
    }
  } else {
    if (debugMode) {
      Serial.println("SD Card Init Successful");
    }
  }
  config = SD.open(fileName);
  if (debugMode){
    if (config) {
      Serial.println("Successfully opened " + fileName);
    } else {
      Serial.println("Could not open " + fileName);
    }
  }

  while (config.available()) {
    String line = config.readStringUntil('\n');
    lineCount++;
  }
  if (debugMode){
    Serial.println(lineCount);
  }

  config.close();
  config = SD.open(fileName);
  
  if (debugMode){
    if (config) {
      Serial.println("Successfully opened " + fileName);
    } else {
      Serial.println("Could not open " + fileName);
    }
  }

  for (int i = 0; i <= lineCount; i++) {
    instructions.push_back(config.readStringUntil('\n'));
  }
  if (debugMode){
    for (int i = 0; i <= lineCount - 1; i++) {
      Serial.println(instructions[i]);
    }
  }

  if (!lidar.begin()) {
    Serial.println("The LiDAR sensor isn't working");
  }

  pinMode(m1e, INPUT);
  pinMode(m2e, INPUT);
  attachInterrupt(digitalPinToInterrupt(m1e), tickm1, RISING);
  attachInterrupt(digitalPinToInterrupt(m2e), tickm2, RISING);

}
void loop() {

  // Init the Measure obj
  VL53L0X_RangingMeasurementData_t measure;

  for (int i = 0; i < lineCount; i++) {
    String instruction = instructions[i];

    // Init Distance var
    int distance;

    if (instruction[0] == 'f') {
      int speed, until;
      sscanf(instruction.c_str(), "f %d %d", &speed, &until);
      bool exited = false;
      while (!exited) {
        lidar.rangingTest(&measure, false);
        if (measure.RangeStatus != 4) { 
          distance = measure.RangeMilliMeter;
        } else {
          distance = -1;
        }
        if (debugMode) {
          Serial.println("Distance: " + String(distance) + " | Speed: " + String(speed));
        }
        if (!car.f(speed, distance, until)) exited = true;
      }
    } else if (instruction[0] == 'b') {
      int speed, until;
      sscanf(instruction.c_str(), "f %d %d", &speed, &until);
      bool exited = false;
      int avg = (m1c+m2c)/2;

      while (!exited) {
        exited = !car.b(speed, avg, until);
      }
      // car.b();
    } else if (instruction[0] == 'l') {
      bool exited = false;
      while (!exited) {
        if (debugMode) {
          Serial.println("Count M1: " + String(m1c) + " | Count M2: " + String(m2c));
        }
        exited = !car.l(m1c, m2c);
      }
    } else if (instruction[0] == 'r') {
      bool exited = false;
      while (!exited) {
        if (debugMode) {
          Serial.println("Count M1: " + String(m1c) + " | Count M2: " + String(m2c));
        }
        exited = !car.r(m1c, m2c);
      }
    } else {
      if (debugMode) Serial.println("How did you screw this part up???????");
      while (true);
    }
    
    // Reset the count Variables
    m1c = 0; m2c = 0;
  }
  while (true);
}

void tickm1 () {
  m1c++;
}

void tickm2 () {
  m2c++;
}
