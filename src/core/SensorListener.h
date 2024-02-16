#pragma once

#include "config/led_table_config.h"
#include "mqtt/async_client.h"
#include "mqtt/callback.h"

#include <memory> // For std::unique_ptr

class SensorListenerCallback;
class SensorListener;

class SensorListener {

    std::shared_ptr<SensorListenerCallback> callback; // Hold the callback object

public:
    using SensorCallback = std::function<void(int nodeId, int sensorValue, bool touched)>; // Define a callback type
    explicit SensorListener(const LedTableConfig &config);

    void setTouchSensorDataCallback(const SensorCallback &callback) {
        onTouchSensorDataReceived = callback;
    }
    // Callback to handle incoming messages
    void on_message(const mqtt::message &msg);

private:
    bool connected = false;
    std::unique_ptr<mqtt::async_client> mqttClient;
    mqtt::connect_options mqttConnOpts;
    void connectSubscriptionClient();
    SensorCallback onTouchSensorDataReceived;

    const std::string touch_sensor_topic = "ledtable/sensor/touch";
};

class SensorListenerCallback : public mqtt::callback {
    SensorListener *listener;

public:
    SensorListenerCallback(SensorListener *listener) : listener(listener) {
        std::cout << "I MADE ONE!!!" << std::endl;
    }

    void message_arrived(mqtt::const_message_ptr msg) override {
        if (!msg) {
            std::cout << "Dead Message" << std::endl;
        } else {
            std::string payload = msg->to_string();
        }

        listener->on_message(*msg);
    }

    // Todo: Implement other necessary callback methods, such as connection_lost and delivery_complete
};
