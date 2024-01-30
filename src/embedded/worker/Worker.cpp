#include "embedded/worker/Worker.h"
#include "config/make_cluster_config.h"
#include "config/worker_settings.h"
#include "core/ClusterMessage.h"
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

#define MAX_MESSAGE_SIZE 512
#define MAX_TOPIC_SIZE 64

int i = 0;

const char *wifi_ssid = WIFI_SSID;
const char *wifi_password = WIFI_PASSWORD;
const char *mqttServer = MQTT_BROKER_HOST;

WiFiClient espClient;
PubSubClient mqttClient(espClient);

// ESP32 240MHz, 320KB RAM, 4MB Flash

struct MQTTMessage {
    // String topic;
    char topic[MAX_TOPIC_SIZE];
    // byte *payload;
    byte payload[MAX_MESSAGE_SIZE]; // Fixed-size buffer
    unsigned int length;
};

Worker::Worker(int clusterId)
    : cluster(clusterId, makeNodeConfigs(clusterId)),
      strip(cluster.getPixelCount(), DEFAULT_LED_PIN, NEO_GRBW + NEO_KHZ800), hasUpdates(false), incomingMsgCount(0), ledUpdates(0) {
}

QueueHandle_t Worker::mqttQueue;

void Worker::setup() {
    strip.setBrightness(DEFAULT_BRIGHTNESS);
    strip.begin();
    strip.show();

    connectWifi();
    connectMqtt();

    Serial.println("setting queue with size " + String(QUEUE_SIZE));
    mqttQueue = xQueueCreate(QUEUE_SIZE, sizeof(MQTTMessage));
}

void Worker::connectWifi() {
    Serial.print("connecting wifi");
    WiFi.begin(wifi_ssid, wifi_password);
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
    }
    Serial.println("\nWiFi connected");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
}

void Worker::connectMqtt() {

    mqttClient.setServer(mqttServer, 1883);
    mqttClient.setCallback(Worker::MQTTcallback);

    if (mqttClient.connect(MQTT_CLIENT_ID)) {
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
}

// void Worker::loop() {
//     Serial.printin('tick');
//     mqttClient.loop();
// }

void Worker::loop() {
    static unsigned long lastTickTime = millis(); // Initialize the last tick time
    unsigned long currentTime = millis();
    unsigned long timeSinceLastTick = currentTime - lastTickTime;
    //    Serial.print("Time since last tick: ");
    //    Serial.println(timeSinceLastTick);
    // Serial.println("Led updates: " + String(ledUpdates));
    // Serial.println("msg: " + String(incomingMsgCount));
    if (WiFi.status() != WL_CONNECTED) {
        //        Serial.print("wifi: down");
    } else {
        //        Serial.print("wifi: up");
    }

    if (!mqttClient.connected()) {
        Serial.println("\tbroker: down --- reconnecting!");
        connectMqtt();
    } else {
        //        Serial.print("\tbroker: up");
    }
    //    Serial.println();

    mqttClient.loop();

    lastTickTime = currentTime; // Update the last tick time
}

void Worker::MQTTcallback(char *topic, byte *payload, unsigned int length) {
    //  Serial.print("Message received on topic: ");
    //  Serial.println(topic);
    // Serial.print("Message: ");
    /// for (int i = 0; i < length; i++) {
    //   Serial.print((char)payload[i]);
    //}
    // Serial.println();

    // Log the payload size
    // Serial.print("Payload size: ");
    // Serial.println(length);

    // dont put a pointer on the queue.. it will go bye bye
    // char *payloadCopy = new char[length + 1];
    // memcpy(payloadCopy, payload, length);
    // MQTTMessage message = {String(topic), (byte *)payloadCopy, length};
    MQTTMessage message;
    strncpy(message.topic, topic, MAX_TOPIC_SIZE);
    message.topic[MAX_TOPIC_SIZE - 1] = '\0';                                 // Ensure null termination
    message.length = (length < MAX_MESSAGE_SIZE) ? length : MAX_MESSAGE_SIZE; // Prevent buffer overflow
    memcpy(message.payload, payload, message.length);                         // Copy payload to fixed-size buffer
    // xQueueSendToBackFromISR(mqttQueue, &message, NULL);
    BaseType_t result = xQueueSendToBack(mqttQueue, &message, portMAX_DELAY);
    if (result != pdPASS) {
        Serial.println("Failed to enqueue MQTT message");
    }

    // Enqueue message to mqttQueue
}

void Worker::handleMQTTMessages() {
    // Serial.println("msgloop");
    MQTTMessage message;
    if (xQueueReceive(mqttQueue, &message, portMAX_DELAY)) {
        incomingMsgCount++;
        //  Serial.println("msg: " + String(incomingMsgCount));
        std::vector<uint8_t> buffer(message.payload, message.payload + message.length);
        ClusterMessage clusterMessage = deserializeClusterMessage(buffer);
        std::lock_guard<std::mutex> lock(bufferMutex);
        executeMessageCommand(clusterMessage, cluster);
        hasUpdates = true;
    }

    /*
        // temporary fake message handler just to make things do stuff
        // but this simulates what an unpacked mqtt message might do
        WRGB color1 = 0x33000033; // Example color 1
        WRGB color2 = 0x00330000; // Example color 2
        cluster.fillNode(0, (i % 2 == 0) ? color1 : color2);
        // Serial.println("Worker message handling");
     */
}

void Worker::updateLEDs() {
    if (!hasUpdates) {
        return;
    }
    //Serial.println("Worker LED Update");

    std::lock_guard<std::mutex> lock(bufferMutex);
    // TODO: make a ref version of this if speed becomes an issue
    std::vector<WRGB> buffer = cluster.getPixelBuffer();

    for (size_t i = 0; i < buffer.size(); ++i) {
        // Assuming WRGB is a uint32_t with your color data
        uint32_t color = buffer[i];
        strip.setPixelColor(i, color);
    }
    strip.show();
    hasUpdates = false;
    ledUpdates++;
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