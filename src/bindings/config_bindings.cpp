#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "config/led_table_config.h"
#include "config/make_artnet_config.h"
#include "config/make_mqtt_config.h"
namespace py = pybind11;

void init_mqtt_config_bindings(py::module_ &m) {
    py::class_<MQTTConfig>(m, "MQTTConfig")
        .def(py::init<const std::string &, const std::string &>(),
             py::arg("broker") = "tcp://localhost", py::arg("id") = "LEDTableController")
        .def_readwrite("brokerAddress", &MQTTConfig::brokerAddress)
        .def_readwrite("clientId", &MQTTConfig::clientId);
}

void init_artnet_config_bindings(py::module_ &m) {
    py::class_<ArtnetConfig>(m, "ArtnetConfig")
        .def(py::init<const std::string &, const std::string &>(),
             py::arg("broker") = "tcp://localhost", py::arg("id") = "LEDTableController")
        .def_readwrite("brokerAddress", &ArtnetConfig::brokerAddress)
        .def_readwrite("clientId", &ArtnetConfig::clientId);
}

void init_led_table_config_bindings(py::module_ &m) {
    init_mqtt_config_bindings(m);
    init_artnet_config_bindings(m);
    py::class_<LedTableConfig>(m, "LedTableConfig")
        .def(py::init<>())
        .def_readwrite("mqttConfig", &LedTableConfig::mqttConfig)
        .def_readwrite("artnetConfig", &LedTableConfig::artnetConfig)
        .def_readwrite("enableMQTTMessaging", &LedTableConfig::enableMQTTMessaging)
        .def_readwrite("enableArtnetMessaging", &LedTableConfig::enableArtnetMessaging);
}
