#include <Arduino.h>
#include <ArduinoSTL.h>
#include <SPI.h>
#include <SD.h>

File configFile;

int m1p1 = 2;
int m1p2 = 3;

void setup() {
  pinMode(m1p1, OUTPUT);
  pinMode(m1p2, OUTPUT);
  pinMode(8, OUTPUT);
  /*
  Serial.begin(9600);
  if (!SD.begin(53)) {
    Serial.println("There was an error");
    while (1);
  }
  Serial.println("Success");

  configFile = SD.open("config.txt");
  if (configFile) {
    while (configFile.available()){
      Serial.println(configFile.readString());
    }
  } else {
    Serial.println("There was an error");
  }
  */
}

void loop() {
  digitalWrite(m1p1, HIGH);
  digitalWrite(m1p2, LOW);
  analogWrite(8, 255);
}
