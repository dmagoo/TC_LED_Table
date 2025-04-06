#include "api/LedTableApi.h"
#include "config/led_table_config.h"
#include "config/make_cluster_config.h"
#include "config/make_mqtt_config.h"
#include "core/Cluster.h"
#include "core/ClusterCommands.h"
#include "core/ClusterManager.h"
#include "core/ClusterMessage.h"
#include "core/ClusterMessageManager.h"
#include "core/LedTableTypes.h"
#ifdef WIN64
#include "windows/AsciiUtils.h"
#include <windows.h>
#else
#include <unistd.h> // For sleep function
#endif

#include "mqtt/async_client.h"
#include <iostream>
#include <signal.h>
#include <string>
#include <vector>

// #include <string>

// max? #define DELAY_MS 75
#define DELAY_MS 500

#define NUM_NODES 7

bool USE_REFRESH = true;
bool SHOW_ASCII = false;
// Global flag to control loop termination
volatile sig_atomic_t keepRunning = 1;

// Signal handler to catch CTRL+C
void intHandler(int dummy) {
    keepRunning = 0;
}

class simple_callback : public virtual mqtt::callback {
public:
    void message_arrived(mqtt::const_message_ptr msg) override {
        // std::cout << "Message arrived: " << msg->to_string() << std::endl;
        // std::cout << "\ttopic: '" << msg->get_topic() << "'" << std::endl;
        std::string payload_str = msg->to_string(); // Get the payload as a string

        // Convert std::string to std::vector<uint8_t>
        std::vector<uint8_t> buffer(payload_str.begin(), payload_str.end());

        ClusterMessage deserialized = deserializeClusterMessage(buffer);

        std::cout << "Received message bound for cluster: " << deserialized.getClusterId() << std::endl;

        // CommandParamsVariant paramsVariant = deserialized.getParams();
        auto params = deserialized.getParams();
        if (std::holds_alternative<NodeWithColorParams>(params)) {
            int nodeId = std::get<NodeWithColorParams>(params).nodeId;
            std::cout << "Received message to color node: " << nodeId << std::endl;
            ;

            // Use nodeId as needed
        } else {
            std::cout << "other type" << std::endl;
        }
    }

    void connection_lost(const std::string &cause) override {
        std::cerr << "\nConnection lost" << std::endl;
        if (!cause.empty())
            std::cerr << "\tcause: " << cause << std::endl;
    }
};

void runReceiver() {
    signal(SIGINT, intHandler); // Register the signal handler

    LedTableConfig config;
    config.mqttConfig.brokerAddress = "tcp://192.168.1.50";

    auto mqttClient = createMQTTClient(config.mqttConfig); // This returns a std::unique_ptr<mqtt::async_client>

    // auto mqttClient = makeMQTTClientConfig("LedTableMonitor");
    mqttClient->subscribe("my_topic", 1)->wait();

    simple_callback cb{};
    mqttClient->set_callback(cb);
    std::cout << " - removing  - " << std::endl;

    while (keepRunning) {

#ifdef WIN64
        Sleep(DELAY_MS); // Delay
#else
        usleep(DELAY_MS * 1000); // Delay
#endif
    }
    std::cout << "disconnecting" << std::endl;
    mqttClient->disconnect();
}

void runSender() {
    signal(SIGINT, intHandler); // Register the signal handler
    // ClusterManager clusterManager(makeClusterConfig(0));
    ClusterManager clusterManager(makeClusterConfigs());

    // In the context of using this configuration
    LedTableConfig config;
    config.mqttConfig.brokerAddress = "tcp://192.168.1.50";
    config.mqttConfig.clientId = "apiTest";
    config.enableMQTTMessaging = false;
    config.enableArtnetMessaging = true;
    config.enableMQTTSubscriptions = true;

    LedTableApi api(clusterManager, config);
    api.setSuppressMessages(USE_REFRESH);

    const Cluster *cluster = clusterManager.getClusterById(0);
    // Define the color list
    // std::vector<WRGB> colors = {0x00000000, 0x33000000, 0x33003300, 0x33330000, 0x33333300, 0x33330000, 0x33003300};
    // std::vector<WRGB> colors = {0x33000000, 0x00000000, 0x00330000, 0x00000000, 0x00003300, 0x00000000, 0x00000000};
    // std::vector<WRGB> colors = {0x00330000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
    std::vector<WRGB> colors = {0x00000000, 0x00000055, 0x00005500, 0x00550000, 0x00555500, 0x00550055, 0x00005555};
    // track the fill from each node so we can move colors to the new node.
    // could do this by getting the buffer, but this is just a cheap demo!
    std::vector<WRGB> nodeColors(NUM_NODES, 0x00000000); // Initialize all nodes to black

    int currentColorIndex = 0;
    int i = 0;
    while (keepRunning) {

        if (USE_REFRESH) {
            api.refresh();
        }

        // Shift colors across nodes
        for (int nodeId = NUM_NODES - 1; nodeId > 0; --nodeId) {
            nodeColors[nodeId] = nodeColors[nodeId - 1];
            api.fillNode(nodeId, nodeColors[nodeId]);
        }

        // Update node 0 with the new color and increment the color index
        nodeColors[0] = colors[currentColorIndex];
        api.fillNode(0, nodeColors[0]);
        currentColorIndex = (currentColorIndex + 1) % colors.size();

#ifdef WIN64
        if (SHOW_ASCII) {
            for (int nodeId = 0; nodeId < NUM_NODES; nodeId++) {
                std::string asciiArt = nodeBufferToAscii(*cluster, nodeId);
                std::cout << asciiArt;
            }
        }
        Sleep(DELAY_MS); // Delay
#else
        if (SHOW_ASCII) {
            //  posix:
            std::ostringstream asciiArt;
            for (int nodeId = 0; nodeId < NUM_NODES; nodeId++) {
                auto nodeBuffer = cluster->getNodePixelBuffer(nodeId);
                asciiArt << std::setw(3) << (nodeBuffer[7] ? 'x' : 'o') << " ";
                asciiArt << std::setw(3) << (nodeBuffer[0] ? 'x' : 'o') << " ";
                asciiArt << std::setw(3) << (nodeBuffer[1] ? 'x' : 'o') << "\n";

                // Middle row (West, Node ID, East)
                asciiArt << std::setw(3) << (nodeBuffer[6] ? 'x' : 'o') << " ";
                asciiArt << std::setw(3) << nodeId << " ";
                asciiArt << std::setw(3) << (nodeBuffer[2] ? 'x' : 'o') << "\n";

                // Bottom row (South-West, South, South-East)
                asciiArt << std::setw(3) << (nodeBuffer[5] ? 'x' : 'o') << " ";
                asciiArt << std::setw(3) << (nodeBuffer[4] ? 'x' : 'o') << " ";
                asciiArt << std::setw(3) << (nodeBuffer[3] ? 'x' : 'o') << "\n";

                std::cout << asciiArt.str();
            }
        }

        usleep(DELAY_MS * 1000); // Delay
#endif
    }

    api.setSuppressMessages(false);

    // this doesn't seem to be sending messages when suppressMessages is false
    // it should!
    api.reset();

    // this works
    //    api.refresh();
    //    mqttClient->disconnect();
}

int main(int argc, char *argv[]) {
    // Parse command-line arguments
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--receiver") {
            runReceiver();
            return 0; // Exit after receiver runs
        }
    }
    runSender();
    return 0;
}
