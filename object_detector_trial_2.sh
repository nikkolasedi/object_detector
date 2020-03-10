#!/bin/bash

SESSIONNAME="object_detector"
ROS_MASTER="ids-horse-01"
has_session=false
rosbag_path="~/robot_folders/checkout/object_detector/catkin_ws/src/ros_posenet/rosbag"
RUN_ROS_IP="export ROS_IP=192.168.1.44"

while getopts "lisr" opt; do
  case ${opt} in
    l )
	tmux new-session -d -s $SESSIONNAME
	has_session=true
	ROS_MASTER="localhost"
	rosbag_path="~/robot_folders-master/checkout/object_detector/catkin_ws/src/ros_posenet/rosbag"
	tmux new-window
	tmux rename-window 'roscore1'
	tmux send-keys "ce object_detector" 'C-m'
	tmux send-keys "export ROS_MASTER_URI=http://${ROS_MASTER}:11311" 'C-m'
#	tmux send-keys 'export ROS_IP=192.168.1.44' 'C-m'
	RUN_ROS_IP=""
	tmux send-keys 'roscore' 'C-m'
      ;;
    i ) 
	tmux new-session -d -s $SESSIONNAME
	has_session=true
      ;;
    s ) 
	sample=true
      ;;
    r ) 
	sample=false
      ;;
    \? ) 
	echo "Usage: sh object_detector_trial_2.sh"
	printf "\t-l\tuse local ROS Master\n"
	printf "\t-l\tuse ROS Master from ids-horse-01\n"
	printf "\t-?\tshow this usage information\n"
	exit
      ;;
  esac
done

if [ $has_session = true ] 
then
tmux set -g mouse on
# NAME1
tmux new-window
tmux rename-window 'roscore'
tmux send-keys "ce object_detector" ENTER
tmux send-keys "export ROS_MASTER_URI=http://${ROS_MASTER}:11311" ENTER
tmux send-keys '${RUN_ROS_IP}' ENTER
tmux send-keys "roslaunch ros_posenet posenet.launch" ENTER
# NAME2
tmux split-window -h
tmux send-keys "ce object_detector" ENTER
tmux send-keys "cd rosbag" ENTER
tmux send-keys "export ROS_MASTER_URI=http://${ROS_MASTER}:11311" ENTER
tmux send-keys '${RUN_ROS_IP}' ENTER
tmux send-keys 'roslaunch realsense2_camera rs_rgbd_xyz.launch camera:=camera5 enable_color:=true' ENTER

if [ $sample = true ] 
then
tmux send-keys "rosbag play 3_no_hand.bag --loop" ENTER
#tmux send-keys 'C-c' ENTER
fi

# NAME3
tmux split-window -v
tmux send-keys "ce object_detector" ENTER
tmux send-keys "export ROS_MASTER_URI=http://${ROS_MASTER}:11311" ENTER
tmux send-keys '${RUN_ROS_IP}' ENTER
tmux send-keys "rosrun object_detector pcl_filter" ENTER

# NAME4
tmux select-pane -L
tmux split-window -v
tmux send-keys "ce object_detector" ENTER
tmux send-keys "export ROS_MASTER_URI=http://${ROS_MASTER}:11311" ENTER
tmux send-keys '${RUN_ROS_IP}' ENTER
tmux send-keys "rosrun object_detector pcl_crop" ENTER

# NAME5
tmux new-window
tmux send-keys "ce object_detector" ENTER
tmux send-keys "export ROS_MASTER_URI=http://${ROS_MASTER}:11311" ENTER
tmux send-keys '${RUN_ROS_IP}' ENTER
tmux send-keys "rosrun object_detector inter_marker" ENTER
# NAME6
tmux split-window -h
tmux send-keys "ce object_detector" ENTER
tmux send-keys "export ROS_MASTER_URI=http://${ROS_MASTER}:11311" ENTER
tmux send-keys '${RUN_ROS_IP}' ENTER
tmux send-keys "rviz -d ~/.rviz/pcl_only.rviz" ENTER
# NAME7
tmux split-window -v
tmux send-keys "ce object_detector" ENTER
tmux send-keys "export ROS_MASTER_URI=http://${ROS_MASTER}:11311" ENTER
tmux send-keys '${RUN_ROS_IP}' ENTER
tmux send-keys "rviz -d ~/.rviz/marker_only.rviz" ENTER
# NAME8
tmux select-pane -L
tmux split-window -v
tmux send-keys "ce object_detector" ENTER
tmux send-keys "export ROS_MASTER_URI=http://${ROS_MASTER}:11311" ENTER
tmux send-keys '${RUN_ROS_IP}' ENTER
tmux send-keys "rqt_graph" ENTER

# NAME9
tmux new-window
tmux send-keys "ce object_detector" ENTER
#<<<<<<< HEAD
#<<<<<<< HEAD
#tmux send-keys "roslaunch ros_posenet posenet.launch" ENTER
#=======
#=======
tmux send-keys "export ROS_MASTER_URI=http://${ROS_MASTER}:11311" ENTER
tmux send-keys '${RUN_ROS_IP}' ENTER
#>>>>>>> github/universal_robot
tmux send-keys "" ENTER
#>>>>>>> github/universal_robot

# NAME10
tmux split-window -h
tmux send-keys "ce object_detector" ENTER
tmux send-keys "export ROS_MASTER_URI=http://${ROS_MASTER}:11311" ENTER
tmux send-keys '${RUN_ROS_IP}' ENTER
tmux send-keys "cd ${rosbag_path}" ENTER
if [ $sample = true ] 
then
tmux send-keys "rosbag play 4_hand_left.bag --loop" ENTER
else
tmux send-keys "rosrun uvc_camera uvc_camera_node _device:=/dev/video0" ENTER
fi

# NAME11
tmux split-window -v
tmux send-keys "ce object_detector" ENTER
tmux send-keys "export ROS_MASTER_URI=http://${ROS_MASTER}:11311" ENTER
tmux send-keys '${RUN_ROS_IP}' ENTER
tmux send-keys "rosrun ros_posenet listener.py" ENTER
# NAME12
tmux select-pane -L
tmux split-window -v
tmux send-keys "ce object_detector" ENTER
tmux send-keys "export ROS_MASTER_URI=http://${ROS_MASTER}:11311" ENTER
tmux send-keys '${RUN_ROS_IP}' ENTER
tmux send-keys "rosrun ros_posenet pose_marker" ENTER
# NAME13
tmux select-pane -R
tmux split-window -v
tmux send-keys "ce object_detector" ENTER
tmux send-keys "export ROS_MASTER_URI=http://${ROS_MASTER}:11311" ENTER
tmux send-keys '${RUN_ROS_IP}' ENTER
tmux send-keys "rviz -d ~/.rviz/posenet_marker.rviz" ENTER
fi
## Start a new line on window 0
#tmux send-keys -t real ENTER

#tmux attach -t real
tmux attach -t $SESSIONNAME
