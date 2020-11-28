#include <H5Cpp.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;


void init_object(py::module_& m) {
    py::class_<H5::H5Object, H5::H5Location, std::unique_ptr<H5::H5Object, py::nodelete>> Object(m, "Object");
}
