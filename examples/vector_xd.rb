require 'orocos'

Orocos.initialize
Orocos.conf.load_dir(".")

Orocos.run "type_to_vector::BaseTask" => "task" do
    task = Orocos::TaskContext.get "task"
    Orocos.conf.apply(task, ["vector_xd"])
    task.configure
    task.start

    in_data = Types.base.VectorXd.from_a [1,2,3,4,5]

    writer = task.vector.writer
    reader = task.raw_out_0.reader

    while true
        writer.write in_data
        sample = reader.read
        if sample
            puts "Typed data: " + in_data.to_a.to_s
            puts "Raw data:   " + sample.to_a.to_s
            puts "................................."
        end
        sleep 0.1
    end
end
