#include "make_mqtt_config.h"

std::unique_ptr<mqtt::async_client> makeMQTTClientConfig() {
   auto client = std::make_unique<mqtt::async_client>("tcp://192.168.1.49", "LEDTableController");
    try {
        // Try to connect the client
         mqtt::connect_options connOpts;
        connOpts.set_keep_alive_interval(20);
        connOpts.set_clean_session(true);

        client->connect(connOpts)->wait();
        std::cout << "Connected to MQTT broker." << std::endl;
    } catch (const mqtt::exception& exc) {
        std::cerr << "Error connecting to MQTT broker: " << exc.what() << std::endl;
        // Handle the connection failure (e.g., by returning a nullptr or rethrowing the exception)
        // For this example, we'll return a nullptr to indicate the connection failure.
        return nullptr;
    }

    // Return the connected client
    return client;
 }

