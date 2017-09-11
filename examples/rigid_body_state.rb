require 'orocos'

Orocos.initialize
Orocos.conf.load_dir(".")

Orocos.run "type_to_vector::BaseTask" => "task" do
    task = Orocos::TaskContext.get "task"
    Orocos.conf.apply(task, ["rigid_body_state"])
    task.configure
    task.start

    in_data = Types.base.samples.RigidBodyState.new
    in_data.position = Types.base.Vector3d.new(1,2,3)

    writer = task.rbs.writer
    reader = task.raw_out_0.reader

    while true
        writer.write in_data
        sample = reader.read
        if sample
            puts "Typed data: " + in_data.position.to_a.to_s
            puts "Raw data:   " + sample.to_a.to_s
            puts "................................."
        end
        sleep 0.1
    end
end
