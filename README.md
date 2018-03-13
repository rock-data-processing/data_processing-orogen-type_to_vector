### The Type-To-Vector Task Library

This tasks in this library can be used as basis for data-agnostic Rock components, e.g. e.g data filters, learning algorithms or statistics. Typed data is converted into Raw data vectors and backwards internally. An arbitrary number of input ports can be added by configuration for any data type that is understood by orogen. 

When creating a new type-agnostic component, you will have to subclass from type_to_vector::BaseTask or type_to_vector::BufferedBaseTask

```
task_context "MyTypeAgnosticTask" do

    subclasses "type_to_vector::BaseTask"

    ...
end
```

Now, the virtual method ```void process()``` has to be implemented. The methods ```void getVector(int vector_idx, base::VectorXd& vector) const``` and ```void setVector(int vector_index, const base::VectorXd& vector)``` can be used to get the raw data stream and set the data stream for back conversion. A minimal configuration looks as follows:

```
port_config:
  - portname: rbs
    type: /base/samples/RigidBodyState
    slice: position
    vectorIdx: 0
```

This creates an input port with name `rbs` of type `/base/samples/RigidBodyState`. Only the the data of the `position` member will be used. Within your `void process()` method you can access the raw data under Index 0 using the `getVector()` method. For further configuration examples, check the [examples folder](https://github.com/rock-data-processing/data_processing-orogen-type_to_vector/tree/master/examples).

For examples of creating components based on type_to_vector check the [data_analysis task library](https://github.com/rock-data-processing/data_processing-orogen-data_analysis).
