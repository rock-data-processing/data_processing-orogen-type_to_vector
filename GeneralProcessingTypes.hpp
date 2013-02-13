/**
 * \file  GeneralProcessingTypes.hpp
 *
 * \brief Specific types. Debug types in particular. 
 *
 *      German Research Center for Artificial Intelligence\n
 *      Project:
 *
 * \date 26.11.2012
 *
 * \author felix.rehrmann@dfki.de
 */

#ifndef GENERALPROCESSING_GENERALPROCESSINGTYPES_HPP
#define GENERALPROCESSING_GENERALPROCESSINGTYPES_HPP

#include <base/eigen.h>

#include <type_to_vector/Definitions.hpp>

namespace general_processing {

struct ConvertedVector {

    base::VectorXd data;
    base::VectorXd time;
    type_to_vector::StringVector places;
};

struct BufferContent {

    base::MatrixXd data;
    base::MatrixXd time;
};

}

#endif // GENERALPROCESSING_GENERALPROCESSINGTYPES_HPP
