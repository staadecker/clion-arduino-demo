#include <Arduino.h>
#include "lib/demo.h"
//#include <SoftwareSerial.h>

void setup() {
    Serial.begin(9600);
    Serial.println("Running main");
    test();
    //SoftwareSerial* softwareSerial = new SoftwareSerial(3,4);
}

void loop() {
}
