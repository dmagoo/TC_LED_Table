// ClusterMessageManager.h
#ifndef CLUSTERMESSAGEMANAGER_H
#define CLUSTERMESSAGEMANAGER_H
#include "mqtt/async_client.h"

#include "ClusterCommands.h"
#include "ClusterMessage.h"

class ClusterMessageManager {
private:
    int lastSequenceNumber = 0;
    bool connected = false;
    mqtt::async_client *mqttClient;

    void connect();

public:
    ClusterMessageManager(mqtt::async_client *client) : mqttClient(client){};
    template <typename CommandType>
    void sendClusterCommand(int clusterId, const CommandType &command);
};

template <typename CommandType>
void ClusterMessageManager::sendClusterCommand(int clusterId, const CommandType &command) {

    //    FillNodeCommand fillNodeCommand(10, 0x00112233);
    ClusterMessage clusterMessage(clusterId, command.getType(), command.getParams());

    // todo: this all goes inside the clusterMessage manager
    std::vector<uint8_t> serialized = serializeClusterMessage(clusterMessage);
    std::cout << "making a message!! WEEEE" << std::endl;

    // Convert std::vector<uint8_t> to std::string for the MQTT message payload
    std::string payload(serialized.begin(), serialized.end());

    //    if(!connected) {
    // cannot get connect workign in here?? WTF
    //  }

    mqttClient->publish("my_topic", payload.data(), payload.size(), 0, false)->wait();

    // mqttClient->publish("/foo", payload, payloadlen, 0, false)->wait();
    //  mqttClient.publish(topic, message.c_str(), message.length(), 0, false)->wait();
}

#endif // CLUSTERMESSAGEMANAGER_H
