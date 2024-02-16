#include "SensorListener.h"
#include "SensorMessage.h"

SensorListener::SensorListener(const LedTableConfig &config) {
    if (config.enableMQTTSubscriptions) {
        mqttClient = createMQTTClient(config.mqttConfig);
        mqttConnOpts.set_keep_alive_interval(20);
        mqttConnOpts.set_clean_session(true);

        connectSubscriptionClient();
    }
}

void SensorListener::connectSubscriptionClient() {
    connected = false;

    if (!mqttClient) {
        std::cerr << "MQTT client is not initialized." << std::endl;
        return;
    }

    if (mqttClient->is_connected()) {
        std::cout << "Already connected to MQTT broker." << std::endl;
        return;
    }

    try {
        // Try to connect the client
        std::cout << "connecting" << std::endl;
        mqttClient->connect(mqttConnOpts)->wait();
        std::cout << "Connected to MQTT broker." << std::endl;
        connected = true;

        mqttClient->subscribe(touch_sensor_topic, 1)->wait(); // QoS level 1 for this example

        callback = std::make_shared<SensorListenerCallback>(this);
        mqttClient->set_callback(*callback);

    } catch (const mqtt::exception &exc) {
        // std::cerr << "Error connecting to MQTT broker: " << exc.what() << std::endl;
        std::cerr << "Error connecting to MQTT broker: " << exc.what() << std::endl;
    } catch (std::exception &exc) {
        std::cerr << "Error connecting to MQTT broker: " << exc.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown error occurred while connecting to MQTT broker." << std::endl;
    }
}

void SensorListener::on_message(const mqtt::message &msg) {
    // Convert the payload to a string (assuming it's text-based for this example)
    std::string payload = msg.to_string();

    std::unique_ptr<SensorMessage> sensorMessage = deserializeSensorMessage(payload);

    const SensorEventDataProto protoPayload = sensorMessage->getPayload();
    if (protoPayload.event_type == SensorEventDataProto_EventType_TOUCH_EVENT) {
        // Trigger the callback with the received SensorData
        if (onTouchSensorDataReceived) {
            onTouchSensorDataReceived(protoPayload.sensor_data.node_id, protoPayload.sensor_data.current_value, protoPayload.sensor_data.touched);
        }
    }
}
