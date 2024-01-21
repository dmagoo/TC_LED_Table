#include "embedded/worker/Worker.h"
// #include "config.h"
#include <Arduino.h>

//Worker::Worker(uint16_t numLEDs, uint8_t pin, neoPixelType type) : strip(numLEDs, pin, type) {
//    // Constructor implementation
//}
Worker::Worker() {

}

void Worker::setup() {
    Serial.println("Setting up the worker!");
}

void Worker::handleMQTTMessages() {
    Serial.println("Worker message handling");
}

void Worker::updateLEDs() {
    Serial.println("Worker LED Update");
}

void Worker::readSensors() {
    Serial.println("Worker sensor reading");
}

void Worker::initializeSensors() {
    Serial.println("Worker sensor initialization");    
}

void Worker::initializeMQTT() {
    Serial.println("Worker messge initialization");    
}