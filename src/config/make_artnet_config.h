#ifndef MAKE_ARTNET_CONFIG_H
#define MAKE_ARTNET_CONFIG_H
#include <string>

// Struct to hold Artnet configuration parameters
struct ArtnetConfig {
    std::string brokerAddress;
    std::string clientId;

    ArtnetConfig(const std::string& broker = "tcp://localhost", const std::string& id = "LEDTableController");
};
#endif
