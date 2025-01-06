#include <Arduino.h>
#include <ArduinoSTL.h>
#include <SPI.h>
#include <SD.h>
#include <array>
#include <Wire.h>
#include <Adafruit_VL53L0X.h>
#include <vector>
#include <cmath>

// Import Class Files
#include "Motors.h"

// Pin Vars (motors)
int m1p1 = 24; 
int m1p2 = 25;
int m1spd = 7;
int m2p1 = 26;
int m2p2 = 27;
int m2spd = 6;

// Motor Encoder Pins
int m1e = 18;
int m2e = 19;

// Motor Encoder Vars
long m1c = 0;
long m2c = 0;

// Motor Encoder Functions
void tickm1();
void tickm2();

// Name of the config file (DO NOT TOUCH)
String fileName = "config.txt";

// Debug Mode (Turn off if no serial out)
bool debugMode = true;
bool verboseOutput = false;

// SD Card
File config;

// Time of Flight Sensor
Adafruit_VL53L0X lidar = Adafruit_VL53L0X();

// Initilize Motors
Motors car(m1p1, m1p2, m1spd, m2p1, m2p2, m2spd, debugMode, verboseOutput);

int lineCount;
std::vector<String> instructions;

void setup() {

  if (debugMode) Serial.begin(9600);

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);

  Wire.begin();

  // SD Card
  if (!SD.begin(53)) {
    if (debugMode) {
      Serial.println("SD Card failed to Init");
    }
    while (true) {
      digitalWrite(13, HIGH);
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

  config.close();
  config = SD.open(fileName);

  for (int i = 0; i <= lineCount; i++) {
    instructions.push_back(config.readStringUntil('\n'));
  }
  if (debugMode){
    Serial.println("Detected " + String(lineCount) + " instruction(s) in " + fileName + ":");
    for (int i = 0; i <= lineCount - 1; i++) {
      Serial.println("\t" + instructions[i]);
    }
  }

  if (!lidar.begin()) {
    Serial.println("The LiDAR sensor isn't working");
    while (true) {
      digitalWrite(13, HIGH);
    }
  }

  pinMode(m1e, INPUT);
  pinMode(m2e, INPUT);
  attachInterrupt(digitalPinToInterrupt(m1e), tickm1, RISING);
  attachInterrupt(digitalPinToInterrupt(m2e), tickm2, RISING);

}
void loop() {

  // Init the Measure obj
  VL53L0X_RangingMeasurementData_t measure;

  // Init Distance var
  int distance;

  // Begin Button
  bool init = false;
  bool begin = false;

  // Button sensitvity in mm
  int sensitivity = 40;

  // Button
  while (true) {
    lidar.rangingTest(&measure, false);
    if (measure.RangeStatus != 4) { 
      distance = measure.RangeMilliMeter;
    } else {
      distance = -1;
    }
    
    // Set LED
    digitalWrite(12, HIGH);

    // Check Button Presses
    if (begin) {
      break;
    } else if (init) {
      if (distance > sensitivity + 10 || distance == -1) {
        begin = true;
        Serial.println("Set Begin to true");
      }
    } else if (!init && distance <= sensitivity && distance != -1) {
      init = true;
      if (debugMode) {
        Serial.println("Set Init to true");
      }
    }

    // Set LED again
    digitalWrite(12, HIGH);
  }

  // Reset LED
  digitalWrite(12, LOW);

  for (int i = 0; i < lineCount; i++) {
    String instruction = instructions[i];

    // Reset the count Variables
    m1c = 0; m2c = 0;

    if (instruction[0] == 'f') {
      // Define variables
      int until;
      int speed;

      // Set until var
      sscanf(instruction.c_str(), "f %d %d", &until, &speed);

      if (until == 420) {
        if (debugMode) {
          Serial.println("\"" + instruction + "\" is a special instruction! Used " + String(until) + " as until value");
        }

        bool exited = false;
        while (!exited) {
          exited = car.f(m1c, m2c, true);
        }
      } else {
        double numRotationsNeeded = until / 500;
        if (debugMode) {
          Serial.println("Used " + String(until) + " as the until value. Will more forwards 500mm " + String(numRotationsNeeded) + " times");
        }

        for (int i = 0; i < ceil(numRotationsNeeded); i++) {
          m1c = 0;
          m2c = 0;

          bool exited = false;
          while (!exited) {
            exited = car.f(m1c, m2c, false);
          }

          delay(1000);
        }
      }
    } else if (instruction[0] == 'l') {
      bool exited = false;
      while (!exited) {
        if (debugMode && verboseOutput) {
          Serial.println("Average: " + String((m1c + m2c) / 2) + " (M1: " + String(m1c) + ", M2: " + String(m2c) + ")");
        }
        exited = !car.l(m1c, m2c);
      }
    } else if (instruction[0] == 'r') {
      bool exited = false;
      while (!exited) {
        if (debugMode && verboseOutput) {
          Serial.println("Average: " + String((m1c + m2c) / 2) + " (M1: " + String(m1c) + ", M2: " + String(m2c) + ")");
        }
        exited = !car.r(m1c, m2c);
      }
    } else if (instruction[0] == 'b') {
      if (debugMode) {
        Serial.println("\"" + instruction + "\" is a special instruction! Used " + String(170) + " as until value");
      }

      bool exited = false;
      while (!exited) {
        exited = car.b(m1c, m2c);
      }
    } else if (instruction[0] == 'e') {
      while (true) {
        digitalWrite(13, HIGH);
        delay(500);
        digitalWrite(13, LOW);
        delay(500);
        if (debugMode) {
          Serial.println("ENDED");
        }
      }
    } else {
      if (debugMode) Serial.println("How did you screw this part up???????");
      while (true) {
        digitalWrite(13, HIGH);
      }
    }
    // Reset the count Variables
    m1c = 0; m2c = 0;

    delay(1000);
  }

}

void tickm1 () {
  m1c++;
}

void tickm2 () {
  m2c++;
}
