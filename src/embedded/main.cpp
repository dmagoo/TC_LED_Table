#include <Arduino.h>

#include "worker/bootstrap.h"

void setup() {
    Serial.begin(115200);
    Serial.println("Starting setup");
    workerSetup();
}

void loop() {
    workerLoop();
    // Serial.println("Hello, from worker loop!");
    delay(10); 
}
