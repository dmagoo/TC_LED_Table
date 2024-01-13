#include <Arduino.h>
#include <LittleFS.h>

#ifdef CONTROLLER_DEVICE
#include "controller/bootstrap.h"
#else
#include "worker/bootstrap.h"
#endif

void setup() {
    // Initialize LittleFS
    if (!LittleFS.begin()) {
        Serial.println("LittleFS initialization failed");
        return;
    }

    #ifdef CONTROLLER_DEVICE
    controllerSetup();
    #else
    workerSetup();
    #endif
}

void loop() {
    #ifdef CONTROLLER_DEVICE
    controllerLoop();
    #else
    workerLoop();
    #endif
}
