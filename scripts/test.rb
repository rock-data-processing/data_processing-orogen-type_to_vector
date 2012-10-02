require 'orocos'
include Orocos
Orocos.initialize

Orocos.run 'abstract_processing_component_test' do |p|
    task = TaskContext.get 'abstract_processing_component'
    task.callop("createPort", "myport", "/base/samples/RigidBodyState")
    
    while true
    end
end

