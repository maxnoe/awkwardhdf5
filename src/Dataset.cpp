#include <H5Cpp.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <vector>


namespace py = pybind11;

const hsize_t get_ndim(const H5::DataSet& self) {
    return self.getSpace().getSimpleExtentNdims(); 
}

const std::vector<hsize_t> get_shape(const H5::DataSet& self) {
    hsize_t ndim = get_ndim(self);
    std::vector<hsize_t> shape(ndim);
    self.getSpace().getSimpleExtentDims(&shape.front());

    return shape;
}


void init_dataset(py::module_& m) {
    py::class_<H5::DataSet, H5::H5Object> DataSet(m, "DataSet");
    DataSet.def(py::init<>());
    DataSet.def_property_readonly("ndim", &get_ndim);
    DataSet.def_property_readonly("shape", [](const H5::DataSet& self) {
        auto shape = get_shape(self);
        py::tuple shape_tuple = py::cast(shape);
        return shape_tuple;
    });
}
