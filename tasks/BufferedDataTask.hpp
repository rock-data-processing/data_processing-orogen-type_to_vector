/* Generated from orogen/lib/orogen/templates/tasks/Task.hpp */

#ifndef GENERAL_PROCESSING_BUFFEREDDATATASK_TASK_HPP
#define GENERAL_PROCESSING_BUFFEREDDATATASK_TASK_HPP

#include "general_processing/BufferedDataTaskBase.hpp"

namespace general_processing {

    struct VectorBuffer;

    /*! \class BufferedDataTask 
     */
    class BufferedDataTask : public BufferedDataTaskBase
    {
	friend class BufferedDataTaskBase;

        typedef std::vector<VectorBuffer> Buffers;

        Buffers mBuffers;

    protected:
        
        /** Processing a sample is called by sampleCallback. */
        virtual void processSample(base::Time const& timestamp, SampleData const& sample);
        
        /** Adds data conversion informations. */ 
        virtual bool addDataInfo(RTT::base::InputPortInterface* reader, int vector_idx, 
                const std::string& slice);
        
        virtual void clear();


        /** Calculates the needed buffer size from times and durations. Set the
         * property buffer_size accordingly.
         */
        virtual boost::int32_t setBufferSizeFromTime(::base::Time const & delta_time, 
                ::base::Time const & window_start, ::base::Time const & window_length);

        /** To get the data matrix from the buffer. 
         *
         * \returns true if the buffer has a matrix.*/
        bool getDataMatrix(int vector_idx, int from, int to, base::MatrixXd& data_matrix);
        
        /** To get the data matrix from the buffer. 
         *
         * Uses getPeriod to resolve the indices. Period is assumed to be 1.0 in case of
         * non-periodic tasks.*/
        bool getDataMatrix(int vector_idx, double from_time, double to_time, 
                base::MatrixXd& data_matrix);

        /** To get the time matrix from the buffer. */
        bool getTimeMatrix(int vector_idx, int from, int to, base::MatrixXd& time_matrix);
        
        /** To get the time matrix from the buffer. Uses getPeriod to resolve the indices.*/
        bool getTimeMatrix(int vector_idx, double from_time, double to_time, 
                base::MatrixXd& time_matrix);

        /** Is the Buffer filled.  */
        bool isFilled(int vector_idx) const;

    public:
        /** TaskContext constructor for BufferedDataTask
         * \param name Name of the task. This name needs to be unique to make it identifiable via nameservices.
         */
        BufferedDataTask(std::string const& name = "general_processing::BufferedDataTask");

        /** TaskContext constructor for BufferedDataTask 
         * \param name Name of the task. This name needs to be unique to make it identifiable for nameservices. 
         * \param engine The RTT Execution engine to be used for this task, which serialises the execution of all commands, programs, state machines and incoming events for a task. 
         * 
         */
        BufferedDataTask(std::string const& name, RTT::ExecutionEngine* engine);

        /** Default deconstructor of BufferedDataTask
         */
	~BufferedDataTask();

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

        /** This hook is called by Orocos when the state machine transitions
         * from Running to Stopped after stop() has been called.
         */
        // void stopHook();

        /** This hook is called by Orocos when the state machine transitions
         * from Stopped to PreOperational, requiring the call to configureHook()
         * before calling start() again.
         */
        // void cleanupHook();
    };
}

#endif

