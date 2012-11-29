/* Generated from orogen/lib/orogen/templates/tasks/Task.cpp */

#include <rtt/base/PortInterface.hpp>
#include <rtt/base/InputPortInterface.hpp>
#include <rtt/DataFlowInterface.hpp>
#include <rtt/typelib/TypelibMarshallerBase.hpp>
#include <rtt/plugin/PluginLoader.hpp>
#include <typelib/registry.hh>

#include <general_processing/VectorTocMaker.hpp>
#include <general_processing/Converter.hpp>

#include <base/eigen.h>
#include <base/samples/rigid_body_state.h>

#include "../GeneralProcessingTypes.hpp"
#include "VectorDataStorage.hpp"

#include "BaseTask.hpp"

using namespace general_processing;
using RTT::log;
using RTT::endlog;
using RTT::Debug;
using RTT::Info;
using RTT::Error;

namespace general_processing {
class TimeNowConversion : public AbstractConverter {

public:
    TimeNowConversion(const VectorToc& toc) : AbstractConverter(toc) {}

    VectorOfDoubles apply (void* data, bool create_places=false) {
        mVector.clear();
        mVector.push_back(base::Time::now().toSeconds());
        if (mPlaceVector.empty())
            mPlaceVector.push_back("time"); 
        return mVector;
    }
};
}

BaseTask::BaseTask(std::string const& name)
    : BaseTaskBase(name), mpRegistry(new Typelib::Registry())
{
}

BaseTask::BaseTask(std::string const& name, RTT::ExecutionEngine* engine)
    : BaseTaskBase(name, engine), mpRegistry(new Typelib::Registry())
{
}

BaseTask::~BaseTask()
{
    clear();
    delete mpRegistry;
}

bool BaseTask::loadTypekit(std::string const& name)
{
    return RTT::plugin::PluginLoader::Instance()->loadLibrary(name);
}

RTT::base::OutputPortInterface* BaseTask::createOutputPort(const std::string& port_name,
        const std::string& type_name) {
    
    RTT::base::PortInterface *pi = ports()->getPort(port_name);
    if(pi)
    {
        // Port exists. Should not be there already.
        log(Info) << "Port " << port_name << " is already registered." << endlog();
        return 0;
    }

    /* Check if port type is known */
    RTT::types::TypeInfoRepository::shared_ptr ti = 
        RTT::types::TypeInfoRepository::Instance();
    RTT::types::TypeInfo* type = ti->type(type_name);
    if (!type)
    {
    	log(Error) << "Cannot find port type " << type_name << 
            " in the type info repository." << endlog();
	
        return 0;
    }
    
    /* Add output port */
    RTT::base::OutputPortInterface *out_port = type->outputPort(port_name);
    if (!out_port)
    {
        log(Error) << "An error occurred during output port generation." << endlog();
        return 0;
    }
    
    ports()->addPort(out_port->getName(), *out_port);

    log(Info) << "Added output port " << port_name << " to task." << endlog();
     
    return out_port;
}

bool BaseTask::addDebugOutput(DataVector& vector, int vector_idx) {

    if (_debug_conversion.get()) {

        std::string idx_str = boost::lexical_cast<std::string>(vector_idx);

        vector.debugOut = createOutputPort("debug_"+idx_str,
            "/general_processing/ConvertedVector");

        if ( ! vector.debugOut)
            return false;
    }

    return true;
}

