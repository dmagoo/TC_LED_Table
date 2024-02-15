#include <atomic>
#include <chrono>
#include <csignal>
#include <future>
#include <iostream>
#include <thread>

#include "config/led_table_config.h"
#include "config/make_mqtt_config.h"
#include "core/SensorTransmitter.h"

#include "mqtt/async_client.h"

// Global flag to indicate if Ctrl+C was pressed
std::atomic<bool> g_stopRequested(false);

// Signal handler for Ctrl+C
void signalHandler(int signal) {
    if (signal == SIGINT) {
        std::cout << "Ctrl+C detected. Gracefully terminating..." << std::endl;
        g_stopRequested.store(true);
    }
}

void doTest(SensorTransmitter &sender) {
    std::cout << "beep" << std::endl;
    sender.sendTouchSensorEvent(1, 100, true);
    std::cout << "boop" << std::endl;
}

void runSender() {
    std::cout << "SEND TEST" << std::endl;

    // In the context of using this configuration
    LedTableConfig config;
    config.mqttConfig.brokerAddress = "tcp://192.168.1.49";
    config.mqttConfig.clientId = "sensorReporter";
    config.enableMQTTMessaging = false;
    config.enableArtnetMessaging = true;
    config.enableMQTTSubscriptions = true;
    SensorTransmitter sender(config);

    while (!g_stopRequested.load()) {
        // Set up future to check for input without blocking
        auto future = std::async(std::launch::async, [] {
            char input;
            std::cin >> input;
            return input;
        });

        // Check if input is available without blocking for more than 20 milliseconds
        if (future.wait_for(std::chrono::milliseconds(20)) == std::future_status::ready) {
            char input = future.get(); // Get the input character
            if (input == 'a') {
                doTest(sender);
            }
            if (input == 'b') {
                doTest(sender);
                doTest(sender);
            }
        }

        // Sleep for 20 milliseconds to simulate work and prevent tight looping
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
}

int main(int argc, char *argv[]) {
    // Register signal handler for Ctrl+C
    std::signal(SIGINT, signalHandler);

    // Run the sender function which includes the main loop
    runSender();

    return 0;
}
