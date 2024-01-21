#include <Arduino.h>
// #include <LittleFS.h>

#ifdef CONTROLLER_DEVICE
#include "controller/bootstrap.h"
#else
#include "worker/bootstrap.h"
#endif

void setup() {
    // Initialize LittleFS
    //if (!LittleFS.begin()) {
    //    Serial.println("LittleFS initialization failed");
    //    return;
    //}
    Serial.begin(115200);
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
