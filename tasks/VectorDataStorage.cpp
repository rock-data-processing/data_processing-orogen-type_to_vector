// \file  DataStorage.cpp

#include <base/eigen.h>

#include <rtt/base/PortInterface.hpp>
#include <rtt/base/InputPortInterface.hpp>
#include <rtt/OutputPort.hpp>
#include <rtt/DataFlowInterface.hpp>
#include <rtt/typelib/TypelibMarshallerBase.hpp>

#include <aggregator/StreamAligner.hpp>

#include <general_processing/VectorBuilder.hpp>

#include "VectorDataStorage.hpp"

using namespace general_processing;
using RTT::log;
using RTT::endlog;
using RTT::Debug;
    
bool DataInfo::update(bool create_places) {
       
    if( readPort->read(sample, false) == RTT::NewData) {

        typelibMarshaller->refreshTypelibSample(handle);

        uint8_t* data_ptr = typelibMarshaller->getTypelibSample(handle);

        conversions.update(data_ptr, create_places);

        newSample.mData = conversions.getData(DATACONVERSION);

        base::Time t;

        if ( hasTime ) {
            newSample.mTime = conversions.getData(TIMECONVERSION).front();

            t = *(reinterpret_cast<base::Time*>( data_ptr + 
                        conversions.getToc(TIMECONVERSION).front().position) );
        } else {
            t = base::Time::now();
            newSample.mTime = t.toSeconds();
        } 
        
        if ( create_places )
            newSample.mPlaces = conversions.getPlaces(DATACONVERSION);
        else
            newSample.mPlaces.clear();

        pStreamAligner->push(streamIndex, t, newSample);

        return true;
    }  
    return false;
}
    

int DataVector::addVectorPart() {
    push_back(SampleData());
    return size()-1;
}

void DataVector::getDataVector (base::VectorXd& vector) {

    iterator it = begin();
    int size = 0;

    for ( ; it != end(); it++ )
        size += it->mData.size();

    vector.resize(size);

    int pos = 0;
    it = begin();
    for (; it != end(); it++) {
        int n = it->mData.size();
        if (n == 0) continue;
        vector.segment(pos, n) = Eigen::Map<base::VectorXd>(&(it->mData[0]), n);
        pos += n;
    }
}

void DataVector::getTimeVector(base::VectorXd& time_vector) {

    time_vector.resize(size());
    
    const_iterator it = begin();

    int i = 0;
    for (; it != end(); it++)
        time_vector[i++] = it->mTime;
}

void DataVector::getExpandedTimeVector(base::VectorXd& time_vector) {
    
    iterator it = begin();
    int size = 0;

    for ( ; it != end(); it++ )
        size += it->mData.size();

    time_vector.resize(size);
    
    int pos = 0;
    it = begin();
    for (; it != end(); it++) {
        int n = it->mData.size();
        if (n == 0) continue;
        time_vector.segment(pos, n) = base::VectorXd::Constant(n,it->mTime);
        pos += n;
    }
}

void DataVector::getPlacesVector (StringVector& places_vector) {
    iterator it = begin();
    int size = 0;
    
    for ( ; it != end(); it++ )
        size += it->mPlaces.size();

    places_vector.clear();
    places_vector.reserve(size);

    for ( it = begin(); it != end(); it++ )
       places_vector.insert(places_vector.end(),it->mPlaces.begin(), it->mPlaces.end());
}

ConvertedVector& DataVector::getConvertedVector (ConvertedVector& data ) {

    getDataVector(data.data);
    getExpandedTimeVector(data.time);
    getPlacesVector(data.places);
    return data;
}

void DataVector::writeDebug() {

    if ( !debugOut && wroteDebug ) return;
    
    static ConvertedVector data;
    static_cast<RTT::OutputPort<ConvertedVector>*>(debugOut)->
        write(getConvertedVector(data));
    wroteDebug = true;
}

bool DataVector::isFilled() {

    for ( const_iterator it = begin(); it != end(); it++)
        if ( it->empty() ) return false;

    return true;
}
