#ifndef MAKE_MQTT_CONFIG_H
#define MAKE_MQTT_CONFIG_H

#include "mqtt/async_client.h"
#include <memory>
#include <string>

// Struct to hold MQTT configuration parameters
struct MQTTConfig {
    std::string brokerAddress;
    std::string clientId;

    MQTTConfig(const std::string& broker = "tcp://localhost", const std::string& id = "LEDTableController");
};

// Function to create and configure an MQTT client
std::unique_ptr<mqtt::async_client> createMQTTClient(const MQTTConfig& config);

#endif // MAKE_MQTT_CONFIG_H
