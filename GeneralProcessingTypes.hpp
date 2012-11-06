/**
 * \file  GeneralProcessingTypes.hpp
 *
 * \brief Spedcial types for the general processing component.
 *
 *      German Research Center for Artificial Intelligence\n
 *      Project: VirGo4
 *
 * \date 05.11.2012
 *
 * \author felix.rehrmann@dfki.de
 */

#ifndef GENERALPROCESSING_GENERALPROCESSINGTYPES_HPP
#define GENERALPROCESSING_GENERALPROCESSINGTYPES_HPP

#include <string>
#include <vector>

#include <base/Time.h>

namespace general_processing {

struct VectorTocInfo {
    int vectorIndex;
    std::string tocString;
    std::vector<base::Time> timestampVector;
};

} // general_processing

#endif //  GENERALPROCESSING_GENERALPROCESSINGTYPES_HPP

