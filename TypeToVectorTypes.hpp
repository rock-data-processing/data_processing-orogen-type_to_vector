/**  \brief Specific types. Debug types in particular. */

#ifndef TYPETOVECTOR_TYPETOVECTORTYPES_HPP
#define TYPETOVECTOR_TYPETOVECTORTYPES_HPP

#include <base/eigen.h>

#include <type_to_vector/Definitions.hpp>

namespace type_to_vector {

struct ConvertedVector {

    base::VectorXd data;
    base::VectorXd time;
    StringVector places;
};

struct BufferContent {

    base::MatrixXd data;
    base::MatrixXd time;
};

}

#endif // TYPETOVECTOR_TYPETOVECTORTYPES_HPP
