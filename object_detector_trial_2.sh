#!/bin/bash

tmux new-session -d -s real

tmux set -g mouse on

# NAME1
tmux new-window
tmux rename-window 'roscore'
tmux send-keys "ce object_detector" ENTER
tmux send-keys "roslaunch ros_posenet posenet.launch" ENTER
# NAME2
tmux split-window -h
tmux send-keys "ce object_detector" ENTER
tmux send-keys "cd rosbag" ENTER
tmux send-keys "rosbag play 3_no_hand.bag --loop" ENTER
# NAME3
tmux split-window -v
tmux send-keys "ce object_detector" ENTER
tmux send-keys "rosrun object_detector pcl_filter" ENTER
# NAME4
tmux select-pane -L
tmux split-window -v
tmux send-keys "ce object_detector" ENTER
tmux send-keys "rosrun object_detector pcl_crop" ENTER

# NAME5
tmux new-window
tmux send-keys "ce object_detector" ENTER
tmux send-keys "rosrun object_detector inter_marker" ENTER
# NAME6
tmux split-window -h
tmux send-keys "ce object_detector" ENTER
tmux send-keys "rviz -d ~/.rviz/pcl_only.rviz" ENTER
# NAME7
tmux split-window -v
tmux send-keys "ce object_detector" ENTER
tmux send-keys "rviz -d ~/.rviz/marker_only.rviz" ENTER
# NAME8
tmux select-pane -L
tmux split-window -v
tmux send-keys "ce object_detector" ENTER
tmux send-keys "rqt_graph" ENTER

# NAME9
tmux new-window
tmux send-keys "ce object_detector" ENTER
<<<<<<< HEAD
#tmux send-keys "roslaunch ros_posenet posenet.launch" ENTER
=======
tmux send-keys "" ENTER
>>>>>>> github/universal_robot
# NAME10
tmux split-window -h
tmux send-keys "ce object_detector" ENTER
tmux send-keys "cd ~/robot_folders/checkout/object_detector/catkin_ws/src/ros_posenet/rosbag" ENTER
#tmux send-keys "rosbag play 4_hand_left.bag --loop" ENTER
tmux send-keys "rosrun uvc_camera uvc_camera_node _device:=/dev/video0" ENTER
# NAME11
tmux split-window -v
tmux send-keys "ce object_detector" ENTER
tmux send-keys "rosrun ros_posenet listener.py" ENTER
# NAME12
tmux select-pane -L
tmux split-window -v
tmux send-keys "ce object_detector" ENTER
tmux send-keys "rosrun ros_posenet pose_marker" ENTER
# NAME13
tmux select-pane -R
tmux split-window -v
tmux send-keys "ce object_detector" ENTER
tmux send-keys "rviz -d ~/.rviz/posenet_marker.rviz" ENTER


## Start a new line on window 0
tmux send-keys -t real ENTER

tmux attach -t real

