#ifndef WORKER_H
#define WORKER_H

#include <mutex>
#include <Adafruit_NeoPixel.h>
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>

#include "core/Cluster.h"

class Worker {

public:
    explicit Worker(int clusterId);
    void setup();
    void connectMqtt();
    void connectWifi();
    void loop();
    void handleMQTTMessages();
    static void MQTTcallback(char* topic, byte* payload, unsigned int length);
    void updateLEDs();
    void readSensors();

private:
    int clusterId;
    int incomingMsgCount;
    bool hasUpdates;
    int ledUpdates;
    Cluster cluster;
    Adafruit_NeoPixel strip;
    mutable std::mutex bufferMutex;
    static QueueHandle_t mqttQueue;
    void initializeSensors();
    void initializeMQTT();
};

#endif // WORKER_H