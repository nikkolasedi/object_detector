#include <ros/ros.h>
#include <visualization_msgs/Marker.h>
#include <std_msgs/Float32.h>

//Create a container
std_msgs::Float32 push;

//Store the value to the container
void pcl_callback(const std_msgs::Float32& mval) {
	push = mval;
}

int main( int argc, char** argv )
{
  ros::init(argc, argv, "inter_marker");
  ros::NodeHandle n;
  //Subscribe to PCL_MVal topic
  ros::Subscriber val_sub = n.subscribe ("PCL_MVal", 1, pcl_callback);
  //Create and define a publisher
  ros::Publisher marker_pub = n.advertise<visualization_msgs::Marker>("visualization_marker", 1);
  
  // Set our initial shape type to be a cylinder
  uint32_t shape = visualization_msgs::Marker::CYLINDER;
  
  // Running at 10Hz
  ros::Rate r(10);
  ros::spinOnce();
  
  while (ros::ok())
  {
    visualization_msgs::Marker marker;
    // Set the frame ID and timestamp
    marker.header.frame_id = "/camera5_depth_optical_frame";
    marker.header.stamp = ros::Time::now();

    // Set the namespace and id for this marker.  This serves to create a unique ID
    // Any marker sent with the same namespace and id will overwrite the old one
    marker.ns = "inter_marker";
    marker.id = 0;

    // Set the marker type.  Initially this is CUBE, and cycles between that and SPHERE, ARROW, and CYLINDER
    marker.type = shape;

    // Set the marker action.  Options are ADD, DELETE, and new in ROS Indigo: 3 (DELETEALL)
    marker.action = visualization_msgs::Marker::ADD;

    // Set the pose of the marker.  This is a full 6DOF pose relative to the frame/time specified in the header
    marker.pose.position.x = -1;
    marker.pose.position.y = 0;
    marker.pose.position.z = 0;
    marker.pose.orientation.x = 0.0;
    marker.pose.orientation.y = 0.0;
    marker.pose.orientation.z = 0.0;
    marker.pose.orientation.w = 1.0;

    // Set the scale of the marker -- 1x1x1 here means 1m on a side
    marker.scale.x = 0.5;
    marker.scale.y = 0.5;
    marker.scale.z = 0.1;

    // Set the color -- be sure to set alpha to something non-zero!
    marker.color.r = 0.0f;
    marker.color.g = 1.0f;
    marker.color.b = 0.0f;
    marker.color.a = 1.0;

    marker.lifetime = ros::Duration();
    
    // Change color if having an input
    if(push.data > 10.0){
      marker.color.r = 0.0f;
      marker.color.g = 0.0f;
      marker.color.b = 1.0f; //Color Blue
      marker.color.a = 1.0;
      
      }else{
      marker.color.r = 0.0f;
      marker.color.g = 1.0f; //Color Green
      marker.color.b = 0.0f;
      marker.color.a = 1.0;
      
      }
  
      marker_pub.publish(marker);//Publish the shape

    // Create a text marker
    marker.id = 1;
    marker.ns = " ";
    marker.type = visualization_msgs::Marker::TEXT_VIEW_FACING;
    marker.action = visualization_msgs::Marker::ADD;
    marker.lifetime = ros::Duration();
    marker.pose.position.x = -1.5;
    marker.pose.position.y = 0;
    marker.pose.position.z = 0;
    marker.scale.z = 0.3;
    marker.color.r = 1.0f; // Color white
    marker.color.g = 1.0f;
    marker.color.b = 1.0f;
    marker.color.a = 1.0;
    marker.text = "Waiting for Input"; //Display text
    
      // Change text if receiving an input   
    if(push.data > 10.0){
      
 		  marker.text = "Receive an Input";
    	ROS_INFO("Receive an Input");
      
    	}else{
      
    	marker.text = "Waiting for Input";
    	ROS_INFO("Waiting for Input");
      
    	}
  
    marker_pub.publish(marker);//Publish the text marker
    
    
    ros::spinOnce();
    r.sleep();
		
  }
}
