/**  \brief Specific types. Debug types in particular. */

#ifndef TYPETOVECTOR_TYPETOVECTORTYPES_HPP
#define TYPETOVECTOR_TYPETOVECTORTYPES_HPP

#include <string>
#include <base/eigen.h>
#include <base/time.h>
#include <type_to_vector/Definitions.hpp>

namespace type_to_vector {

/** Configuration for the input ports contributing to the vectors. */
struct PortConfig {
    
    std::string portname; //!< Name the port should have.
    std::string type; //!< Name of the type the port should handle.
    std::string slice; //!< Gives the part of a type that should be used in a vector.
    int vectorIdx; //!< The vector to add the port's data to.
    base::Time period; //!< Sample rate (0 if unknown or not periodic).

    PortConfig() : vectorIdx(0), period(base::Time()) {}

};

/** Data in the converted vector for debug purposes. */
struct ConvertedVector {

    base::VectorXd data;
    base::VectorXd time;
    StringVector places;
};

/** To put out the buffer content on a debug port. */
struct BufferContent {

    base::MatrixXd data;
    base::MatrixXd time;
};

}

#endif // TYPETOVECTOR_TYPETOVECTORTYPES_HPP
