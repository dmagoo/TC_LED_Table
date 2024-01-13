#ifndef WORKER_H
#define WORKER_H

// #include <FastLED.h>
#include "Cluster.h"

class Worker {
private:
  //  Adafruit_NeoPixel strip; // NeoPixel strip object
//    Cluster clusterConfig;   // Private cluster object

    void initializeSensors();
    void initializeMQTT();

public:
    //Worker(uint16_t numLEDs, uint8_t pin, neoPixelType type = NEO_GRBW + NEO_KHZ800);
    //~Worker();
    Worker();
    void setup();
    void handleMQTTMessages();
    void updateLEDs();
    void readSensors();

};

#endif // WORKER_H