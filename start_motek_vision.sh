#!/bin/bash

SESSIONNAME="shared_workspace_demo"
tmux has-session -t $SESSIONNAME &> /dev/null
has_session=$?

ROBOT_ENV_VISION="motek_vision" 

DESCRIPTION="description_motek"
FTS_DEMO="fts_demo_motek"
SHARED_WS_NODE="shared_workspace_motek"

ROS_MASTER="ids-horse-01"

function show_help()
{
  echo "Usage: rosrun shared_workspace_launch $(basename $0) [-e <robot_folders_environment>] [-f] [-b <quoted_string>]"
  echo "Options:"
  printf "\t-e\tchange to this robot folders environment, default ${ROBOT_ENV}\n"
  printf "\t-l\tuse local ROS master\n"
  printf "\t-?\tshow this usage information\n"
}

while getopts ":e:b:l" opt; do
  case ${opt} in
    b ) # process option b
      echo "launching behavior ${OPTARG}"
      BEHAVIOR=${OPTARG}
      ;;
    e ) # process option e
      echo "changing to environment ${OPTARG}"
      ROBOT_ENV_VISION="${OPTARG}"
      ;;
    l ) # process option l
      echo "setting ROS_MASTER_URI to localhost"
      ROS_MASTER="localhost"
	  tmux new-window
	  tmux rename-window 'roscore'
	  tmux send-keys "fzirob change_environment ${ROBOT_ENV_VISION}" 'C-m'
	  tmux send-keys "export ROS_MASTER_URI=http://${ROS_MASTER}:11311" 'C-m'
	  tmux send-keys 'export ROS_IP=192.168.1.44' 'C-m'
	  tmux send-keys 'roscore' 'C-m'
      ;;
    \? ) show_help
      exit
      ;;
  esac
done

if [[ $has_session -eq 1 ]]; then
  tmux new-session -s $SESSIONNAME -d
  tmux set -g mouse on

  # Highlight active window in tab-bar at bottom in red
  tmux set-window-option -g window-status-current-bg red
  tmux set-option -g pane-active-border-fg red

  tmux set -g window-style 'fg=colour247,bg=colour236'
  tmux set -g window-active-style 'fg=colour250,bg=black'

  # Use Alt-arrow keys without prefix key to switch panes
  tmux bind -n M-Left select-pane -L
  tmux bind -n M-Right select-pane -R
  tmux bind -n M-Up select-pane -U
  tmux bind -n M-Down select-pane -D

  # Shift arrow to switch windows
  tmux bind -n C-Pageup  previous-window
  tmux bind -n C-Pagedown next-window

  tmux bind -n M-o split-window -h
  tmux bind -n M-e split-window -v

  # Everything on the shuttle goes here
  tmux new-window
  tmux rename-window 'camera'
  tmux send-keys "fzirob change_environment ${ROBOT_ENV_VISION}" 'C-m'
  tmux send-keys "export ROS_MASTER_URI=http://${ROS_MASTER}:11311" 'C-m'
  tmux send-keys 'export ROS_IP=192.168.1.44' 'C-m'
  tmux send-keys 'roscd motek_gripper' 'C-m'
  tmux send-keys 'roslaunch pylon_camera pylon_camera_node.launch config_file:=etc/pylon_config.yaml' 'C-m'

  tmux new-window
  tmux rename-window 'homography'
  tmux send-keys "fzirob change_environment ${ROBOT_ENV_VISION}" 'C-m'
  tmux send-keys "export ROS_MASTER_URI=http://${ROS_MASTER}:11311" 'C-m'
  tmux send-keys 'export ROS_IP=192.168.1.44' 'C-m'
  tmux send-keys 'roscd motek_gripper' 'C-m'
  #tmux send-keys 'rosparam set /no_gui "true"' 'C-m'
  tmux send-keys 'rosrun homography homography homography.yml _show_gui:=false' 'C-m'

  tmux new-window
  tmux rename-window 'gripper_detector'
  tmux send-keys "fzirob change_environment ${ROBOT_ENV_VISION}" 'C-m'
  tmux send-keys "export ROS_MASTER_URI=http://${ROS_MASTER}:11311" 'C-m'
  tmux send-keys 'export ROS_IP=192.168.1.44' 'C-m'
  #tmux send-keys 'rosrun motek_gripper gripper_detector.py _show_gui:=false' 'C-m'
  tmux send-keys 'rosrun motek_gripper gripper_detector.py _show_gui:=true' 'C-m'

  tmux new-window
  tmux rename-window 'spare'
  tmux send-keys "fzirob change_environment ${ROBOT_ENV_VISION}" 'C-m'
fi
tmux attach -t $SESSIONNAME
