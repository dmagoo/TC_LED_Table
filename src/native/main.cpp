#include "mqtt/async_client.h"
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    // Check arguments
    if (argc != 4) {
        std::cerr << "Usage: mqtt_test <broker> <topic> <message>\n";
        return 1;
    }

    std::string broker = argv[1];
    std::string topic = argv[2];
    std::string message = argv[3];

    // Paho MQTT C++ client creation
    mqtt::async_client client(broker, "LEDTableTestClient");

    // Connect to the MQTT broker
    mqtt::connect_options connOpts;
    connOpts.set_keep_alive_interval(20);
    connOpts.set_clean_session(true);

    try {
        std::cout << "Connecting to broker: " << broker << std::endl;
        client.connect(connOpts)->wait();
        std::cout << "Connected\n";

        // Publish the message
        std::cout << "Publishing message: " << message << " to topic: " << topic << std::endl;
        client.publish(topic, message.c_str(), message.length(), 0, false)->wait();
        std::cout << "Message published\n";

        // Disconnect
        client.disconnect()->wait();
        std::cout << "Disconnected\n";
    } catch (const mqtt::exception& exc) {
        std::cerr << "Error: " << exc.what() << std::endl;
        return 1;
    }

    return 0;
}


/*
#include "CLI/CLI.hpp"
#include "config/make_cluster_config.h"
#include "core/Cluster.h"
#include "core/LedTableTypes.h"
#include "core/Node.h"
#include "core/NodeConfig.h"
#include "mqtt/async_client.h"
#include <iostream>
#include <vector>
#define PIXELS_PER_NODE 8

// Command handlers
void handleConnect(const std::string &broker) {
    std::cout << "Connecting to broker: " << broker << std::endl;

    // Paho MQTT C++ client creation
    mqtt::async_client client(broker, "LEDTableClient");

    // Create an MQTT connection options object
    mqtt::connect_options connOpts;
    connOpts.set_keep_alive_interval(20);
    connOpts.set_clean_session(true);

    try {
        // Connect to the MQTT broker
        auto token = client.connect(connOpts);
        token->wait();
        std::cout << "Connected to broker: " << broker << std::endl;
    } catch (const mqtt::exception &exc) {
        std::cerr << "Error connecting to broker: " << exc.what() << std::endl;
    }
}

void handleSetColor(int nodeId, const std::string &color) {
    std::cout << "Setting color of node " << nodeId << " to " << color << std::endl;

    // Example topic and payload. Adjust as needed for your project
    std::string topic = "ledtable/node/" + std::to_string(nodeId) + "/color";
    std::string payload = color;

    try {
        // Paho MQTT C++ client creation
        mqtt::async_client client("tcp://your_broker:1883", "LEDTableClient");

        // Connect (reuse the handleConnect logic or ensure the client is connected)
        mqtt::connect_options connOpts;
        connOpts.set_keep_alive_interval(20);
        connOpts.set_clean_session(true);
        client.connect(connOpts)->wait();

        // Publish the color change
        client.publish(topic, payload.c_str(), payload.length(), 0, false)->wait();
        std::cout << "Color change published" << std::endl;

        // Disconnect
        client.disconnect()->wait();
    } catch (const mqtt::exception &exc) {
        std::cerr << "Error setting color: " << exc.what() << std::endl;
    }
}

int main(int argc, char **argv) {
    CLI::App app{"LED Table Control CLI"};
    CLI11_PARSE(app, argc, argv);

    std::vector<NodeConfig> nodeConfigs = makeNodeConfigs(0);
    Cluster cluster(0, nodeConfigs);

    const Node *node7 = cluster.getNode(6);

    std::cout << node7->getId() << " : NODE! " << std::endl;

    // MQTT Broker command
    std::string broker = "tcp://your_broker:1883";
    mqtt::async_client client(broker, "LEDTableClient");

    std::cout << node7->getId() << " broker " << broker << std::endl;

    auto connectCmd = app.add_subcommand("connect", "Connect to MQTT broker");
    connectCmd->add_option("-b,--broker", broker, "MQTT Broker Address")->required();
    connectCmd->callback([&broker] { handleConnect(broker); });

    // Set Color command
    int nodeId;
    std::string color;
    auto setColorCmd = app.add_subcommand("set-color", "Set color of a node");
    setColorCmd->add_option("-n,--node", nodeId, "Node ID")->required();
    setColorCmd->add_option("-c,--color", color, "Color")->required();
    setColorCmd->callback([&] { handleSetColor(nodeId, color); });

    return 0;
}
*/