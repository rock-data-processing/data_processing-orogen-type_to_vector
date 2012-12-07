require 'readline'
require 'orocos'
include Orocos
Orocos.initialize

ENV['BASE_LOG_LEVEL'] = 'DEBUG'
ENV['BASE_LOG_FORMAT'] = 'SHORT'
ENV['ORO_LOGLEVEL'] = '6'

# Testing the debug port creation

#Orocos.run 'general_processing_test', 'valgrind' => true do |p|
Orocos.run 'general_processing_test' do |p|

    task = TaskContext.get 'TestBufferedTask'

    task.debug_conversion = false
    task.create_places = false
    task.aggregator_max_latency = 0.01
    task.debug_buffer = true

    task.createInputPort("rbs1","/base/samples/RigidBodyState","position orientation",0)


    puts "Task  Type  Orocos_Type"
    puts "--- Input ports ---"
    task.each_input_port do |p|
        puts "#{p.name}  #{p.type_name}  #{p.orocos_type_name}"
    end
    
    puts "\n--- Output ports ---"
    task.each_output_port do |p|
        puts "#{p.name}  #{p.type_name}  #{p.orocos_type_name}"
    end

    Readline.readline "Press enter to start task." do
    end

    task.configure
    task.start

    sleep(1.0)
    
end

