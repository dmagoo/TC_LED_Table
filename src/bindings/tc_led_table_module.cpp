#include <pybind11/pybind11.h>
#include <pybind11/stl.h> 

#include "LedTableApi.h"
#include "config/make_cluster_config.h"
#include "config/make_mqtt_config.h"
#include "core/ClusterManager.h"
#include "core/ClusterMessageManager.h"

namespace py = pybind11;

// Assuming these functions are defined in their respective .cpp files
void init_coordinate_bindings(py::module_ &);
void init_led_table_api_bindings(py::module_ &);

PYBIND11_MODULE(tc_led_table, m) {
    init_coordinate_bindings(m);  // Initialize coordinate bindings
    init_led_table_api_bindings(m);  // Initialize LedTableApi bindings

    // Assuming getApiInstance is defined and returns a pointer to LedTableApi instance
    m.def("getApiInstance", [](const std::map<std::string, std::map<std::string, std::string>>& config) {
        auto host = config.at("mqtt_broker").at("host");

        // Create the MQTT client configuration
        // auto mqttClient = makeMQTTClientConfig("LedTableController", host);
        auto mqttClient = makeMQTTClientConfig("LedTableController");

        ClusterMessageManager clusterMessageManager(mqttClient.get());
        ClusterManager clusterManager(makeClusterConfigs());
        static LedTableApi api(clusterManager, &clusterMessageManager);

        // Return a pointer to the static LedTableApi instance
        return &api;


    }, "Get or create an API instance with the specified configuration.");
}
