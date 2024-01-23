#include "embedded/worker/Worker.h"
#include "config/make_cluster_config.h"
#include "core/LedTableTypes.h"

#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFi.h>

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1:
#define DEFAULT_LED_PIN 16

// NeoPixel brightness, 0 (min) to 255 (max)
#define DEFAULT_BRIGHTNESS 50

#define QUEUE_SIZE 50

int i = 0;

const char *wifi_ssid = WIFI_SSID;
const char *wifi_password = WIFI_PASSWORD;
const char *mqttServer = "test.mosquitto.org";

WiFiClient espClient;
PubSubClient mqttClient(espClient);

// ESP32 240MHz, 320KB RAM, 4MB Flash

struct MQTTMessage {
    String topic;
    byte *payload;
    unsigned int length;
};

Worker::Worker(int clusterId)
    : cluster(clusterId, makeNodeConfigs(clusterId)),
      strip(cluster.getPixelCount(), DEFAULT_LED_PIN, NEO_GRBW + NEO_KHZ800), hasUpdates(false) {
}

QueueHandle_t Worker::mqttQueue;


void Worker::setup() {
    strip.setBrightness(DEFAULT_BRIGHTNESS);
    strip.begin();
    strip.show();

    Serial.print("connecting wifi");
    WiFi.begin(wifi_ssid, wifi_password);
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
    }
    Serial.println("\nWiFi connected");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    mqttClient.setServer(mqttServer, 1883);
    mqttClient.setCallback(Worker::MQTTcallback);

    if (mqttClient.connect("TCLEDTABLE_CLUSTER0")) {
        Serial.println("Connected to MQTT broker");
        String topic = "ledtable/cluster/" + String(clusterId) + "/command";
        if (mqttClient.subscribe(topic.c_str())) {
            Serial.println("Subscribed to topic: " + topic);
        } else {
            Serial.println("Failed to subscribe to topic: " + topic);
        }
    } else {
        Serial.println("Failed to connect to MQTT broker");
    }
    mqttQueue = xQueueCreate(QUEUE_SIZE, sizeof(MQTTMessage));
}

// In Worker.cpp
void Worker::MQTTcallback(char *topic, byte *payload, unsigned int length) {
    Serial.print("Message received on topic: ");
    Serial.println(topic);
    Serial.print("Message: ");
    for (int i = 0; i < length; i++) {
        Serial.print((char)payload[i]);
    }
    Serial.println();

    // dont put a pointer on the queue.. it will go bye bye
    char *payloadCopy = new char[length + 1];
    memcpy(payloadCopy, payload, length);

    MQTTMessage message = {String(topic), (byte *)payloadCopy, length};
    xQueueSendToBackFromISR(mqttQueue, &message, NULL);

    // Enqueue message to mqttQueue
}

void Worker::handleMQTTMessages() {
    MQTTMessage message;
    // note that this is a blocking call
    if (xQueueReceive(mqttQueue, &message, portMAX_DELAY)) {
        Serial.println("handlemsg");
        // Process message
        // e.g., unpack and execute command
    }
    // Free the copied payload
    delete[] message.payload;

    /*
        std::lock_guard<std::mutex> lock(bufferMutex);
        // temporary fake message handler just to make things do stuff
        // but this simulates what an unpacked mqtt message might do
        RGBW color1 = 0x33000033; // Example color 1
        RGBW color2 = 0x00330000; // Example color 2
        cluster.fillNode(0, (i % 2 == 0) ? color1 : color2);
        // Serial.println("Worker message handling");
     */
}

void Worker::updateLEDs() {
    i++;

    if (!hasUpdates) {
        return;
    }

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