bool BaseTask::addInputPort(DataInfo& di, RTT::base::InputPortInterface* reader) {
    
    RTT::types::TypeInfo const* type = reader->getTypeInfo();

    orogen_transports::TypelibMarshallerBase* transport = 
        dynamic_cast<orogen_transports::TypelibMarshallerBase*>(
                type->getProtocol(orogen_transports::TYPELIB_MARSHALLER_ID) );

    if (! transport) {
        log(Error) << "cannot report ports of type " << type->getTypeName() << 
            " as no typekit generated by orogen defines it" << endlog();
        return false;
    }

    mpRegistry->merge(transport->getRegistry());

    if (! mpRegistry->has(transport->getMarshallingType())) {
        log(Error) << "cannot report ports of type " << type->getTypeName() << 
            " as I can't find a typekit Typelib registry that defines it" << endlog();
        return false;
    }

    ports()->addEventPort(reader->getName(), *reader); 
   
    di.typelibMarshaller = transport;
    di.readPort = reader;
    di.handle = di.typelibMarshaller->createSample();
    di.sample = di.typelibMarshaller->getDataSource(di.handle);

    return true;
}

void BaseTask::addConvertersToInfo(DataInfo& di, const std::string& slice) {
    
    VectorToc toc = VectorTocMaker().apply(
        *(mpRegistry->get(di.typelibMarshaller->getMarshallingType())) );

    log(Debug) << "--- created toc for " << toc.mType << ":\n";
    VectorToc::const_iterator its = toc.begin();
    for ( ; its != toc.end(); its++)
        log(Debug) << its->placeDescription << " @ " << its->position << "\n";
    log(Debug) << endlog();

    VectorToc toc_sliced = VectorTocSlicer::slice(toc, slice);
    VectorToc toc_time = VectorTocSlicer::slice(toc, _time_fields.get());

    typedef AbstractConverter::Pointer ConvertPtr;

    ConvertPtr c_sliced_ptr;
    if ( toc_sliced.isFlat() )
        c_sliced_ptr = ConvertPtr(new FlatConverter(toc_sliced));
    else {
        c_sliced_ptr = ConvertPtr(new ConvertToVector(toc_sliced, *mpRegistry));
        boost::static_pointer_cast<ConvertToVector>(c_sliced_ptr)->setSlice(slice);
    }

    ConvertPtr c_time_ptr;
    if ( toc_time.empty() ) {
        c_time_ptr = ConvertPtr(new TimeNowConversion(toc_time));
        di.hasTime = false;
    } else {
        toc_time.resize(1);
        ConvertPtr c_single_ptr(new SingleConverter(toc_time));
        c_time_ptr = ConvertPtr(new MultiplyConverter(c_single_ptr, 1.0e-6));
        di.hasTime = true;
    }
    
    di.conversions.addConverter(c_sliced_ptr);
    di.conversions.addConverter(c_time_ptr);
}

bool BaseTask::addDataInfo(RTT::base::InputPortInterface* reader, int vector_idx,
        const std::string& slice) {
    
    if ( vector_idx < 0 ) return false;

    DataInfo di;
    
    if ( !addInputPort(di, reader) ) return false;
    
    di.conversions = VectorConversion(reader->getName());

    if ( mVectors.size() <= vector_idx ) mVectors.resize(vector_idx+1);
    
    di.mVectorIndex = vector_idx;

    DataVector& dv = mVectors.at(vector_idx);

    if ( !dv.debugOut && !addDebugOutput(dv, vector_idx) ) 
        return false;
 
    aggregator::StreamAligner::Stream<general_processing::SampleData>::callback_t cb = 
        boost::bind(&BaseTask::sampleCallback,this,_1,_2);

    di.streamIndex = _stream_aligner.registerStream<general_processing::SampleData>(cb, 
            0, base::Time());

    di.pStreamAligner = &_stream_aligner;
     
    di.newSample.mDataInfoIndex = mDataInfos.size();
    di.mSampleVectorIndex = dv.addVectorPart();

    addConvertersToInfo(di, slice);
    
    mDataInfos.push_back(di);

    log(Info) << "added data info for " << mDataInfos.back().readPort->getName() <<
        " with type " << mDataInfos.back().conversions.getTypeName() << 
        " to data infos index " << mDataInfos.size()-1 << " and stream index " <<
        mDataInfos.back().streamIndex << endlog();

    return true;
}

