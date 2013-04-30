/* Generated from orogen/lib/orogen/templates/tasks/Task.hpp */

#ifndef TYPE_TO_VECTOR_BASETASK_TASK_HPP
#define TYPE_TO_VECTOR_BASETASK_TASK_HPP

#include "type_to_vector/BaseTaskBase.hpp"

namespace Typelib {
    class Registry;
}

namespace RTT{ 
    namespace base{
        class OutputPortInterface;
    }
}
      
namespace type_to_vector {
    
    struct DataInfo;
    struct DataVector;
    struct SampleData;

    /*! \class BaseTask 
     * \brief This task provides an interface to add input ports that are assembled into
     * vectors of doubles.
     *
     * The Task provides two means for realizing the interaces:
     *  -# There is an operation available to the outside that let one create a port of
     *       some type.
     *  -# From withhin the componente in particular the updateHook, there are functions
     *       to get the vector data.
     *
     * \section port Create a port.
     * The operation BaseTask::createInputPort is used to add a port to the task. This
     * port has the given type. The data arrqving at the port will be put into a vector.
     */
    class BaseTask : public BaseTaskBase
    {
	friend class BaseTaskBase;

        
        typedef std::vector<DataInfo> DataInfos;
        typedef std::vector<DataVector> Vectors;

        Typelib::Registry* mpRegistry;
        DataInfos mDataInfos;
        Vectors mVectors;

        typedef std::vector<PortConfig> DataPorts;

        DataPorts mDataPorts;

        /** Add ports for debug output. */
        bool addDebugOutput(DataVector& vector, int vector_idx);
       
        /** Adds an input port to data infos, especially the data handling. */ 
        bool addInputPortDataHandling(DataInfo& di);

        /** Adds the converters to the data info. */
        void addConvertersToInfo(DataInfo& di, const std::string& slice);
       
        /** Callback function for the StreamAligner. */
        void sampleCallback(base::Time const& timestamp, SampleData const& sample);

    protected:
        /** Load a typekit so its types are known inside the component.*/
        virtual bool loadTypekit(std::string const& name);
        
        /** Add a port whose data become part of a vector. */
        virtual void addPort(::type_to_vector::PortConfig const & port_config);
       

        /** Processing a sample is called by sampleCallback. */
        virtual void processSample(base::Time const& timestamp, SampleData const& sample);
        
        /** Creates an ouptput port. */
        RTT::base::OutputPortInterface* createOutputPort(const std::string& port_name,
            const std::string& type_name);    
        
        /** Creates an input port. */
        RTT::base::InputPortInterface* createInputPort(::std::string const & port_name, 
            ::std::string const & type_name );

        /** Adds data conversion informations. */ 
        virtual bool createDataInfo(const PortConfig& config);
       
        /** Clear eveyrhint including removing all input ports. */ 
        virtual void clear();
        
        

        const DataVector& getDataVector(int vector_idx) const;

        const DataInfo& getDataInfo(int index) const;

        int getDataVectorCount() const;

        int getDataInfoCount() const;
        
        /** Get the vector at vector_idx. */
        void getVector(int vector_idx, base::VectorXd& vector) const;

        /** Get the time vector of vector at vector_idx. */
        void getTimeVector(int vector_idx, base::VectorXd& time_vector) const;

        /** Get a time vector that has the size of the data vector.  */
        void getExpandedTimeVector(int vector_idx, base::VectorXd& time_vector) const;

        /** Get the places of the vector elements (if stored). */
        void getPlaces(int vector_idx, type_to_vector::StringVector& places) const;

        /** Checks if the vector at idx \c vector_idx is filled.
         *
         * Means that data from each port contributing to the vector are in the
         * vector. If the data does not change */
        bool isFilled(int vector_idx) const;

        /** Checks whether the data are all there and everything is ready to
         * start the processing.
         *
         * The base implementation is, that all vectors have to be filled once. */
        virtual bool isDataAvailable() const;

        /** The processing method, that runs the algorithm.
         *
         * Implement the algorithm here and use getVector to get the vectors.*/
        virtual void process() {}

        /** Fetches the data from the ports, forward them to the stream aligner and
         * step through the stream aligner. */
        virtual void updateData();

    public:
        /** TaskContext constructor for BaseTask
         * \param name Name of the task. This name needs to be unique to make it 
         *      identifiable via nameservices.
         */
        BaseTask(std::string const& name = "type_to_vector::BaseTask");

        /** TaskContext constructor for BaseTask 
         * \param name Name of the task. This name needs to be unique to make it 
         *      identifiable for nameservices. 
         * \param engine The RTT Execution engine to be used for this task, which 
         *      serialises the execution of all commands, programs, state machines and 
         *      incoming events for a task. 
         */
        BaseTask(std::string const& name, RTT::ExecutionEngine* engine);

        /** Default deconstructor of BaseTask
         */
	~BaseTask();

        bool configureHook();

        /** During update all new data will be gathered from the input ports, converted
         * to a vector and feed to the stream aligner.
         *
         * If subclassing from this task this updateHook needs always to be called first.
         *
         * \code
         * void NewTask::updateHook() {
         *  BaseTask::updateHook();
         *
         *  // the new code
         * }
         * \endcode 
         */
        void updateHook();

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

