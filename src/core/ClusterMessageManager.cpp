#include "ClusterMessageManager.h"

ClusterMessageManager::ClusterMessageManager(const LedTableConfig &config) {
    if (config.enableMQTTMessaging) {
        mqttClient = createMQTTClient(config.mqttConfig);
        mqttConnOpts.set_keep_alive_interval(20);
        mqttConnOpts.set_clean_session(true);
    }
}

void ClusterMessageManager::connectMessagingClient() {
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
        // std::cerr << "Error connecting to MQTT broker: " << exc.what() << std::endl;
        std::cerr << "Error connecting to MQTT broker: " << exc.what() << std::endl;
    } catch (std::exception &exc) {
        std::cerr << "Error connecting to MQTT broker: " << exc.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown error occurred while connecting to MQTT broker." << std::endl;
    }
}
