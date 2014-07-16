/**
 * \file  VectorDataStorage.hpp
 *
 * \brief What is needed for handling the conversion process in the component.
 */

#ifndef TYPETOVECTOR_VECTORDATASTORAGE_HPP
#define TYPETOVECTOR_VECTORDATASTORAGE_HPP

#include <boost/shared_ptr.hpp>

#include <rtt/base/DataSourceBase.hpp>
#include <rtt/InputPort.hpp>

#include <rtt/typelib/TypelibMarshallerBase.hpp>

#include <base/time.h>

#include <type_to_vector/Definitions.hpp>
#include <type_to_vector/VectorBuilder.hpp>
#include <type_to_vector/MatrixBuffer.hpp>
#include <type_to_vector/BackConverter.hpp>
#include "../TypeToVectorTypes.hpp"

namespace RTT {
    namespace Base {
        class OutputPortInterface;
    }
}

namespace aggregator {
    class StreamAligner;
}

namespace type_to_vector {
        
enum Conversions { DATACONVERSION, TIMECONVERSION };

struct DataInfo;

/** The data of a sample plus additional informations to process the sample. */
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
    RTT::InputPort<base::VectorXd>* rawPort; //!< Allows to feed raw vectors in.

    VectorConversion conversions;

    bool hasTime; //!< True if the type has a time field.
    base::Time period; //!< Sample rate (0 if unknown or not periodic).
    base::Time start; //!< When to start with the time stamping.
    base::Time delta; //!< delta time for time stamping with tref
    bool useTimeNow; //!< set to true if base::Time::now() should be used for stamping.
    unsigned int sampleCounter;
   
    int streamIndex; //!< Index of stream in the streamAligner.
    aggregator::StreamAligner* pStreamAligner;

    SampleData newSample;
    
    int mVectorIndex; //!< Vector to put the data in. 
    int mSampleVectorIndex;

    //For back conversion

    AbstractBackConverter::Pointer back_converter;
    RTT::base::OutputPortInterface* write_port;
    std::vector<double> output_vect;
    bool output_data_available;

    /** Fetches data from the port, convert it and add it to the appropiate stream.*/
    bool update(bool create_places=false);
};


/** Holds all vector parts and provides methods to get the assembled vector.*/
struct DataVector : public std::vector<SampleData> {

    /** Will be set to true by component. Setting it to false must to be done by user.*/
    bool mUpdated;

    /** For the purpose of writing the debug only once. Should not be touched by user. */
    bool wroteDebug; 
    
    RTT::base::OutputPortInterface* debugOut;
    RTT::base::OutputPortInterface* rawOut;

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
    void getPlacesVector (type_to_vector::StringVector& places_vector) const;

    /** Puts the current available data to the struct data and returns it. */
    ConvertedVector& getConvertedVector (ConvertedVector& data) const;

    /** Write debug data to the port. */
    void writeDebug();

    void writeRaw();

    /** Returns true, if all parts have been update once. */
    bool isFilled() const;

    /** Returns the size of the vector currently available with \p getDataVector. */
    int vectorSize() const;
};


typedef boost::shared_ptr<type_to_vector::MatrixBuffer> BufferPointer;

/** \brief Buffering data vectors. */
struct VectorBuffer { 

    int mDataVectorIndex;

    bool newData;
    
protected:
    BufferPointer mDataBuffer;
    BufferPointer mTimeBuffer;

    base::VectorXd mStoreVector;

public:

    VectorBuffer() : mDataVectorIndex(-1), newData(false), debugOut(0) {}
    
    RTT::base::OutputPortInterface* debugOut;
    BufferPointer mNewSampleMask; //!< flag to indicate if this sample was new

    bool create(const DataVector& dv, int vector_count, bool buffer_time=true);

    void push(const DataVector& dv);

    /** Returns true if there was a matrix to get. */
    bool getDataMatrix(int from, int to, base::MatrixXd& matrix);
    bool getTimeMatrix(int from, int to, base::MatrixXd& matrix);
    
    bool getDataMatrix(double from_time, double to_time, double delta_time,
          base::MatrixXd& matrix);  
    bool getTimeMatrix(double from_time, double to_time, double delta_time,
          base::MatrixXd& matrix);

    /**
     * Write the flag if the sample is new to 'matrix'
     */
    bool getNewSampleFlagMatrix(int from, int to, base::MatrixXd& matrix);

    bool isFilled() const { return isCreated() && mDataBuffer->isFilled(); }

    bool isCreated() const { return mDataBuffer.get(); }

    BufferContent& getBufferContent(BufferContent& content);

    void writeDebug();
};

}
#endif // TYPETOVECTOR_VECTORDATASTORAGE_HPP
