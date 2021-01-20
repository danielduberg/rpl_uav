#include <ros/ros.h>

#include <geometry_msgs/PoseStamped.h>
#include <nav_msgs/Odometry.h>

#include <tf2/utils.h>
#include <tf2_ros/transform_broadcaster.h>
#include "tf2_geometry_msgs/tf2_geometry_msgs.h"

std::string map_frame_id;
std::string stabililzed_frame_id;
std::string robot_frame_id;

void odomCallback(nav_msgs::Odometry::ConstPtr const &odom)
{
	static tf2_ros::TransformBroadcaster br;

	geometry_msgs::TransformStamped transform;
	transform.header = odom->header;
	transform.header.frame_id = map_frame_id;
	transform.child_frame_id = stabililzed_frame_id;
	transform.transform.translation.x = odom->pose.pose.position.x;
	transform.transform.translation.y = odom->pose.pose.position.y;
	transform.transform.translation.z = odom->pose.pose.position.z;
  // TODO: Figure out if something below can work
  // tf2::convert(pose.pose.position, transform.transform.translation); 
  tf2::Quaternion q;
  q.setRPY(0, 0, tf2::getYaw(odom->pose.pose.orientation));
  tf2::convert(q, transform.transform.rotation);

  geometry_msgs::TransformStamped transform_2;
  transform_2.header.stamp = transform.header.stamp;
  transform_2.header.frame_id = stabililzed_frame_id;
  transform_2.child_frame_id = robot_frame_id;
  double roll, pitch, yaw;
  tf2::getEulerYPR(odom->pose.pose.orientation, yaw, pitch, roll);
  q.setRPY(roll, pitch, 0);
  tf2::convert(q, transform_2.transform.rotation);

  br.sendTransform(transform);
  br.sendTransform(transform_2);
}

int main(int argc, char** argv)
{
  ros::init(argc, argv, "stabilized_transform_publisher");

  ros::NodeHandle nh;
  ros::NodeHandle nh_priv("~");

	map_frame_id = nh_priv.param<std::string>("map_frame_id", "map");
  stabililzed_frame_id = nh_priv.param<std::string>("stabilized_frame_id", "base_stabilized_link");
  robot_frame_id = nh_priv.param<std::string>("robot_frame_id", "base_link");

	ros::Subscriber sub = nh.subscribe("odom", 1, &odomCallback);

  ros::spin();
  return 0;
}