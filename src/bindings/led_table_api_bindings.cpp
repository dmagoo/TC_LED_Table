#include <pybind11/pybind11.h>
#include <pybind11/stl.h>  // For STL containers bindings
#include "LedTableApi.h"  // Adjust the path as necessary to where LedTableApi is defined
#include <iostream>
namespace py = pybind11;

void init_led_table_api_bindings(py::module_ &m) {
    py::class_<LedTableApi>(m, "LedTableApi")
        .def("setSuppressMessages", &LedTableApi::setSuppressMessages)

        // fillNode methods
        .def("fillNode", [](LedTableApi &self, int nodeId, const WRGB &color) {
            self.fillNode(nodeId, color);
        })
        .def("fillNode", [](LedTableApi &self, const RingCoordinate &coordinate, const WRGB &color) {
            self.fillNode(coordinate, color);
        })
        .def("fillNode", [](LedTableApi &self, const Cartesian2dCoordinate &coordinate, const WRGB &color) {
            self.fillNode(coordinate, color);
        })
        .def("fillNode", [](LedTableApi &self, const CubeCoordinate &coordinate, const WRGB &color) {
            self.fillNode(coordinate, color);
        })

        // Overloaded fillNode methods with color arrays and pad color
        .def("fillNode", [](LedTableApi &self, int nodeId, const std::vector<WRGB> &colors, WRGB padcolor) {
            self.fillNode(nodeId, colors, padcolor);
        })
        .def("fillNode", [](LedTableApi &self, const RingCoordinate &coordinate, const std::vector<WRGB> &colors, WRGB padcolor) {
std::cout << "------------------------------------------------" << std::endl;
            self.fillNode(coordinate, colors, padcolor);
        })
        .def("fillNode", [](LedTableApi &self, const Cartesian2dCoordinate &coordinate, const std::vector<WRGB> &colors, WRGB padcolor) {
            self.fillNode(coordinate, colors, padcolor);
        })
        .def("fillNode", [](LedTableApi &self, const CubeCoordinate &coordinate, const std::vector<WRGB> &colors, WRGB padcolor) {
            self.fillNode(coordinate, colors, padcolor);
        })

        // setNodePixel methods
        .def("setNodePixel", [](LedTableApi &self, int nodeId, int pixelIndex, WRGB color) {
            self.setNodePixel(nodeId, pixelIndex, color);
        })
        .def("setNodePixel", [](LedTableApi &self, const RingCoordinate &coordinate, int pixelIndex, WRGB color) {
            self.setNodePixel(coordinate, pixelIndex, color);
        })
        .def("setNodePixel", [](LedTableApi &self, const Cartesian2dCoordinate &coordinate, int pixelIndex, WRGB color) {
            self.setNodePixel(coordinate, pixelIndex, color);
        })
        .def("setNodePixel", [](LedTableApi &self, const CubeCoordinate &coordinate, int pixelIndex, WRGB color) {
            self.setNodePixel(coordinate, pixelIndex, color);
        })

        // queueNodePixel methods
        .def("queueNodePixel", [](LedTableApi &self, int nodeId, WRGB color) -> WRGB {
            return self.queueNodePixel(nodeId, color);
        })
        .def("queueNodePixel", [](LedTableApi &self, const RingCoordinate &coordinate, WRGB color) -> WRGB {
            return self.queueNodePixel(coordinate, color);
        })
        .def("queueNodePixel", [](LedTableApi &self, const Cartesian2dCoordinate &coordinate, WRGB color) -> WRGB {
            return self.queueNodePixel(coordinate, color);
        })
        .def("queueNodePixel", [](LedTableApi &self, const CubeCoordinate &coordinate, WRGB color) -> WRGB {
            return self.queueNodePixel(coordinate, color);
        })

        // dequeueNodePixel methods
        .def("dequeueNodePixel", [](LedTableApi &self, int nodeId, WRGB color) -> WRGB {
            return self.dequeueNodePixel(nodeId, color);
        })
        .def("dequeueNodePixel", [](LedTableApi &self, const RingCoordinate &coordinate, WRGB color) -> WRGB {
            return self.dequeueNodePixel(coordinate, color);
        })
        .def("dequeueNodePixel", [](LedTableApi &self, const Cartesian2dCoordinate &coordinate, WRGB color) -> WRGB {
            return self.dequeueNodePixel(coordinate, color);
        })
        .def("dequeueNodePixel", [](LedTableApi &self, const CubeCoordinate &coordinate, WRGB color) -> WRGB {
            return self.dequeueNodePixel(coordinate, color);
        })

        // reset and refresh
        .def("reset", &LedTableApi::reset)
        .def("refresh", &LedTableApi::refresh);
}
