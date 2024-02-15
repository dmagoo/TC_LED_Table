#include "SensorTransmitter.h"
#include "SensorMessage.h"

SensorTransmitter::SensorTransmitter(const LedTableConfig &config) {
    if (config.enableMQTTSubscriptions) {
        mqttClient = createMQTTClient(config.mqttConfig);
        mqttConnOpts.set_keep_alive_interval(20);
        mqttConnOpts.set_clean_session(true);

        connect();
    }
}

void SensorTransmitter::connect() {
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
    } catch (const mqtt::exception &exc) {
        std::cerr << "Error connecting to MQTT broker: " << exc.what() << std::endl;
    } catch (std::exception &exc) {
        std::cerr << "Error connecting to MQTT broker: " << exc.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown error occurred while connecting to MQTT broker." << std::endl;
    }
}

bool SensorTransmitter::sendTouchSensorEvent(int nodeId, int value, bool touched) {

    if (mqttClient) {
        TouchEventMessage message;
        message.setSensorData(1, 100, true);
        std::vector<uint8_t> serialized = message.serialize();
        std::string payload(serialized.begin(), serialized.end());

        if (!connected) {
            connect();
        }
        mqttClient->publish(touch_sensor_topic, payload.data(), payload.size(), 0, false)->wait();
        return true;
    }
    return false;
}
