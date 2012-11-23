// \file  DataStorage.cpp

#include <base/eigen.h>

#include <rtt/base/PortInterface.hpp>
#include <rtt/base/InputPortInterface.hpp>
#include <rtt/DataFlowInterface.hpp>
#include <rtt/typelib/TypelibMarshallerBase.hpp>

#include <aggregator/StreamAligner.hpp>

#include <general_processing/VectorBuilder.hpp>

#include "VectorDataStorage.hpp"

using namespace general_processing;
    
bool DataInfo::update(bool create_places) {
       
    if( readPort->read(sample, false) == RTT::NewData) {
        
        typelibMarshaller->refreshTypelibSample(handle);

        uint8_t* data_ptr = typelibMarshaller->getTypelibSample(handle);

        conversions.update(data_ptr, create_places);

        newSample.mData = conversions.getData(DATACONVERSION);
        newSample.mTime = conversions.getData(TIMECONVERSION).front();

        base::Time t = *(reinterpret_cast<base::Time*>( data_ptr + 
                    conversions.getToc(TIMECONVERSION).front().position) );

        pStreamAligner->push(streamIndex, t, &newSample);

        if ( create_places )
            newSample.mPlaces = conversions.getPlaces(DATACONVERSION);
        else
            newSample.mPlaces.clear();

        return true;
    }  
    return false;
}
    

SampleData* DataVector::addVectorPart(const DataInfo& info) {
    push_back(SampleData(info));
    return &back();
}

bool DataVector::getDataVector (base::VectorXd& vector) {

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
    
    bool result = mUpdated;
    mUpdated = false;

    return result;
}

bool DataVector::getTimeVector(base::VectorXd& time_vector) {

    time_vector.resize(size());
    
    const_iterator it = begin();

    int i = 0;
    for (; it != end(); it++)
        time_vector[i++] = it->mTime;
    
    bool result = mUpdatedTime;
    mUpdatedTime = false;

    return result;
}

bool DataVector::getExpandedTimeVector(base::VectorXd& time_vector) {
    
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
    
    bool result = mUpdatedTime;
    mUpdatedTime = false;

    return result;
}

bool DataVector::getPlacesVector (StringVector& places_vector) {
    return false;
}
