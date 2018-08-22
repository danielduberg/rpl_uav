#!/bin/bash

rosbag_PID=""


handler()
{
    if [ "$rosbag_PID" != "" ]; then
        pkill -INT -P $rosbag_PID
        rosbag_PID=""
    fi
    exit
}

# Trap in case something does CTRL-C
trap handler SIGINT SIGTERM


rosbag record -a &
rosbag_PID=$!
sleep 1

sleep 100

# kill -SIGKILL $rosbag_PID
# killall -s SIGINT record
# killall -s SIGINT rosbag

if [ "$rosbag_PID" != "" ]; then
    pkill -INT -P $rosbag_PID
    rosbag_PID=""
fi


sleep 100