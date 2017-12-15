require 'orocos'

Orocos.initialize
Orocos.conf.load_dir(".")

Orocos.run "type_to_vector::BaseTask" => "task" do
    task = Orocos::TaskContext.get "task"
    Orocos.conf.apply(task, ["default", "joints"])
    task.configure
    task.start

    in_data = Types.base.samples.Joints.new
    in_data.elements = [Types.base.JointState.new] * 5

    writer_1    = task.joints_1.writer
    writer_2    = task.joints_2.writer
    reader_all  = task.debug_0.reader
    reader_some = task.debug_1.reader

    while true
        in_data.elements.each do |e|
            e.position = rand
        end
        writer_1.write in_data
        writer_2.write in_data
        sleep 0.1

        sample_all  = reader_all.read
        sample_some = reader_some.read

        if sample_all && sample_some
            print "Typed data:  ["
            in_data.elements.each{|e| print e.position.to_s + " "}
            puts "]"
            puts "All joints:  " + sample_all.data.to_a.to_s
            puts "Some joints: " + sample_some.data.to_a.to_s
            puts "................................."
        end
    end
end
