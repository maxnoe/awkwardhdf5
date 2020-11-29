#include <iostream>
#include <string>
#include <H5Cpp.h>
#include <vector>
#include <random>


const hsize_t n_dims = 1;
const hsize_t n_rows = 100;
const std::string dataset_name = "test";


int main () {
    H5::H5File file("vlen_cpp.hdf5", H5F_ACC_TRUNC);

    H5::DataSpace dataspace(n_dims, &n_rows);

    // target dtype for the file
    H5::FloatType item_type(H5::PredType::IEEE_F64LE);
    H5::VarLenType file_type(item_type);

    // dtype of the generated data
    H5::FloatType mem_item_type(H5::PredType::NATIVE_DOUBLE);
    H5::VarLenType mem_type(item_type);

    H5::DataSet dataset = file.createDataSet(dataset_name, file_type, dataspace);

    std::vector<std::vector<double>> data;
    data.reserve(n_rows);

    // this structure stores length of each varlen row and a pointer to
    // the actual data
    std::vector<hvl_t> varlen_spec(n_rows);

    std::mt19937 gen;
    std::normal_distribution<double> normal(0.0, 1.0);
    std::poisson_distribution<hsize_t> poisson(20);


    for (hsize_t idx; idx < n_rows; idx++) {
        data.emplace_back();

        hsize_t size = poisson(gen);
        data.at(idx).reserve(size);

        varlen_spec.at(idx).len = size;
        varlen_spec.at(idx).p = (void*) &data.at(idx).front();

        for (hsize_t i = 0; i < size; i++) {
            data.at(idx).push_back(normal(gen));
        }
    }

    dataset.write(&varlen_spec.front(), mem_type);

    return 0;
}
