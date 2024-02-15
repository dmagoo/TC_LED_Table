#pragma once

#include "SensorMessage.h"
#include "config/led_table_config.h"
#include "mqtt/async_client.h"

#include <memory> // For std::unique_ptr

class SensorTransmitter {
public:
    explicit SensorTransmitter(const LedTableConfig &config);
    bool sendTouchSensorEvent(int nodeId, int value, bool touched);

private:
    bool connected = false;
    std::unique_ptr<mqtt::async_client> mqttClient;
    mqtt::connect_options mqttConnOpts;
    void connect();
    const std::string touch_sensor_topic = "ledtable/sensor/touch";
};
