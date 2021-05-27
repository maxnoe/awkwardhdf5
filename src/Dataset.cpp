#include <H5Cpp.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <vector>
#include <exception>


namespace py = pybind11;

const hsize_t get_ndim(const H5::DataSet& self) {
    return self.getSpace().getSimpleExtentNdims();
}

const std::vector<hsize_t> get_shape(const H5::DataSet& self) {
    hsize_t ndim = get_ndim(self);
    std::vector<hsize_t> shape(ndim);
    self.getSpace().getSimpleExtentDims(shape.data());

    return shape;
}

std::string read_dataset(const H5::DataSet& self) {
    std::string data;
    data.resize(self.getInMemDataSize());

    self.getSpace().selectAll();
    H5::DataType mem_type(H5::PredType::NATIVE_DOUBLE);
    self.read(&data.front(), mem_type);

    return data;
}

H5::DataType get_mem_type(const H5::DataSet& self) {
    H5T_class_t type = self.getTypeClass();

    switch(type) {
        case H5T_INTEGER:  std::cout << "Found integer" << std::endl; break;
        case H5T_FLOAT:    std::cout << "Found float" << std::endl; break;
        case H5T_STRING:   std::cout << "Found string" << std::endl; break;
        case H5T_BITFIELD: std::cout << "Found bitfiled" << std::endl; break;
        case H5T_COMPOUND: std::cout << "Found float" << std::endl; break;
        case H5T_NO_CLASS: std::cout << "Found noclass" << std::endl; break;
        case H5T_TIME:     std::cout << "Found time" << std::endl; break;
        case H5T_OPAQUE:   std::cout << "Found opaque" << std::endl; break;
        case H5T_ENUM:     std::cout << "Found enum" << std::endl; break;
        default:  std::cout << "Unknown class" << std::endl; break;
    }

    return H5::PredType::C_S1;
}

std::string to_string(H5T_order_t order) {
    std::string s;
    switch (order) {
        case H5T_ORDER_LE: s = "<"; break;
        case H5T_ORDER_BE: s = ">"; break;
        case H5T_ORDER_VAX: s = "v"; break;
        case H5T_ORDER_MIXED: s = "m"; break;
        case H5T_ORDER_NONE: s = "|"; break;
        case H5T_ORDER_ERROR: throw std::runtime_error("Found H5T_ORDER_ERROR in to_string(order)");
    }
    return s;
}

std::string to_string(H5T_sign_t sign) {
    std::string s;
    switch (sign) {
        case H5T_SGN_2: s = ""; break;
        case H5T_SGN_NONE: s = "u"; break;
        case H5T_SGN_ERROR: throw std::runtime_error("Found H5T_SGN_ERROR in to_string(sign)");
        case H5T_NSGN: throw std::runtime_error("Found H5T_NSGN with unknown meaning in to_string(sign)");
    }
    return s;
}

std::string to_string(const H5::IntType& int_type) {
    return to_string(int_type.getOrder())
        + to_string(int_type.getSign())
        + "int"
        + std::to_string(int_type.getSize() * 8);
}

std::string memtype(const H5::DataSet& self) {
    H5T_class_t type = self.getTypeClass();

    switch(type) {
        case H5T_INTEGER: {
            return to_string(self.getIntType());
        }
        case H5T_FLOAT: {
            H5::FloatType float_type = self.getFloatType();
            return "float" + std::to_string(float_type.getSize() * 8);
        }
        case H5T_STRING:   return "string";
        case H5T_BITFIELD: return "bitfiled";
        case H5T_COMPOUND: return "float";
        case H5T_NO_CLASS: return "noclass";
        case H5T_TIME:     return "time";
        case H5T_OPAQUE:   return "opaque";
        case H5T_ENUM:     return "enum";
        default: return "unknown";
    }
}


void init_dataset(py::module_& m) {
    py::class_<H5::DataSet> DataSet(m, "DataSet");
    DataSet.def(py::init<>());
    DataSet.def_property_readonly("ndim", &get_ndim);
    DataSet.def_property_readonly("shape", [](const H5::DataSet& self) {
        auto shape = get_shape(self);
        py::tuple shape_tuple = py::cast(shape);
        return shape_tuple;
    });
    DataSet.def("read", [](const H5::DataSet& self) {return py::bytes(read_dataset(self));});
    DataSet.def_property_readonly("storage_size", &H5::DataSet::getStorageSize);
    DataSet.def_property_readonly("in_memory_size", &H5::DataSet::getInMemDataSize);
    DataSet.def_property_readonly("type", &memtype);
}
