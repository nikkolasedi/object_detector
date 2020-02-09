#!/bin/bash

tmux new-session -d -s real


tmux set -g mouse on

## Create the windows on which each node or .launch file is going to run
tmux send-keys -t real 'tmux new-window -n roscore ' ENTER
tmux send-keys -t real 'tmux new-window -n rosbag ' ENTER
tmux send-keys -t real 'tmux new-window -n pcl_filter ' ENTER
tmux send-keys -t real 'tmux new-window -n pcl_crop ' ENTER
tmux send-keys -t real 'tmux new-window -n inter_marker ' ENTER
tmux send-keys -t real 'tmux new-window -n rviz_1 ' ENTER
tmux send-keys -t real 'tmux new-window -n rviz_2 ' ENTER
tmux send-keys -t real 'tmux new-window -n rqt ' ENTER

## Send the command to each window from window 0
# NAME1
tmux send-keys -t real "tmux send-keys -t roscore 'ce object_detector' ENTER" ENTER
tmux send-keys -t real "tmux send-keys -t roscore 'roscore' ENTER" ENTER
# NAME2
tmux send-keys -t real "tmux send-keys -t rosbag 'ce object_detector' ENTER" ENTER
tmux send-keys -t real "tmux send-keys -t rosbag 'cd rosbag' ENTER" ENTER
tmux send-keys -t real "tmux send-keys -t rosbag 'rosbag play 3_no_hand.bag --loop ' ENTER" ENTER
# NAME3
tmux send-keys -t real "tmux send-keys -t pcl_filter 'ce object_detector' ENTER" ENTER
tmux send-keys -t real "tmux send-keys -t pcl_filter 'rosrun object_detector pcl_filter' ENTER" ENTER
# NAME4
tmux send-keys -t real "tmux send-keys -t pcl_crop 'ce object_detector' ENTER" ENTER
tmux send-keys -t real "tmux send-keys -t pcl_crop 'rosrun object_detector pcl_crop' ENTER" ENTER
# NAME5
tmux send-keys -t real "tmux send-keys -t inter_marker 'ce object_detector' ENTER" ENTER
tmux send-keys -t real "tmux send-keys -t inter_marker 'rosrun object_detector inter_marker' ENTER" ENTER
# NAME6
tmux send-keys -t real "tmux send-keys -t rviz_1 'ce object_detector' ENTER" ENTER
tmux send-keys -t real "tmux send-keys -t rviz_1 'rviz -d ~/.rviz/pcl_only.rviz' ENTER" ENTER
# NAME7
tmux send-keys -t real "tmux send-keys -t rviz_2 'ce object_detector' ENTER" ENTER
tmux send-keys -t real "tmux send-keys -t rviz_2 'rviz -d ~/.rviz/marker_only.rviz' ENTER" ENTER
# NAME8
tmux send-keys -t real "tmux send-keys -t rqt 'ce object_detector' ENTER" ENTER
tmux send-keys -t real "tmux send-keys -t rqt 'rqt_graph' ENTER" ENTER

## Start a new line on window 0
tmux send-keys -t real ENTER

## Attach to session
tmux send-keys -t real "tmux select-window -t rviz" ENTER
tmux attach -t real
