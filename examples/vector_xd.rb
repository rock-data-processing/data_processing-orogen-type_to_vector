require 'orocos'

Orocos.initialize
Orocos.conf.load_dir(".")

Orocos.run "type_to_vector::BaseTask" => "task" do
    task = Orocos::TaskContext.get "task"
    Orocos.conf.apply(task, ["default", "vector_xd"])
    task.start_time = Types.base.Time.now
    task.configure
    task.start

    in_data = Types.base.VectorXd.from_a [1,2,3,4,5]

    writer = task.vector.writer
    reader = task.debug_0.reader

    while true
        in_data = Types.base.VectorXd.from_a [rand,rand,rand,rand,rand]
        writer.write in_data
        sleep 0.1
        sample = reader.read
        if sample
            puts "Typed data: " + in_data.to_a.to_s
            puts "Raw data:   " + sample.data.to_a.to_s
            puts "................................."
        end
    end
end
