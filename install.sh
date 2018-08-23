# Mavros, Mavlink, and PX4
wstool init src
sudo apt-get install python-catkin-tools python-rosinstall-generator -y
wstool init ~/rpl_ws/src
wstool update -t src -j4
rosdep install --from-paths src --ignore-src -y
source ./src/mavros/mavros/scripts/install_geographiclib_datasets.sh

cd ~/rpl_ws/src/px4
make posix_sitl_default gazebo
