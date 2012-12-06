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

#include <boost/shared_ptr.hpp>

#include <rtt/base/DataSourceBase.hpp>

#include <rtt/typelib/TypelibMarshallerBase.hpp>

#include <general_processing/Definitions.hpp>
#include <general_processing/VectorBuilder.hpp>
#include <general_processing/MatrixBuffer.hpp>

#include "../GeneralProcessingTypes.hpp"

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
    
    int mDataInfoIndex;
};

struct DataVector;

/** Informations that belongs to the data source or port. */
struct DataInfo {

    orogen_transports::TypelibMarshallerBase* typelibMarshaller;
    orogen_transports::TypelibMarshallerBase::Handle* handle;
    RTT::base::DataSourceBase::shared_ptr sample;

    RTT::base::InputPortInterface* readPort;

    VectorConversion conversions;

    bool hasTime; //!< True if the type has a time field.
   
    int streamIndex; //!< Index of stream in the streamAligner.
    aggregator::StreamAligner* pStreamAligner;

    SampleData newSample;
    
    int mVectorIndex; //!< Vector to put the data in. 
    int mSampleVectorIndex;

    bool update(bool create_places=false);
};


/** Holds all vector parts and provides methods to get the assembled vector.*/
struct DataVector : public std::vector<SampleData> {

    /** Will be set to true by component. Setting it to false must to be done by user.*/
    bool mUpdated;

    /** For the purpose of writing the debug only once. Should not be touched by user. */
    bool wroteDebug; 
    
    RTT::base::OutputPortInterface* debugOut;

    DataVector() : mUpdated(false), wroteDebug(true), debugOut(0) {}

    /** Adds a part of the vector and returns the index. */
    int addVectorPart();

    /** Copies the data into vector and sets mUpdated to false. 
     *
     * \returns true if part of the vector is newdata. */
    void getVector (base::VectorXd& vector) const;

    /** Copies the time data into time_vector and sets mUpdatedTime to false. 
     *
     * \returns true if part of the vector is new data. */
    void getTimeVector (base::VectorXd& time_vector) const;
    
    /** Copies the time data into time_vector and sets mUpdatedTime to false. 
     *
     * time_vector is expanded so that it has the size of the data vector.
     * \returns true if part of the vector is new data*/
    void getExpandedTimeVector (base::VectorXd& time_vector) const;

    /** Copies the place informations to places_vector and sets mUpdatePlaces to 
     * false. 
     *
     * \returns true if part of the vector is new data*/
    void getPlacesVector (StringVector& places_vector) const;

    /** Puts the current available data to the struct data and returns it. */
    ConvertedVector& getConvertedVector (ConvertedVector& data) const;

    /** Write debug data to the port. */
    void writeDebug();

    /** Returns true, if all parts have been update once. */
    bool isFilled() const;

    /** Returns the size of the vector currently available with \p getDataVector. */
    int vectorSize() const;
};


typedef boost::shared_ptr<MatrixBuffer> BufferPointer;

/** Buffering data vectors. */
struct VectorBuffer {

    int mDataVectorIndex;
    
protected:
    BufferPointer mDataBuffer;
    BufferPointer mTimeBuffer;

    base::VectorXd mStoreVector;

public:
    
    RTT::base::OutputPortInterface* debugOut;

    bool create(const DataVector& dv, int vector_count, bool buffer_time=true);

    void push(const DataVector& dv);

    /** Returns true if there was a matrix to get. */
    bool getDataMatrix(int from, int to, base::MatrixXd& matrix);
    bool getTimeMatrix(int from, int to, base::MatrixXd& matrix);
    
    bool getDataMatrix(double from_time, double to_time, double delta_time,
          base::MatrixXd& matrix);  
    bool getTimeMatrix(double from_time, double to_time, double delta_time,
          base::MatrixXd& matrix);

    bool isFilled() const { return mDataBuffer->isFilled(); }

    bool isCreated() const { return mDataBuffer.get(); }

    BufferContent& getBufferContent(BufferContent& content);

    void writeDebug();
};

}
#endif // GENERAL_PROCESSING_DATASTORAGE_HPP
