require 'readline'
require 'orocos'
include Orocos
Orocos.initialize

ENV['BASE_LOG_LEVEL'] = 'DEBUG'
ENV['BASE_LOG_FORMAT'] = 'SHORT'
ENV['ORO_LOGLEVEL'] = '6'

Orocos.run 'type_to_vector::BaseTask' => 'task' do |p|
    task = TaskContext.get 'task'

    puts "--- Test of port creation ---"

    task.create_places = false
    task.debug_conversion = true

    Readline.readline "Press enter to add ports via operation." do
    end

    puts "- add port rbs1 of type /base/samples/RigidBodyState"
    puts "  data are added to vextorIdx 0"
    pc1 = Types::TypeToVector::PortConfig.new
    pc1.portname = "rbs1"
    pc1.type = "/base/samples/RigidBodyState"
    pc1.vectorIdx = 0
    pc1.period = Time.new(0)

    task.addPort(pc1)
    
    puts "- add port mc2d of type /base/MotionCommand2D"
    puts "  data are added to vextorIdx 0"
    pc2 = Types::TypeToVector::PortConfig.new
    pc2.portname = "mc2d"
    pc2.type = "/base/MotionCommand2D"
    pc2.vectorIdx = 0
    pc2.period = Time.new(0)
    
    task.addPort(pc2)

    Readline.readline "Press enter to configure task." do
    end

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

