#include <H5Cpp.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;


void init_dataset(py::module_& m) {
    py::class_<H5::DataSet, H5::H5Object> DataSet(m, "DataSet");
    DataSet.def(py::init<>());
}
