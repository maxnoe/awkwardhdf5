#include <pybind11/pybind11.h>

namespace py = pybind11;


// see inheritance diagram at
// https://support.hdfgroup.org/HDF5/doc/cpplus_RM/class_h5_1_1_id_component.html

void init_dataset(py::module_ &);
void init_file(py::module_ &);
void init_group(py::module_ &);



PYBIND11_MODULE(awkwardhdf5, m) {
    init_dataset(m);
    init_group(m);
    init_file(m);
}

