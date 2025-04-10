#include <Arduino.h>
#include <ArduinoJson.h>

#define MAX_SENSORS 10
#define CONFIG_FLASH_OFFSET (FLASH_SIZE - 4096) // last 4KB sector
#define CONFIG_MAGIC 0x544F5543  // 'TOUC'

struct NodeConfig {
  int nodeId;
  int gpio;
  bool lastState;
};

struct ClusterConfig {
  int clusterId;
  int sensorCount;
  NodeConfig sensors[MAX_SENSORS];
};

ClusterConfig config;
bool configLoaded = false;
unsigned long lastPeriodic;
unsigned long nextIntervalMs = 60000;

// --- Utility ---
void setNextInterval() {
  nextIntervalMs = 60000 + (random(10000) - 5000);  // jitter ±5s
  lastPeriodic = millis();
}

// --- Flash Config ---
bool loadConfigFromFlash() {
  // Flash reading for configuration here (not directly supported in Arduino, adjust accordingly)
  // For now, returning true to simulate config load.
  return true;
}

void saveConfigToFlash() {
  // Flash writing for configuration here (not directly supported in Arduino, adjust accordingly)
}

// --- JSON Parsing ---
void parseConfig(const char* json) {
  StaticJsonDocument<1024> doc;
  if (deserializeJson(doc, json)) return;

  config.clusterId = doc["clusterId"];
  JsonArray nodes = doc["nodes"];
  config.sensorCount = nodes.size() > MAX_SENSORS ? MAX_SENSORS : nodes.size();
  for (int i = 0; i < config.sensorCount; i++) {
    config.sensors[i].nodeId = nodes[i]["nodeId"];
    config.sensors[i].gpio = nodes[i]["touchSensorGPIOPin"];
    config.sensors[i].lastState = false;
    pinMode(config.sensors[i].gpio, INPUT);
  }

  saveConfigToFlash();
  configLoaded = true;
  setNextInterval();
}

// --- Messaging ---
void sendTouchEvent(int idx) {
  StaticJsonDocument<256> doc;
  doc["eventType"] = "touch_event";
  doc["clusterId"] = config.clusterId;
  JsonArray arr = doc.createNestedArray("sensorData");
  JsonObject obj = arr.createNestedObject();
  obj["nodeId"] = config.sensors[idx].nodeId;
  obj["touched"] = config.sensors[idx].lastState;
  serializeJson(doc, Serial);
  Serial.println();
}

void sendPeriodicStatus() {
  StaticJsonDocument<512> doc;
  doc["eventType"] = "periodic_touch_status";
  doc["clusterId"] = config.clusterId;
  JsonArray arr = doc.createNestedArray("sensorData");
  for (int i = 0; i < config.sensorCount; i++) {
    JsonObject obj = arr.createNestedObject();
    obj["nodeId"] = config.sensors[i].nodeId;
    obj["touched"] = config.sensors[i].lastState;
  }
  serializeJson(doc, Serial);
  Serial.println();
}

// --- Input ---
void handleIncomingSerial() {
  static char buf[1024];
  static int idx = 0;

  while (Serial.available()) {
    char ch = Serial.read();
    if (ch == '\n' || idx >= 1023) {
      buf[idx] = 0;
      if (!configLoaded) parseConfig(buf);
      idx = 0;
    } else {
      buf[idx++] = ch;
    }
  }
}

// --- Main ---
void setup() {
  Serial.begin(115200);
  delay(500);  // allow time for USB connect

  if (loadConfigFromFlash()) {
    configLoaded = true;
    setNextInterval();
  }
}

void loop() {
  handleIncomingSerial();

  if (!configLoaded) {
    delay(10);
    return;
  }

  for (int i = 0; i < config.sensorCount; i++) {
    bool state = digitalRead(config.sensors[i].gpio);
    if (state != config.sensors[i].lastState) {
      config.sensors[i].lastState = state;
      sendTouchEvent(i);
    }
  }

  if (millis() - lastPeriodic > nextIntervalMs) {
    sendPeriodicStatus();
    setNextInterval();
  }

  delay(10);
}
