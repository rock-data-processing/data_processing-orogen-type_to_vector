/* Generated from orogen/lib/orogen/templates/tasks/Task.cpp */

#include "Task.hpp"

using RTT::types::TypeInfo;
using RTT::log;
using RTT::endlog;
using RTT::Error;
using RTT::Info;

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

bool Task::createPort(::std::string const & port_name, ::std::string const & type_name)
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
    ports()->addPort(in_port->getName(), *in_port);
    log(Info) << "Added port " << port_name << " to task." << endlog();
    
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

