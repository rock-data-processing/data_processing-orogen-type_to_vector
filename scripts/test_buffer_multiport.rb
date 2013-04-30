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

    task.debug_buffer = true
    task.buffer_time = true
    task.buffer_size = 5
    task.buffer_new_only = false

    rbs1_p = Types::TypeToVector::PortConfig.new
    rbs1_p.portname = "rbs1"
    rbs1_p.type = "/base/samples/RigidBodyState"
    rbs1_p.slice = "position orientation"
    
    task.addPort(rbs1_p)

    mc2d_p = Types::TypeToVector::PortConfig.new
    mc2d_p.portname = "mc2d"
    mc2d_p.type = "/base/MotionCommand2D"

    task.addPort(mc2d_p)
    
    rbs2_p = Types::TypeToVector::PortConfig.new
    rbs2_p.portname = "rbs2"
    rbs2_p.type = "/base/samples/RigidBodyState"
    rbs2_p.slice = "orientation"
    rbs2_p.vectorIdx = 1

    task.addPort(rbs2_p)
    
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

    w_rbs = task.rbs1.writer
    w_mc2d = task.mc2d.writer
    r_b0 = task.debug_buffer_0.reader
    
    w_rbs1 = task.rbs2.writer
    r_b1 = task.debug_buffer_1.reader

    Readline.readline "Press enter to start task." do
    end

    task.start
    
    mc2d = w_mc2d.new_sample 
    rbs = w_rbs.new_sample
    rbs.position.z = 0.0
    angle = 0.0

    stop_t = false

    write_t = Thread.new do
        while !stop_t
            t_now = Time.now
            mc2d.translation = Math::sin(t_now.to_f / 10.0 * Math::PI)
            mc2d.rotation = Math::cos(t_now.to_f * Math::PI) * 0.1 + 0.5
            rbs.position.x = t_now.to_i % 3
            rbs.position.y = 2.0
            rbs.position.z += 1
            angle += 0.01
            rbs.orientation = Eigen::Quaternion.from_angle_axis(angle, Eigen::Vector3.new(0.1,2.0,0.5))
            rbs.time = t_now
            w_rbs.write(rbs)
            w_mc2d.write(mc2d)
            w_rbs1.write(rbs)
            sleep(0.1)
        end
    end

    puts "- reading 50 buffer outputs"
    (1..50).each do |idx|

        puts "cnt #{idx}"
 
        sleep 0.1

        if cv = r_b0.read()
            puts "--- debug buffer 0 ---"
            puts "time:"
            puts cv.time
            puts "data:"
            puts cv.data
        end
        if cv = r_b1.read()
            puts "--- debug buffer 1 ---"
            puts "time:"
            puts cv.time
            puts "data:"
            puts cv.data
        end
    end    

    stop_t = true
    write_t.join
end

