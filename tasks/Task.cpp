/* Generated from orogen/lib/orogen/templates/tasks/Task.cpp */

#include "Task.hpp"
#include <general_processing/ValueToVector.hpp>

#include <rtt/base/PortInterface.hpp>
#include <rtt/DataFlowInterface.hpp>
#include <typelib/value.hh>

#include <Eigen/Core>

#include <iostream>
#include <string>

using RTT::types::TypeInfo;
using RTT::log;
using RTT::endlog;
using RTT::Error;
using RTT::Info;
using RTT::Warning;

using namespace general_processing;

Task::Task(std::string const& name, TaskCore::TaskState initial_state)
    : TaskBase(name, initial_state)
{
}

Task::Task(std::string const& name, RTT::ExecutionEngine* engine, TaskCore::TaskState initial_state)
    : TaskBase(name, engine, initial_state)
{
}

Task::~Task()
{
}

bool Task::createPort(const ::std::string & port_name, const ::std::string & type_name, const ::std::string & container_slice, int vector_id)
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
    RTT::types::TypeInfoRepository::shared_ptr ti = RTT::types::TypeInfoRepository::Instance();
    RTT::types::TypeInfo* type = ti->type(type_name);
    if (!type)
    {
    	log(Error) << "Cannot find port type " << type_name << " in the type info repository." << endlog();
	    return false;
    }
    
    /* Add input port */
    RTT::base::InputPortInterface *in_port = type->inputPort(port_name);
    if (!in_port)
    {
        log(Error) << "An error occurred during input port generation." << endlog();
        return false;
    }
    
    std::string assigned_name = in_port->getName();
    
    ports()->addPort(in_port->getName(), *in_port);
    
    if(port_name.compare(assigned_name))
    {
        log(Warning) << "Assigned port name '" << assigned_name << "' does not match desired port name '" << port_name << "'. " << endlog(); 
    }
    
    log(Info) << "Added port " << port_name << " to task." << endlog();
    
    /* Storing port information */
    // TODO update missing fields
    //VectorValueInfo info;
    //info.containerSlice = container_slice;
    //port_info[port_name] = info;
    
    return true;
}

/// The following lines are template definitions for the various state machine
// hooks defined by Orocos::RTT. See Task.hpp for more detailed
// documentation about them.

// bool Task::configureHook()
// {
//     if (! TaskBase::configureHook())
//         return false;
//     return true;
// }
// bool Task::startHook()
// {
//     if (! TaskBase::startHook())
//         return false;
//     return true;
// }
void Task::updateHook()
{
    TaskBase::updateHook();

    // Get all ports of this task context
    RTT::DataFlowInterface::Ports portsVec = ports()->getPorts();
    RTT::DataFlowInterface::Ports::iterator it;

    // Iterate over all ports of the task context
    for(it = portsVec.begin(); it != portsVec.end(); it++)
    {
        // TODO check necessary, if port is connected?
        
        // Check port type.
        if ((dynamic_cast<RTT::base::InputPortInterface*>(*it)) != 0)
        {
            // We have an input port!
            std::cout << "Found input port " << (*it)->getName() << std::endl;
            
            // TODO This is just a first guess ...
            // Get payload
//            Typelib::Value type;
//            (*it)->read(type);
//            VectorToc toc = VectorTocMaker::apply(type);
//            Eigen::VectorXd payload = toc.valueToEigen(type);
//            std::cout << "payload[0]: " << payload[0] << std::endl;

//            std::cout << "container slice: '" << port_info[(*it)->getName()].containerSlice << "'." << std::endl;
            
        }
    }
    
    
}
// void Task::errorHook()
// {
//     TaskBase::errorHook();
// }
// void Task::stopHook()
// {
//     TaskBase::stopHook();
// }
// void Task::cleanupHook()
// {
//     TaskBase::cleanupHook();
// }

