#include <Arduino.h>
#include "lib/demo.h"

// the setup function runs once when you press reset or power the board
void setup() {
    // initialize digital pin LED_BUILTIN as an output.
    Serial.begin(9600);
    Serial.println("test1");
    test();
}

// the loop function runs over and over again forever
void loop() {
}
