#ifndef WORKER_H
#define WORKER_H

#include <mutex>
#include <Adafruit_NeoPixel.h>

#include "native/Cluster.h"

class Worker {

public:
    explicit Worker(int clusterId);
    void setup();
    void handleMQTTMessages();
    void updateLEDs();
    void readSensors();


private:
    int clusterId;
    Cluster cluster;
    Adafruit_NeoPixel strip;
    mutable std::mutex bufferMutex;
    void initializeSensors();
    void initializeMQTT();
};

#endif // WORKER_H