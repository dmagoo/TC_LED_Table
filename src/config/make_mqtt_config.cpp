#include "make_mqtt_config.h"
#include <iostream>

// Function to create and configure an MQTT client
std::unique_ptr<mqtt::async_client> createMQTTClient(const MQTTConfig& config) {
    auto client = std::make_unique<mqtt::async_client>(config.brokerAddress, config.clientId);
    std::cout << "MQTT Client Created: " << config.clientId << std::endl;
    // Additional client configuration can be done here if needed
    return client;
}

MQTTConfig::MQTTConfig(const std::string& broker, const std::string& id)
    : brokerAddress(broker), clientId(id) {
    // Constructor implementation (can be empty if there's nothing to initialize besides the member initializer list)
}
