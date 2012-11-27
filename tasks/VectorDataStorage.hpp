/**
 * \file  DataStorage.hpp
 *
 * \brief 
 *
 *      German Research Center for Artificial Intelligence\n
 *      Project:
 *
 * \date 21.11.2012
 *
 * \author felix.rehrmann@dfki.de
 */

#ifndef GENERAL_PROCESSING_DATASTORAGE_HPP
#define GENERAL_PROCESSING_DATASTORAGE_HPP

#include <rtt/base/DataSourceBase.hpp>

#include <general_processing/Definitions.hpp>
#include <general_processing/VectorBuilder.hpp>

#include "../GeneralProcessingTypes.hpp"

namespace orogen_transports {
    class TypelibMarshallerBase;
}

namespace RTT {
    namespace Base {
        class InputPortInterface;
        class OutputPortInterface;
    }
}

namespace aggregator {
    class StreamAligner;
}

namespace general_processing {
        
enum Conversions { DATACONVERSION, TIMECONVERSION };

struct DataInfo;

// Information that belong to the data a port produces
struct SampleData {

    VectorOfDoubles mData;
    double mTime;
    StringVector mPlaces;
    
    const DataInfo* mpInfo;

    SampleData() : mTime(0), mpInfo(0) {}
    SampleData(const DataInfo& info) : mTime(0), mpInfo(&info) {}
};

struct DataVector;

/** Informations that belongs to the data source or port. */
struct DataInfo {

    orogen_transports::TypelibMarshallerBase* typelibMarshaller;
    orogen_transports::TypelibMarshallerBase::Handle* handle;
    RTT::base::DataSourceBase::shared_ptr sample;

    RTT::base::InputPortInterface* readPort;

    VectorConversion conversions;
   
    int streamIndex; //!< Index of stream in the streamAligner.
    aggregator::StreamAligner* pStreamAligner;

    SampleData newSample;
    
    DataVector* mpVector; //!< Vector to put the data in. 
    SampleData* mpTargetVector;

    bool update(bool create_places=false);
};


/** Holds all vector parts and provides methods to get the assembled vector.*/
struct DataVector : public std::vector<SampleData> {

    /** Will be set to true by component. Setting it to false must to be done by user.*/
    bool mUpdated;
    
    RTT::base::OutputPortInterface* debugOut;

    DataVector() : mUpdated(false), debugOut(0) {}

    SampleData* addVectorPart(const DataInfo& info);

    /** Copies the data into vector and sets mUpdated to false. 
     *
     * \returns true if part of the vector is new data. */
    void getDataVector (base::VectorXd& vector);

    /** Copies the time data into time_vector and sets mUpdatedTime to false. 
     *
     * \returns true if part of the vector is new data. */
    void getTimeVector (base::VectorXd& time_vector);
    
    /** Copies the time data into time_vector and sets mUpdatedTime to false. 
     *
     * time_vector is expanded so that it has the size of the data vector.
     * \returns true if part of the vector is new data*/
    void getExpandedTimeVector (base::VectorXd& time_vector);

    /** Copies the place informations to places_vector and sets mUpdatePlaces to 
     * false. 
     *
     * \returns true if part of the vector is new data*/
    void getPlacesVector (StringVector& places_vector);


    /** Puts the current available data to the struct data and returns it. */
    ConvertedVector& getConvertedVector (ConvertedVector& data);

    /** Write debug data to the port. */
    void writeDebug(); 
};


}
#endif // GENERAL_PROCESSING_DATASTORAGE_HPP
