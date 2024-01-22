#include "embedded/worker/Worker.h"
#include "native/LedTableTypes.h"
#include "config/make_cluster_config.h"

#include <Arduino.h>
#include <mutex>

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1:
#define DEFAULT_LED_PIN 16

// NeoPixel brightness, 0 (min) to 255 (max)
#define DEFAULT_BRIGHTNESS 50

int i = 0;

// ESP32 240MHz, 320KB RAM, 4MB Flash

Worker::Worker(int clusterId) 
    : cluster(clusterId, makeNodeConfigs(clusterId)), 
      strip(cluster.getPixelCount(), DEFAULT_LED_PIN, NEO_GRBW + NEO_KHZ800) {
}


void Worker::setup() {

    Serial.println("Setting up the worker!");

    strip.setBrightness(DEFAULT_BRIGHTNESS);
    strip.begin();
    strip.show();
}

void Worker::handleMQTTMessages() {
    std::lock_guard<std::mutex> lock(bufferMutex);
    // temporary fake message handler just to make things do stuff
    RGBW color1 = 0x33000033; // Example color 1
    RGBW color2 = 0x00330000; // Example color 2
    cluster.fillNode(0, (i % 2 == 0) ? color1 : color2);
    Serial.println("Worker message handling");
    delay(1000);
}

void Worker::updateLEDs() {
    i++;
    std::lock_guard<std::mutex> lock(bufferMutex);
    // TODO: make a ref version of this if speed becomes an issue
    std::vector<RGBW> buffer = cluster.getPixelBuffer();

    for (size_t i = 0; i < buffer.size(); ++i) {
        // Assuming RGBW is a uint32_t with your color data
        uint32_t color = buffer[i];
        strip.setPixelColor(i, color);
    }
    strip.show();
    Serial.println("Worker LED Update");
}

void Worker::readSensors() {
    // Serial.println("Worker sensor reading");
}

void Worker::initializeSensors() {
    Serial.println("Worker sensor initialization");
}

void Worker::initializeMQTT() {
    Serial.println("Worker messge initialization");
}