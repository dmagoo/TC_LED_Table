#pragma once

#include "config/led_table_config.h"
#include "mqtt/async_client.h"
#include "mqtt/callback.h"

#include <memory> // For std::unique_ptr

struct SensorData {
    std::string data; // Simplified representation of sensor data
    // Add more fields if necessary for initial testing
};

class SensorListenerCallback;
class SensorListener;

class SensorListener {

    std::shared_ptr<SensorListenerCallback> callback; // Hold the callback object

public:
    using SensorCallback = std::function<void(const SensorData &)>; // Define a callback type
    explicit SensorListener(const LedTableConfig &config);

    void setSensorDataCallback(const SensorCallback &callback) {
        onSensorDataReceived = callback;
    }
    // Callback to handle incoming messages
    void on_message(const mqtt::message &msg);

    // Method to invoke the callback
    void processData(const SensorData &inputData) { // Renamed parameter to avoid conflict
        SensorData testData{"example data"};        // Renamed variable to avoid redefinition
        if (onSensorDataReceived) {
            onSensorDataReceived(testData); // Use the testData variable here
        }
    }

private:
    bool connected = false;
    std::unique_ptr<mqtt::async_client> mqttClient;
    mqtt::connect_options mqttConnOpts;
    void connectSubscriptionClient();
    SensorCallback onSensorDataReceived;

    const std::string touch_sensor_topic = "ledtable/sensor/touch";
};

class SensorListenerCallback : public mqtt::callback {
    SensorListener *listener;

public:
    SensorListenerCallback(SensorListener *listener) : listener(listener) {
        std::cout << "I MADE ONE!!!" << std::endl;
    }

    void message_arrived(mqtt::const_message_ptr msg) override {
        std::cout << "MESSAGE ARRIVED!" << std::endl;
        listener->on_message(*msg);
    }

    // Todo: Implement other necessary callback methods, such as connection_lost and delivery_complete
};

