#include <Arduino.h>
#include <ArduinoJson.h>
#include <LittleFS.h>

#define MAX_SENSORS 10
#define CONFIG_MAGIC 0x544F5543  // 'TOUC' - used to verify config validity

struct NodeConfig {
  int nodeId;
  int gpio;
  bool lastState;
};

struct ClusterConfig {
  int magic;
  int clusterId;
  int sensorCount;
  NodeConfig sensors[MAX_SENSORS];
};

ClusterConfig config;
bool configLoaded = false;
unsigned long lastPeriodic;
unsigned long nextIntervalMs = 60000;
FS* filesystem = &LittleFS;

void setNextInterval() {
  long jitter = random(-5000, 5000);
  nextIntervalMs = 20000 + jitter;
  lastPeriodic = millis();
}

void sendInfo(const char* msg) {
  StaticJsonDocument<256> doc;
  doc["eventType"] = "info";
  doc["clusterId"] = configLoaded ? config.clusterId : 0;
  doc["message"] = msg;
  serializeJson(doc, Serial);
  Serial.println();
}

bool loadConfigFromFlash() {
  if (!filesystem->begin()) {
    sendInfo("LittleFS mount failed");
    return false;
  }
  File f = filesystem->open("/config.bin", "r");
  if (!f) {
    sendInfo("config not found");
    return false;
  }
  f.read((uint8_t*)&config, sizeof(config));
  f.close();
  if (config.magic != CONFIG_MAGIC) {
    sendInfo("config not found");
    return false;
  }
  sendInfo("config found");
  return true;
}

void saveConfigToFlash() {
  config.magic = CONFIG_MAGIC;  // Mark data as valid
  File f = filesystem->open("/config.bin", "w");
  if (!f) return;
  f.write((const uint8_t*)&config, sizeof(config));
  f.flush();
  f.close();
}

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

void parseConfig(const char* json) {
  StaticJsonDocument<1024> doc;
  DeserializationError err = deserializeJson(doc, json);
  if (err) return;

  config.clusterId = doc["clusterId"];
  JsonArray nodes = doc["nodes"];
  config.sensorCount = min((int)nodes.size(), MAX_SENSORS);
  for (int i = 0; i < config.sensorCount; i++) {
    config.sensors[i].nodeId = nodes[i]["nodeId"];
    config.sensors[i].gpio = nodes[i]["touchSensorGPIOPin"];
    config.sensors[i].lastState = false;
    pinMode(config.sensors[i].gpio, INPUT_PULLDOWN);
  }

  saveConfigToFlash();
  configLoaded = true;
  setNextInterval();
  sendInfo("config received");
  sendPeriodicStatus();
    sendInfo(configLoaded ? "configLoaded is TRUE" : "configLoaded is FALSE");
}

void handleIncomingSerial() {
  static char buf[1024];
  static int idx = 0;

  while (Serial.available()) {
    char ch = Serial.read();
    if ((ch == '\n' || ch == '\r') || idx >= 1023) {
      buf[idx] = 0;
      //sendInfo(buf);
      parseConfig(buf);
      sendInfo("input received");  // Short, static message
      idx = 0;
    } else {
      buf[idx++] = ch;
    }
  }
}

void setup() {
  sendInfo("setup() starting");
  Serial.begin(115200);
  delay(500);
  sendInfo("initializing");

  if (loadConfigFromFlash()) {
    configLoaded = true;
    setNextInterval();
    sendPeriodicStatus();
  }
}

void loop() {
  /*
  static unsigned long lastDebug = 0;

  if (millis() - lastDebug > 1000) {
    lastDebug = millis();
    bool pinState = digitalRead(4);
    sendInfo(pinState ? "GPIO4 HIGH" : "GPIO4 LOW");
  }
*/
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
// sample payload to send via serial: {"clusterId":1,"nodes":[{"nodeId":6,"touchSensorGPIOPin":4}]}\n

//{"clusterId":1,"nodes":[{"nodeId":7,"touchSensorGPIOPin":4},{"nodeId":8,"touchSensorGPIOPin":5},{"nodeId":9,"touchSensorGPIOPin":6},{"nodeId":10,"touchSensorGPIOPin":7},{"nodeId":11,"touchSensorGPIOPin":8},{"nodeId":12,"touchSensorGPIOPin":9},{"nodeId":13,"touchSensorGPIOPin":10},{"nodeId":14,"touchSensorGPIOPin":11},{"nodeId":15,"touchSensorGPIOPin":12},{"nodeId":16,"touchSensorGPIOPin":13}]}
