/* Generated from orogen/lib/orogen/templates/tasks/Task.cpp */

#include "VectorDataStorage.hpp"

#include "BufferedDataTask.hpp"

using namespace general_processing;

BufferedDataTask::BufferedDataTask(std::string const& name)
    : BufferedDataTaskBase(name)
{
}

BufferedDataTask::BufferedDataTask(std::string const& name, RTT::ExecutionEngine* engine)
    : BufferedDataTaskBase(name, engine)
{
}

BufferedDataTask::~BufferedDataTask()
{
}

void BufferedDataTask::processSample(base::Time const& timestamp, SampleData const& sample) {

    BaseTask::processSample(timestamp, sample);

    const DataInfo& di = getDataInfo(sample.mDataInfoIndex);
    VectorBuffer& buf = mBuffers.at(di.mVectorIndex);
    
    buf.newData = true;
}

bool BufferedDataTask::addDataInfo(RTT::base::InputPortInterface* reader, int vector_idx, 
                const std::string& slice) {

    BaseTask::addDataInfo(reader, vector_idx, slice);
    
    if ( mBuffers.size() <= vector_idx ) mBuffers.resize(vector_idx+1);

    mBuffers.at(vector_idx).mDataVectorIndex = vector_idx; 

    if ( _debug_buffer.get() && !mBuffers.at(vector_idx).debugOut ) {

        std::string idx_str = boost::lexical_cast<std::string>(vector_idx);
        
        mBuffers.at(vector_idx).debugOut = createOutputPort("debug_buffer_"+idx_str,
            "/general_processing/BufferContent");
    }
    
}

void BufferedDataTask::clear() {

    BaseTask::clear();
    
    Buffers::iterator it = mBuffers.begin();

    for ( ; it != mBuffers.end(); it++ )
        if ( it->debugOut )
            ports()->removePort(it->debugOut->getName());
    
    mBuffers.clear();
}

boost::int32_t BufferedDataTask::setBufferSizeFromTime(::base::Time const & delta_time, ::base::Time const & window_start, ::base::Time const & window_length)
{

    int n = (window_start.toSeconds()+window_length.toSeconds()) / delta_time.toSeconds();
    n++;
    _buffer_size.set(n);
    return n;
}

bool BufferedDataTask::getDataMatrix(int vector_idx, int from, int to, 
        base::MatrixXd& data_matrix) {

    return mBuffers.at(vector_idx).getDataMatrix(from, to, data_matrix);
}
        
bool BufferedDataTask::getDataMatrix(int vector_idx, double from_time, double to_time, 
                base::MatrixXd& data_matrix) {

    double dt = getPeriod();
    if ( dt <= 0 ) dt = 1.0;

    return mBuffers.at(vector_idx).getDataMatrix(from_time, to_time, dt, data_matrix); 
}

bool BufferedDataTask::getTimeMatrix(int vector_idx, int from, int to, 
        base::MatrixXd& time_matrix) {

    return mBuffers.at(vector_idx).getTimeMatrix(from, to, time_matrix);
}

bool BufferedDataTask::getTimeMatrix(int vector_idx, double from_time, double to_time, 
                base::MatrixXd& time_matrix) {

    double dt = getPeriod();
    if ( dt <= 0 ) dt = 1.0;

    return mBuffers.at(vector_idx).getTimeMatrix(from_time, to_time, dt, time_matrix); 
}
        
bool BufferedDataTask::isBufferFilled(int vector_idx) const 
    { return mBuffers.at(vector_idx).isFilled(); }



/// The following lines are template definitions for the various state machine
// hooks defined by Orocos::RTT. See BufferedDataTask.hpp for more detailed
// documentation about them.

// bool BufferedDataTask::configureHook()
// {
//     if (! BufferedDataTaskBase::configureHook())
//         return false;
//     return true;
// }
// bool BufferedDataTask::startHook()
// {
//     if (! BufferedDataTaskBase::startHook())
//         return false;
//     return true;
// }

bool BufferedDataTask::isDataAvailable () const {

    Buffers::const_iterator it = mBuffers.begin();

    for ( ; it != mBuffers.end(); it++)
        if ( !it->isFilled() ) return false;

    return true;
}

void BufferedDataTask::updateData() {

    BufferedDataTaskBase::updateData();
    
    Buffers::iterator it = mBuffers.begin();
    
    for ( int i=0; it != mBuffers.end(); it++,i++ ) {

        if ( ! it->isCreated() ) {
            const DataVector& dv = getDataVector(it->mDataVectorIndex);
            it->create(dv, _buffer_size.get(), _buffer_time.get());
        }

        if ( it->isCreated() && 
                ((_buffer_new_only.get() && it->newData) || !_buffer_new_only.get()) ) {

            const DataVector& dv = getDataVector(it->mDataVectorIndex);
            it->push(dv);

            it->newData = false;
            
            it->writeDebug();
        }
    }
}
