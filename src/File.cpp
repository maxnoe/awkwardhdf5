#include <unordered_map>
#include <H5Cpp.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

std::unordered_map<std::string, unsigned int> FILE_MODES = {
    {"r", H5F_ACC_RDONLY},
    {"w", H5F_ACC_TRUNC},
    {"x", H5F_ACC_EXCL},
    {"a", H5F_ACC_RDWR},
    {"r+", H5F_ACC_RDWR},
};

std::unique_ptr<H5::H5File> file_init(const std::string& path, const std::string& mode = "r") {
    if (FILE_MODES.find(mode) == FILE_MODES.end()) {
        throw std::invalid_argument("Unknown mode: '" + mode + "'.");
    }
    return std::make_unique<H5::H5File>(path, FILE_MODES[mode]);
}


void init_file(py::module_& m) {
    py::class_<H5::H5File, H5::Group>(m, "File")
        .def(py::init(&file_init), py::arg("path"), py::arg("mode") = "r")
        ;
}
