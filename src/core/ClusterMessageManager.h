// ClusterMessageManager.h
#ifndef CLUSTERMESSAGEMANAGER_H
#define CLUSTERMESSAGEMANAGER_H
#include "ClusterCommands.h"
#include "ClusterMessage.h"
#include "config/led_table_config.h"
#include "mqtt/async_client.h"
#include <sstream>

class ClusterMessageManager {
private:
    int lastSequenceNumber = 0;
    bool connected = false;
    std::unique_ptr<mqtt::async_client> mqttClient;
    mqtt::connect_options mqttConnOpts;
    void connectMessagingClient();

public:
    explicit ClusterMessageManager(const LedTableConfig &config);

    template <typename CommandType>
    void sendClusterCommand(int clusterId, const CommandType &command);
};

template <typename CommandType>
void ClusterMessageManager::sendClusterCommand(int clusterId, const CommandType &command) {
    ClusterMessage clusterMessage(clusterId, command.getType(), command.getParams());
    // todo: this all goes inside the clusterMessage manager?
    std::vector<uint8_t> serialized = serializeClusterMessage(clusterMessage);
    // Convert std::vector<uint8_t> to std::string for the MQTT message payload
    std::string payload(serialized.begin(), serialized.end());
    std::ostringstream topic;
    topic << "ledtable/cluster/" << clusterId << "/command";
    if (!connected) {
        connectMessagingClient();
    }
    mqttClient->publish(topic.str(), payload.data(), payload.size(), 0, false)->wait();
}

#endif // CLUSTERMESSAGEMANAGER_H
