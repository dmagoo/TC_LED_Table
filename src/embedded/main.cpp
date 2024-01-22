#include <Arduino.h>

#ifdef CONTROLLER_DEVICE
#include "controller/bootstrap.h"
#else
#include "worker/bootstrap.h"
#endif

void setup() {
    Serial.begin(115200);
    Serial.println("Hello, from main setup!");
    #ifdef CONTROLLER_DEVICE
    controllerSetup();
    #else
    workerSetup();
    #endif
}

void loop() {
    Serial.println("Hello, from main loop!");
    #ifdef CONTROLLER_DEVICE
    controllerLoop();
    #else
    workerLoop();
    #endif
    delay(1000); 
}
