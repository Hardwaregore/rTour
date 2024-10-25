#include <Arduino.h>
#include <ArduinoSTL.h>
#include <SPI.h>
#include <SD.h>
#include <String>
#include <array>

// Import Class Files
#include "Motors.h"

// Pin Vars
int m1p1 = 2;
int m1p2 = 3;
int m1spd = 8;

// Name of the config file (DO NOT TOUCH)
String fileName = "config.txt";

// Debug Mode (Turn off if no serial out)
bool debugMode = true;

// SD Card
File config;

// Initilize Motors
Motors car(m1p1, m1p2, m1spd, 23, 24, 25);

void setup() {
  if (debugMode){
    Serial.begin(9600);
  }


  // SD Card
  if (debugMode){
    if (!SD.begin(53)) {
      Serial.println("SD Card failed to Init");
    } else {
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

  int lineCount = 0;
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

  String instructions[lineCount];
  for (int i = 0; i <= lineCount; i++) {
    instructions[i] = config.readStringUntil('\n');
  }
  if (debugMode){
    for (int i = 0; i <= lineCount - 1; i++) {
      Serial.println(instructions[i]);
    }
  }


}
void loop() {
  car.f(100);
}
