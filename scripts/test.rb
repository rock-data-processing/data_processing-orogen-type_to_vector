require 'readline'
require 'orocos'
include Orocos
Orocos.initialize

ENV['BASE_LOG_LEVEL'] = 'DEBUG'
ENV['BASE_LOG_FORMAT'] = 'SHORT'
ENV['ORO_LOGLEVEL'] = '6'

Orocos.run 'type_to_vector::BaseTask' => 'task' do |p|
    task = TaskContext.get 'task'

    Readline.readline "Press enter to create RigidBodyState port." do
    end

    task.createInputPort("rbs1","/base/samples/RigidBodyState","",0)
    
    Readline.readline "Press enter to create MotionCommand2d port." do
    end

    task.createInputPort("mc2d","/base/MotionCommand2D","",0)

    Readline.readline "Press enter to start task." do
    end

    task.configure
    
    task.start
    
    Readline.readline "Press enter to quit." do
    end
end

