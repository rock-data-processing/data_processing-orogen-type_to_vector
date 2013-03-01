require 'readline'
require 'orocos'
include Orocos
Orocos.initialize

ENV['BASE_LOG_LEVEL'] = 'DEBUG'
ENV['BASE_LOG_FORMAT'] = 'SHORT'
ENV['ORO_LOGLEVEL'] = '6'

# Testing the debug port creation

Orocos.run 'type_to_vector_test' do |p|

    task = TaskContext.get 'TestBaseTask'

    task.debug_conversion = true
    task.create_places = false

    task.createInputPort("rbs1","/base/samples/RigidBodyState","position",0)

    task.createInputPort("mc2d","/base/MotionCommand2D","",0)

    puts "Task  Type  Orocos_Type"
    puts "--- Input ports ---"
    task.each_input_port do |p|
        puts "#{p.name}  #{p.type_name}  #{p.orocos_type_name}"
    end
    
    puts "\n--- Output ports ---"
    task.each_output_port do |p|
        puts "#{p.name}  #{p.type_name}  #{p.orocos_type_name}"
    end

    w_rbs = task.rbs1.writer
    w_mc2d = task.mc2d.writer
    r_d0 = task.debug_0.reader

    Readline.readline "Press enter to start task." do
    end

    task.configure
    task.start

    rbs = w_rbs.new_sample
    rbs.position = Eigen::Vector3.new(1.0,2.0,3.0)
    rbs.time = Time.now
    w_rbs.write(rbs)

    sleep(0.3)

    mc2d = w_mc2d.new_sample
    mc2d.translation = 10.0
    mc2d.rotation = -1.0
    w_mc2d.write(mc2d)

    Readline.readline "Press enter to read data" do
    end

    if cv = r_d0.read()
        puts "time:"
        puts cv.time
        puts "data:"
        puts cv.data
        puts "places"
        puts cv.places
    end

    Readline.readline "Press enter to quit" do
    end
end

