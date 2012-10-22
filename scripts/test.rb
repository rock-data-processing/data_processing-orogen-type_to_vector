require 'orocos'
include Orocos
Orocos.initialize

Orocos.run 'general_processing' do |p|
    task = TaskContext.get 'general_processing'
    task.callop("createPort", "myport", "/base/samples/RigidBodyState")
    
    task.start
    
    while true
    end
end

