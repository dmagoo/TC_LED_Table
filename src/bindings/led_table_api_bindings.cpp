
#include <pybind11/pybind11.h>
#include <pybind11/stl.h> // If you're using STL containers in your API
// #include "LedTableApi.h" // Include your LedTableApi header

#include "api/LedTableApi.h"
#include "config/make_cluster_config.h"
#include "config/make_mqtt_config.h"
#include "core/Cluster.h"
#include "core/ClusterCommands.h"
#include "core/ClusterManager.h"
#include "core/ClusterMessage.h"
#include "core/ClusterMessageManager.h"
#include "core/LedTableTypes.h"

#include <vector>

namespace py = pybind11;
// Global API object

LedTableApi* getLazyApi() {
    static auto mqttClient = makeMQTTClientConfig("LedTableController");
    static ClusterMessageManager clusterMessageManager(mqttClient.get());
    static ClusterManager clusterManager(makeClusterConfigs());
    static LedTableApi api(clusterManager, &clusterMessageManager);
    return &api;
}


// LedTableApi api;
PYBIND11_MODULE(ledtableapi_py, m) {
    m.doc() = "Python bindings for LED Table API"; // Optional module docstring

/*
    // Initialize the API object (could also be done lazily upon first function call)
    m.def("init_api", []() {
     static auto mqttClient = makeMQTTClientConfig("LedTableController");
    static ClusterMessageManager clusterMessageManager(mqttClient.get());
    static ClusterManager clusterManager(makeClusterConfig(0));
    static LedTableApi api(clusterManager, &clusterMessageManager);
    });
*/
    m.def("setSuppressMessages", [](bool suppress) {
        LedTableApi* api = getLazyApi();
        api->setSuppressMessages(suppress);
    });

    m.def("refresh", []() {
        LedTableApi* api = getLazyApi();
        api->refresh();
    });

    m.def("test", [](int nodeId) {
        // std::vector<WRGB> colors = {0x00000000, 0x00000055, 0x00005500, 0x00550000, 0x00555500, 0x00550055, 0x00005555};
        LedTableApi* api = getLazyApi();
        api->fillNode(nodeId, 0x00005555);
    });


}


/*


#include <pybind11/pybind11.h>
#include <pybind11/stl.h> // If you're using STL containers in your API
#include "LedTableApi.h" // Include your LedTableApi header

namespace py = pybind11;

PYBIND11_MODULE(led_table_api, m) {
    // Define the module name, in this case 'led_table_api'
    m.doc() = "Python bindings for the LedTableApi class in C++";

    // Bind the LedTableApi class
    py::class_<LedTableApi>(m, "LedTableApi")
        .def(py::init<ClusterManager&, ClusterMessageManager*>()) // Constructor binding
        .def("setSuppressMessages", &LedTableApi::setSuppressMessages)
        .def("fillNode", (void (LedTableApi::*)(int, WRGB)) &LedTableApi::fillNode, "nodeId"_a, "color"_a)
        // Repeat .def for other overloads of fillNode
        .def("setNodePixel", (void (LedTableApi::*)(int, int, WRGB)) &LedTableApi::setNodePixel, "nodeId"_a, "pixelIndex"_a, "color"_a)
        // Repeat for other methods you wish to expose
        .def("reset", &LedTableApi::reset);

    // If using custom types like WRGB or coordinate classes, you need to bind those as well
    py::class_<WRGB>(m, "WRGB")
        .def(py::init<>()) // Default constructor
        .def(py::init<uint8_t, uint8_t, uint8_t, uint8_t>(), "w"_a, "r"_a, "g"_a, "b"_a); // Parameterized constructor

    // Repeat for other custom types like RingCoordinate, Cartesian2dCoordinate, CubeCoordinate
}
*/