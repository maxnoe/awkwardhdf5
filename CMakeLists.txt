cmake_minimum_required(VERSION 3.15...3.19)


project(awkwardhdf5 VERSION 0.1.0 LANGUAGES C CXX)

find_package(HDF5 REQUIRED COMPONENTS C CXX)

find_package(Python3 REQUIRED COMPONENTS Interpreter Development)
message(STATUS "Python3 library: ${Python3_LIBRARIES}")

add_subdirectory(vendor/pybind11)

pybind11_add_module(
    awkwardhdf5 src/awkwardhdf5.cpp
    src/Location.cpp src/Object.cpp 
    src/Dataset.cpp src/File.cpp src/Group.cpp
)
target_link_libraries(awkwardhdf5 PUBLIC hdf5::hdf5 hdf5::hdf5_cpp)


add_executable(readhdf cli/readhdf.cpp)
target_link_libraries(readhdf PUBLIC hdf5::hdf5 hdf5::hdf5_cpp)
set_target_properties(PROPERTIES CXX_STANDARD 14)
set_target_properties(PROPERTIES CXX_STANDARD_REQUIRED TRUE)
