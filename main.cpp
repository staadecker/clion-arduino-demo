#include <Arduino.h>
#include "lib/demo.h"

void setup() {
    Serial.begin(9600);
    Serial.println("Running main");
    test();
}

void loop() {
}
