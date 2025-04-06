#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "core/SensorTransmitter.h"
#include "config/led_table_config.h"

namespace py = pybind11;

// Assuming these functions are defined in their respective .cpp files

std::shared_ptr<SensorTransmitter> init(LedTableConfig *config = nullptr) {
    LedTableConfig effectiveConfig;
    if (config != nullptr) {
        effectiveConfig = *config;
    } else {
        // Default configuration
        effectiveConfig.mqttConfig.brokerAddress = "tcp://192.168.1.50";
        effectiveConfig.mqttConfig.clientId = "pythonSensorTransmitter";
        effectiveConfig.enableMQTTMessaging = true;
    }

    static auto senderSharedPtr = std::make_shared<SensorTransmitter>(effectiveConfig);

    return senderSharedPtr;
}

void bootstrap(LedTableConfig *config = nullptr) {
    init(config);
}

PYBIND11_MODULE(tc_sensor_transmitter, m) {
    m.def("init", &bootstrap, py::arg("config") = nullptr, "Function to initialize and get the Sensor Transmitter instance with an optional configuration");
    // fillNode methods

    m.def("sendTouchSensorEvent", [](int nodeId, int value, bool touched) -> bool {
        auto sender = init();
        return sender->sendTouchSensorEvent(nodeId, value, touched);
    });
}