void BaseTask::sampleCallback(base::Time const& timestamp, SampleData const& sample) {


    DataInfo& di = mDataInfos.at(sample.mDataInfoIndex);
    DataVector& dv = mVectors.at(di.mVectorIndex);
            
    dv.at(di.mSampleVectorIndex) = sample;

    dv.mUpdated = true;
    dv.wroteDebug = false; 
}

bool BaseTask::addComponentToVector(::std::string const & component, 
        ::std::string const & slice, boost::int32_t to_vector)
{
    return false;
}

bool BaseTask::addPortToVector(::std::string const & component, ::std::string const & port, 
        ::std::string const & slice, boost::int32_t to_vector)
{
    return false;
}

bool BaseTask::createInputPort(::std::string const & port_name, 
                               ::std::string const & type_name, 
                               ::std::string const & slice, 
                               boost::int32_t to_vector)
{
    
    /* Check if port already exists (check name) */
    RTT::base::PortInterface *pi = ports()->getPort(port_name);
    if(pi)
    {
        // Port exists. Returns success.
        log(Info) << "Port " << port_name << " is already registered." << endlog();
        return true;
    }

    /* Check if port type is known */
    RTT::types::TypeInfoRepository::shared_ptr ti = 
        RTT::types::TypeInfoRepository::Instance();
    RTT::types::TypeInfo* type = ti->type(type_name);
    if (!type)
    {
    	log(Error) << "Cannot find port type " << type_name << 
            " in the type info repository." << endlog();
	
        return false;
    }
    
    /* Create input port */
    RTT::base::InputPortInterface *in_port = type->inputPort(port_name);
    if (!in_port)
    {
        log(Error) << "An error occurred during input port generation." << endlog();
        return false;
    }
    
    //ports()->addPort(in_port->getName(), *in_port);

    return addDataInfo(in_port, to_vector, slice);  
}

void BaseTask::processingStepCallback() {

}

void BaseTask::getDataVector(int vector_idx, base::VectorXd& vector) {
    mVectors.at(vector_idx).getDataVector(vector);
}

void BaseTask::getTimeVector(int vector_idx, base::VectorXd& time_vector) {
    mVectors.at(vector_idx).getTimeVector(time_vector);
}

void BaseTask::getExpandedTimeVector(int vector_idx, base::VectorXd& time_vector) {
    mVectors.at(vector_idx).getExpandedTimeVector(time_vector);
}
    

void BaseTask::clear() {

    DataInfos::iterator it = mDataInfos.begin();

    for ( ; it != mDataInfos.end(); it++) {

        it->typelibMarshaller->deleteHandle(it->handle);

        _stream_aligner.unregisterStream(it->streamIndex);

        ports()->removePort(it->readPort->getName());

        DataVector& dv = mVectors.at(it->mVectorIndex);
        if ( dv.debugOut )
            ports()->removePort(dv.debugOut->getName());
    }

    mDataInfos.clear();
    mVectors.clear();
}

// bool BaseTask::configureHook()
// {
//     if (! BaseTaskBase::configureHook())
//         return false;
//     return true;
// }
// bool BaseTask::startHook()
// {
//     if (! BaseTaskBase::startHook())
//         return false;
//     return true;
// }

void BaseTask::updateHook()
{
    BaseTaskBase::updateHook();

    DataInfos::iterator data_it = mDataInfos.begin();

    for ( ; data_it != mDataInfos.end(); data_it++ ) {
        while ( data_it->update(_create_places.get()) );
    }
    
    while ( _stream_aligner.step() );
        
    if ( _debug_conversion.get() ) {
        Vectors::iterator vector_it = mVectors.begin();
        for ( ; vector_it != mVectors.end(); vector_it++ )
            vector_it->writeDebug();
    }
}

// void BaseTask::errorHook()
// {
//     BaseTaskBase::errorHook();
// }

void BaseTask::stopHook()
{
    BaseTaskBase::stopHook();
    _stream_aligner.clear();
}

void BaseTask::cleanupHook()
{
    BaseTaskBase::cleanupHook();
    clear(); 
}

