require 'readline'
require 'orocos'
include Orocos
Orocos.initialize

ENV['BASE_LOG_LEVEL'] = 'WARNING'
ENV['BASE_LOG_FORMAT'] = 'SHORT'
ENV['ORO_LOGLEVEL'] = '4'

# Testing the debug port creation

#Orocos.run 'type_to_vector_test', 'valgrind' => true do |p|
Orocos.run 'type_to_vector_test' do |p|

    task = TaskContext.get 'TestBufferedTask'

    task.debug_buffer = true
    task.buffer_time = true
    task.buffer_size = 5
    task.buffer_new_only = false
    task.aggregator_max_latency = 0.3

    rbs1_p = Types::TypeToVector::PortConfig.new
    rbs1_p.portname = "rbs1"
    rbs1_p.type = "/base/samples/RigidBodyState"
    rbs1_p.slice = "position orientation"
    rbs1_p.vectorIdx = 0
    rbs1_p.period = 0.0
    rbs1_p.useTimeNow = false
    
    task.addPort(rbs1_p)

    mc2d_p = Types::TypeToVector::PortConfig.new
    mc2d_p.portname = "mc2d"
    mc2d_p.type = "/base/MotionCommand2D"
    mc2d_p.slice = ""
    mc2d_p.vectorIdx = 0
    mc2d_p.period = 0.0
    mc2d_p.useTimeNow = false

    task.addPort(mc2d_p)
    
    mc2p_p = Types::TypeToVector::PortConfig.new
    mc2p_p.portname = "mc2d_periodic"
    mc2p_p.type = "/base/MotionCommand2D"
    mc2p_p.slice = ""
    mc2p_p.vectorIdx = 1
    mc2p_p.period = 0.1
    mc2d_p.useTimeNow = false

    task.addPort(mc2p_p)

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
    
    w_mc2p = task.mc2d_periodic.writer
    r_b1 = task.debug_buffer_1.reader

    puts "settint start time to now and start ..."
    task.start_time = Time.now
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
            w_mc2p.write(mc2d)
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
