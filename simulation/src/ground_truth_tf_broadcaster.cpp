#include <ros/ros.h>

#include <nav_msgs/Odometry.h>

#include <tf2/utils.h>
#include <tf2_ros/transform_broadcaster.h>
#include "tf2_geometry_msgs/tf2_geometry_msgs.h"

std::string frame_id_;
std::string child_frame_id_;

void odomCallback(nav_msgs::Odometry::ConstPtr const &odom)
{
	static tf2_ros::TransformBroadcaster br;

	geometry_msgs::TransformStamped transform;
	transform.header = odom->header;
	transform.header.frame_id = frame_id_;
	transform.child_frame_id = child_frame_id_;
	transform.transform.translation.x = odom->pose.pose.position.x;
	transform.transform.translation.y = odom->pose.pose.position.y;
	transform.transform.translation.z = odom->pose.pose.position.z;
	transform.transform.rotation = odom->pose.pose.orientation;
	br.sendTransform(transform);
}

int main(int argc, char **argv)
{
	ros::init(argc, argv, "ground_truth_tf_broadcaster");

	ros::NodeHandle nh;
	ros::NodeHandle nh_priv("~");

	frame_id_ = nh_priv.param<std::string>("frame_id", "map");
	child_frame_id_ = nh_priv.param<std::string>("child_frame_id", "base_link");

	ros::Subscriber sub = nh.subscribe("base_link_ground_truth", 1, &odomCallback);

	ros::spin();
	return 0;
}