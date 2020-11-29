#include <H5Cpp.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "Object.h"

namespace py = pybind11;


void init_object(py::module_& m) {
    py::class_<H5::H5Object, std::unique_ptr<H5::H5Object, py::nodelete>>(m, "H5Object");
    py::class_<PyH5Object, H5::H5Object> Object(m, "Object");
}
