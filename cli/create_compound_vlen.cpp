#include <iostream>
#include <string>
#include <H5Cpp.h>
#include <vector>
#include <random>


const hsize_t n_dims = 1;
const hsize_t n_rows = 100;
const std::string dataset_name = "test";

struct CompoundData {
    double x;
    double y;
    hvl_t  values;

    CompoundData(double x, double y) : x(x), y(y) {};
};


int main () {
    H5::H5File file("vlen_compound.hdf5", H5F_ACC_TRUNC);

    H5::DataSpace dataspace(n_dims, &n_rows);

    // target dtype for the file
    H5::CompType data_type(sizeof(CompoundData));
    data_type.insertMember("x", HOFFSET(CompoundData, x), H5::PredType::NATIVE_DOUBLE);
    data_type.insertMember("y", HOFFSET(CompoundData, y), H5::PredType::NATIVE_DOUBLE);
    data_type.insertMember("values", HOFFSET(CompoundData, values), H5::VarLenType(H5::PredType::NATIVE_DOUBLE));

    H5::DataSet dataset = file.createDataSet(dataset_name, data_type, dataspace);

    // one vector holding the actual data
    std::vector<std::vector<double>> values;
    values.reserve(n_rows);

    // and one holding the hdf5 description and the "simple" columns
    std::vector<CompoundData> data;
    data.reserve(n_rows);

    std::mt19937 gen;
    std::normal_distribution<double> normal(0.0, 1.0);
    std::poisson_distribution<hsize_t> poisson(20);

    for (hsize_t idx = 0; idx < n_rows; idx++) {
        hsize_t size = poisson(gen);
        values.emplace_back();
        values.at(idx).reserve(size);

        for (hsize_t i = 0; i < size; i++) {
            values.at(idx).push_back(normal(gen));
        }

        // set len and pointer for the variable length descriptor
        data.emplace_back(normal(gen), normal(gen));
        data.at(idx).values.len = size;
        data.at(idx).values.p = (void*) &values.at(idx).front();
    }

    dataset.write(&data.front(), data_type);

    return 0;
}
