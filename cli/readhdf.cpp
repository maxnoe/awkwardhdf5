#include <iostream>
#include <memory>
#include <vector>
#include "H5Cpp.h"


int main() {
    H5::H5File file("test.hdf5", H5F_ACC_RDONLY);
    std::cout << H5F_ACC_RDONLY << std::endl;

    auto group = file.openGroup("test");
    auto dataset = group.openDataSet("data");
    auto dataspace = dataset.getSpace();

    auto ndim = dataspace.getSimpleExtentNdims();
    std::cout << "ndim: " << ndim << std::endl;

    std::vector<hsize_t> shape(ndim);
    dataspace.getSimpleExtentDims(&shape.front());

    hsize_t size = dataspace.getSimpleExtentNpoints();
    std::cout << "size: " << size << std::endl;

    auto type = dataset.getTypeClass();
    std::cout << type << std::endl;

    std::cout << "shape: (";
    for (hsize_t val: shape) {
        std::cout << val << ", ";
    }
    std::cout << ")" << std::endl;


    std::vector<double> data(size);
    dataset.read(&data.front(), H5::PredType::NATIVE_DOUBLE);

    return 0; 
}
