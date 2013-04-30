require 'readline'
require 'orocos'
include Orocos
Orocos.initialize

ENV['BASE_LOG_LEVEL'] = 'DEBUG'
ENV['BASE_LOG_FORMAT'] = 'SHORT'
ENV['ORO_LOGLEVEL'] = '6'

# Testing the debug port creation

#Orocos.run 'type_to_vector_test', 'valgrind' => true do |p|
Orocos.run 'type_to_vector_test' do |p|

    task = TaskContext.get 'TestBufferedTask'

    puts "--- Test port creation for buffered task ---"

    task.debug_conversion = true
    task.debug_buffer = true

    puts "- add port rbs1 of type /base/samples/RigidBodyState"
    puts "  data are added to vextorIdx 0"
    
    rbs1_p = Types::TypeToVector::PortConfig.new
    rbs1_p.portname = "rbs1"
    rbs1_p.type = "/base/samples/RigidBodyState"
    rbs1_p.slice = "position orientation"

    task.addPort(rbs1_p)

    puts "- ports are created in the configureHook"
    
    task.configure

    puts
    puts "Task  Type  Orocos_Type"
    puts "--- Input ports ---"
    task.each_input_port do |p|
        puts "#{p.name}  #{p.type_name}  #{p.orocos_type_name}"
    end
    
    puts "\n--- Output ports ---"
    task.each_output_port do |p|
        puts "#{p.name}  #{p.type_name}  #{p.orocos_type_name}"
    end
    puts

    Readline.readline "Press enter to quit." do
    end

end
