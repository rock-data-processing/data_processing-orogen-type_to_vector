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

#include <general_processing/Definitions.hpp>

namespace general_processing {

struct ConvertedVector {

    base::VectorXd data;
    base::VectorXd time;
    StringVector places;
};

}

#endif // GENERALPROCESSING_GENERALPROCESSINGTYPES_HPP
