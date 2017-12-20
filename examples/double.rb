require 'orocos'

Orocos.initialize
Orocos.conf.load_dir(".")

Orocos.run "type_to_vector::BaseTask" => "task" do
    task = Orocos::TaskContext.get "task"
    Orocos.conf.apply(task, ["default", "double"])
    task.configure
    task.start

    in_data = 1.0
    writer    = task.double.writer
    reader = task.debug_0.reader

    while true
        writer.write in_data
        sleep 0.1
        sample = reader.read_new
        if sample
            puts "Typed data: " + in_data.to_s
            puts "Raw data:   " + sample.data.to_a[0].to_s
            puts "................................."
        end
        in_data+=1
    end
end
