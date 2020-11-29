#ifndef OBJECT_H
#define OBJECT_H value

#include <H5Cpp.h>


// Necessary because ~H5::H5Object is protected, but unique_ptr requires public dtor
// See https://stackoverflow.com/a/56252649/3838691
class PyH5Object : public H5::H5Object {
    public:
        using H5::H5Object::H5Object;
        ~PyH5Object() {}
};

#endif /* ifndef OBJECT_H */
