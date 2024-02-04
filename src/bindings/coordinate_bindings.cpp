#include <pybind11/pybind11.h>
#include "core/coordinates/Cartesian2dCoordinate.h"
#include "core/coordinates/CubeCoordinate.h"
#include "core/coordinates/RingCoordinate.h"

namespace py = pybind11;

void init_coordinate_bindings(py::module_ &m) {
    py::class_<Cartesian2dCoordinate>(m, "Cartesian2dCoordinate")
        .def(py::init<float, float>())
        .def("getX", &Cartesian2dCoordinate::getX)
        .def("getY", &Cartesian2dCoordinate::getY);

    py::class_<CubeCoordinate>(m, "CubeCoordinate")
        .def(py::init<int, int, int>())
        .def("getQ", &CubeCoordinate::getQ)
        .def("getR", &CubeCoordinate::getR)
        .def("getS", &CubeCoordinate::getS);

    py::class_<RingCoordinate>(m, "RingCoordinate")
        .def(py::init<int, int>())
        .def("getRing", &RingCoordinate::getRing)
        .def("getIndex", &RingCoordinate::getIndex);
}
