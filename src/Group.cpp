#include <H5Cpp.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

class DataSet;


std::vector<std::string> get_keys(const H5::Group& group) {
    hsize_t n_objects = group.getNumObjs();
    std::vector<std::string> keys;
    keys.reserve(n_objects);

    for (hsize_t idx = 0; idx < n_objects; idx++) {
        keys.push_back(group.getObjnameByIdx(idx));
    }

    return keys;
}


py::object group_getitem(const H5::Group& self, const std::string& key) {
    H5O_type_t type = self.childObjType(key);

    switch (type) {
        case H5O_TYPE_GROUP:
            return py::cast(std::make_unique<H5::Group>(self.openGroup(key)));
        case H5O_TYPE_DATASET:
            return py::cast(std::make_unique<H5::DataSet>(self.openDataSet(key)));
        default:
            throw std::invalid_argument("Unsupported member");
    }
}

void init_group(py::module_& m) {
    py::class_<H5::Group> Group(m, "Group");
    Group.def(py::init<>());
    Group.def("keys", &get_keys);
    Group.def("__getitem__", &group_getitem);
}
