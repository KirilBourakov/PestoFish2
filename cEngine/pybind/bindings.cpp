//
// Created by Kiril on 2025-08-23.
//
#include <pybind11/pybind11.h>
import Engine;
namespace py = pybind11;

PYBIND11_MODULE(cEngine, m) {
    py::class_<Engine>(m, "Engine")
        .def(py::init<>())
    .def("add", &Engine::add);
}