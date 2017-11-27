require 'orocos'

Orocos.initialize
Orocos.conf.load_dir(".")

Orocos.run "type_to_vector::BaseTask" => "task" do
    task = Orocos::TaskContext.get "task"
    Orocos.conf.apply(task, ["rigid_body_state"])
    task.configure
    task.start

    in_data = Types.base.samples.RigidBodyState.new

    writer = task.rbs.writer
    reader = task.raw_out_0.reader

    while true
        in_data.position = Types.base.Vector3d.new(rand(),rand(),rand())
        writer.write in_data
        sleep 0.1
        sample = reader.read_new
        if sample
            puts "Typed data: " + in_data.position.to_a.to_s
            puts "Raw data:   " + sample.to_a.to_s
            puts "................................."
        end
    end
end
