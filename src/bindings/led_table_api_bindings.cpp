#include <pybind11/pybind11.h>
#include "LedTableApi.h"  // Adjust the path as necessary to where LedTableApi is defined

namespace py = pybind11;

void init_led_table_api_bindings(py::module_ &m) {
    py::class_<LedTableApi>(m, "LedTableApi")
        // Expose the LedTableApi methods
        .def("setSuppressMessages", &LedTableApi::setSuppressMessages)
        // Add bindings for other public methods of LedTableApi
        .def("fillNode", [](LedTableApi &self, int nodeId, const WRGB &color) {
            // Example method binding with a lambda to handle overloading
            self.fillNode(nodeId, color);
        })
        // Continue with other overloads and methods...
        ;
}
