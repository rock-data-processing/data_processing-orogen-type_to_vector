require 'readline'
require 'orocos'
include Orocos
Orocos.initialize

ENV['BASE_LOG_LEVEL'] = 'DEBUG'
ENV['BASE_LOG_FORMAT'] = 'SHORT'
ENV['ORO_LOGLEVEL'] = '6'

# Testing the debug port creation

Orocos.run 'type_to_vector::BaseTask' => 'task' do |p|

    task = TaskContext.get 'task'

    task.create_places = false

    task.createInputPort("rbs1","/base/samples/RigidBodyState","",0)

    task.createInputPort("mc2d","/base/MotionCommand2D","",0)
    
    
    task.debug_conversion = true
    task.configure

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

    
    task.start

    sleep(0.2)

    task.stop
    task.cleanup   
    
    puts "after cleanup"

    puts "--- Input ports ---"
    task.each_input_port do |p|
        puts "#{p.name}  #{p.type_name}  #{p.orocos_type_name}"
    end
    
    puts "\n--- Output ports ---"
    task.each_output_port do |p|
        puts "#{p.name}  #{p.type_name}  #{p.orocos_type_name}"
    end
end

