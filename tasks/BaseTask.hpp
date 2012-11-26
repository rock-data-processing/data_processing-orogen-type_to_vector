/* Generated from orogen/lib/orogen/templates/tasks/Task.hpp */

#ifndef GENERAL_PROCESSING_BASETASK_TASK_HPP
#define GENERAL_PROCESSING_BASETASK_TASK_HPP

#include "general_processing/BaseTaskBase.hpp"

namespace Typelib {
    class Registry;
}
namespace RTT{ 
    namespace base{
        class OutputPortInterface;
    }
}

        
namespace general_processing {
    
    struct DataInfo;
    struct DataVector;
    struct SampleData;

    /*! \class BaseTask 
     * \brief This task provides an interface to add input ports that are assembled into
     * vectors of doubles.
     */
    class BaseTask : public BaseTaskBase
    {
	friend class BaseTaskBase;


        typedef std::vector<DataInfo> DataInfos;
        typedef std::vector<DataVector> Vectors;

        Typelib::Registry* mpRegistry;
        DataInfos mDataInfos;
        Vectors mVectors;

        /** Creates an ouptput port. */
        RTT::base::OutputPortInterface* createOutputPort(const std::string& port_name,
                const std::string& type_name);    

        /** Add ports for debug output. */
        bool addDebugOutput(DataVector& vector, int vector_idx);
       
        /** Adds an input port to data infos. */ 
        bool addInputPort(DataInfo& di, RTT::base::InputPortInterface* reader);
       
        /** Adds data conversion informations. */ 
        bool addDataInfo(RTT::base::InputPortInterface* reader, int vector_idx, 
                const std::string& slice);

        /** Callback function for the StreamAligner. */
        void sampleCallback(base::Time const& timestamp, SampleData const& sample);

    protected:
        
        /** Add all ports of a component to a vector.  */
        virtual bool addComponentToVector(::std::string const & component, 
                                          ::std::string const & slice, 
                                          boost::int32_t to_vector);

        /** Add a port of a componente to a vector. */
        virtual bool addPortToVector(::std::string const & component, 
                                     ::std::string const & port, 
                                     ::std::string const & slice, 
                                     boost::int32_t to_vector);

        /** Creates an input port that data are put into a vector of doubles.*/
        virtual bool createInputPort(::std::string const & port_name, 
                                     ::std::string const & type_name, 
                                     ::std::string const & slice, 
                                     boost::int32_t to_vector);

        /** This functions is called after every positive step of the stream aligner.
         *
         * Overwrite it to process data after each step. Otherwise use the 
         * update hook to implement algorithms. */
        virtual void processingStepCallback();
        
        /** Get the data of the vector at vector_idx. */
        void getDataVector(int vector_idx, base::VectorXd& vector);

        /** Get the time vector of vector at vector_idx. */
        void getTimeVector(int vector_idx, base::VectorXd& time_vector);

        /** Get a time vector that has the size of the data vector.  */
        void getExpandedTimeVector(int vector_idx, base::VectorXd& time_vector);

    public:
        /** TaskContext constructor for BaseTask
         * \param name Name of the task. This name needs to be unique to make it 
         *      identifiable via nameservices.
         * \param initial_state The initial TaskState of the TaskContext. 
         *      Default is Stopped state.
         */
        BaseTask(std::string const& name = "general_processing::BaseTask");

        /** TaskContext constructor for BaseTask 
         * \param name Name of the task. This name needs to be unique to make it 
         *      identifiable for nameservices. 
         * \param engine The RTT Execution engine to be used for this task, which 
         *      serialises the execution of all commands, programs, state machines and 
         *      incoming events for a task. 
         * \param initial_state The initial TaskState of the TaskContext. 
         *      Default is Stopped state.
         */
        BaseTask(std::string const& name, RTT::ExecutionEngine* engine);

        /** Default deconstructor of BaseTask
         */
	~BaseTask();

        /** This hook is called by Orocos when the state machine transitions
         * from PreOperational to Stopped. If it returns false, then the
         * component will stay in PreOperational. Otherwise, it goes into
         * Stopped.
         *
         * It is meaningful only if the #needs_configuration has been specified
         * in the task context definition with (for example):
         \verbatim
         task_context "TaskName" do
           needs_configuration
           ...
         end
         \endverbatim
         */
        // bool configureHook();

        /** This hook is called by Orocos when the state machine transitions
         * from Stopped to Running. If it returns false, then the component will
         * stay in Stopped. Otherwise, it goes into Running and updateHook()
         * will be called.
         */
        // bool startHook();

        /** This hook is called by Orocos when the component is in the Running
         * state, at each activity step. Here, the activity gives the "ticks"
         * when the hook should be called.
         *
         * The error(), exception() and fatal() calls, when called in this hook,
         * allow to get into the associated RunTimeError, Exception and
         * FatalError states. 
         *
         * In the first case, updateHook() is still called, and recover() allows
         * you to go back into the Running state.  In the second case, the
         * errorHook() will be called instead of updateHook(). In Exception, the
         * component is stopped and recover() needs to be called before starting
         * it again. Finally, FatalError cannot be recovered.
         */
        void updateHook();

        /** This hook is called by Orocos when the component is in the
         * RunTimeError state, at each activity step. See the discussion in
         * updateHook() about triggering options.
         *
         * Call recover() to go back in the Runtime state.
         */
        // void errorHook();

        /** Things to be done when going into Stopped state.
         *
         * - clear stream aligner so old data are thrown out. 
         *
         * This hook is called by Orocos when the state machine transitions
         * from Running to Stopped after stop() has been called.
         */
        void stopHook();

        /** Cleans up the vector data conversion things.
         *
         * Including deleting all data and removing the ports.
         *
         * This hook is called by Orocos when the state machine transitions
         * from Stopped to PreOperational, requiring the call to configureHook()
         * before calling start() again.
         */
        void cleanupHook();
    };
}

#endif

