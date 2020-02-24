#include <ros/ros.h>
#include <visualization_msgs/Marker.h>
#include <std_msgs/Float32.h>
#include <std_msgs/Bool.h>
#include <sstream>
#include <iostream>

//Create a container
std_msgs::Float32 push;

//Create a container
std_msgs::Bool push_loc;
std_msgs::Bool push_robot_running;  

//Create a counter
int counter = 30;
int lcounter;

//State
bool state;
bool state_rob_run;
int var;

//Timer
ros::Time time_last_count;
double count_timeout = 3.0f;

//Set info_offset_x
float info_offset_x = 0.05;

//Set info_offset_y
float info_offset_y = 0.03;

//Set lifetime
double LIFETIME_PERMANENT = 0;
double LIFETIME_LONG = 0.75f;
double LIFETIME_SHORT = 0.50f;

//Color function
std_msgs::ColorRGBA createColorRGBA(float r, float g, float b, float a) {
    std_msgs::ColorRGBA color;
    color.r = r;
    color.g = g;
    color.b = b;
    color.a = a;
    return color;
}
//Set colors
std_msgs::ColorRGBA color_white = createColorRGBA(1.0, 1.0, 1.0, 1.0);
std_msgs::ColorRGBA color_red = createColorRGBA(1.0, 0.0, 0.0, 1.0);
std_msgs::ColorRGBA color_green = createColorRGBA(0.0, 1.0, 0.0, 1.0);
std_msgs::ColorRGBA color_fzi_green = createColorRGBA(0.0, 0.6, 0.2, 1.0);
std_msgs::ColorRGBA color_blue = createColorRGBA(0.0, 0.0, 1.0, 1.0);
std_msgs::ColorRGBA color_black = createColorRGBA(0.0, 0.0, 0.0, 1.0);
std_msgs::ColorRGBA color_yellow = createColorRGBA(1.0, 1.0, 0.0, 1.0);
std_msgs::ColorRGBA color_violet = createColorRGBA(1.0, 0.0, 1.0, 1.0);

//Store the value to the container
void pcl_callback(const std_msgs::Float32& mval) {
	push = mval;
}

void loc_callback(const std_msgs::Bool& mval_loc) {
	push_loc = mval_loc;
}

void countdown(const ros::TimerEvent&){
	
	lcounter = --counter;
	ROS_INFO("Test");
	if(lcounter == 5 && push_loc.data == true){
	state = !state;
	switch(var){
	case 1: 
		var = 2;
		break;
	case 2:
		var = 1;
		break;
	}
	}
	
	if(push.data < 5.0 || push_loc.data != true){state = false;}
	
}



int main( int argc, char** argv )
{
  ros::init(argc, argv, "inter_marker");
  ros::NodeHandle n;
  ros::NodeHandle nh;
  //Subscribe to PCL_MVal topic
  ros::Subscriber val_sub = n.subscribe ("PCL_MVal", 1, pcl_callback);
  //Subscribe to loc_pub topic
  ros::Subscriber loc_sub = n.subscribe ("loc_pub", 1, loc_callback);
  //Create and define a publisher
  ros::Publisher marker_pub = n.advertise<visualization_msgs::Marker>("visualization_marker", 1);
  //Create and define a publisher
  ros::Publisher robot_running_pub = n.advertise<std_msgs::Bool>("robot_running", 1);
  
  state = false;
  var = 1;
  
  
  
  
  // Set our initial shape type to be a cylinder
  uint32_t shape = visualization_msgs::Marker::CYLINDER;
  ros::Timer timer = nh.createTimer(ros::Duration(0.1), countdown);
  // Running at 10Hz
  ros::Rate r(10);
  ros::spinOnce();
  
  while (ros::ok())
  {
		
		// Create a marker
    visualization_msgs::Marker marker;
    int marker_id = 0;
    
    
    // publish TEST Cylinder and publish it by Nikkolas.
 	 	marker.id = marker_id++;
 	 	marker.ns = "inter_marker";
  	marker.type = visualization_msgs::Marker::CYLINDER;
  	marker.action = visualization_msgs::Marker::ADD;
  	marker.header.frame_id = "table";
  	marker.header.stamp = ros::Time();
  	marker.pose.position.x = info_offset_x + 0.40; //was 0.11
  	marker.pose.position.y = 0.50 + info_offset_y; //was 0.68
  	marker.pose.position.z = 0.01;
  	marker.pose.orientation.x = 0.0;
  	marker.pose.orientation.y = 0.0;
  	marker.pose.orientation.z = 0.0;
  	marker.pose.orientation.w = 1.0;

  	marker.scale.x = 0.1;
  	marker.scale.y = 0.1;
  	marker.scale.z = 0.05;
  	marker.color = color_blue;
  
  	marker.lifetime = ros::Duration(LIFETIME_PERMANENT);
  	
  	// Change color if having an input
    
    switch(var){ 
    	case 1: 
    		marker.color = color_red;
    		break;
    	case 2:
    		marker.color = color_blue;
    		break;
    		}
    		
  
		marker_pub.publish(marker);//Publish the shape
  	
  	
//    // Set the frame ID and timestamp
//    marker.header.frame_id = "/camera5_link";
//    marker.header.stamp = ros::Time::now();

//    // Set the namespace and id for this marker.  This serves to create a unique ID
//    // Any marker sent with the same namespace and id will overwrite the old one
//    marker.ns = "inter_marker";
//    marker.id = 0;

//    // Set the marker type.  Initially this is CUBE, and cycles between that and SPHERE, ARROW, and CYLINDER
//    marker.type = shape;

//    // Set the marker action.  Options are ADD, DELETE, and new in ROS Indigo: 3 (DELETEALL)
//    marker.action = visualization_msgs::Marker::ADD;

//    // Set the pose of the marker.  This is a full 6DOF pose relative to the frame/time specified in the header
//    marker.pose.position.x = info_offset_x + 0.40;
//    marker.pose.position.y = 0.50;
//    marker.pose.position.z = 0.01;
//    marker.pose.orientation.x = 0.0;
//    marker.pose.orientation.y = 0.0;
//    marker.pose.orientation.z = 0.0;
//    marker.pose.orientation.w = 1.0;

//    // Set the scale of the marker -- 1x1x1 here means 1m on a side
//    marker.scale.x = 0.1;
//    marker.scale.y = 0.1;
//    marker.scale.z = 0.05;

//    // Set the color -- be sure to set alpha to something non-zero!
//    marker.color = color_blue;

//    marker.lifetime = ros::Duration();
    
    
    


		//Create another button
//		marker.id = marker_id++;
//		marker.ns = "inter_marker";
//    marker.type = visualization_msgs::Marker::CYLINDER;
//  	marker.action = visualization_msgs::Marker::ADD;
//  	marker.header.frame_id = "table";
//  	marker.header.stamp = ros::Time();
//  	marker.pose.position.x = info_offset_x + 0.55; //was 0.11
//  	marker.pose.position.y = 0.50; //was 0.68
//  	marker.pose.position.z = 0.01;
//  	marker.pose.orientation.x = 0.0;
//  	marker.pose.orientation.y = 0.0;
//  	marker.pose.orientation.z = 0.0;
//  	marker.pose.orientation.w = 1.0;

//  	marker.scale.x = 0.1;
//  	marker.scale.y = 0.1;
//  	marker.scale.z = 0.05;
//  	marker.color = color_red;
//  	
//  	// Change color if having an input
//    if(push.data > 5.0){
//      marker.color = color_white;
//      
//      }else{
//      marker.color = color_red;
//      
//      }
//  
//      marker_pub.publish(marker);



    // Create a text marker
    marker.id = marker_id++;
    marker.ns = "inter_marker";
    marker.type = visualization_msgs::Marker::TEXT_VIEW_FACING;
    marker.action = visualization_msgs::Marker::ADD;
    marker.header.frame_id = "table";
    marker.lifetime = ros::Duration();
    marker.pose.position.x = info_offset_x + 0.45;
    marker.pose.position.y = 0.60 + info_offset_y;
    marker.pose.position.z = 0.01;
    marker.scale.z = 0.03;
    marker.color = color_green;
    marker.text = "Waiting for Input"; //Display text
    
      // Change text if receiving an input   
    if(push.data > 5.0 && push_loc.data == true){
      
 		  marker.text = "Receive an Input";
    	ROS_INFO("Receive an Input");
      
    	}else{
      
    	marker.text = "Waiting for Input";
    	ROS_INFO("Waiting for Input");
      
    	}
  
    marker_pub.publish(marker);//Publish the text marker
    
    // Create a text marker on button
    marker.id = marker_id++;
    marker.ns = "inter_marker";
    marker.type = visualization_msgs::Marker::TEXT_VIEW_FACING;
    marker.action = visualization_msgs::Marker::ADD;
    marker.header.frame_id = "table";
    marker.lifetime = ros::Duration();
		marker.pose.position.x = info_offset_x + 0.405; //was 0.11
  	marker.pose.position.y = 0.495 + info_offset_y; //was 0.68
  	marker.pose.position.z = 0.06;
    marker.scale.z = 0.05;
    marker.color = color_black;
    
    
    //Typecasting int to string
    std::string out_string;
		std::stringstream ss;
		
 		//Print count down
    if(push.data > 5.0 && lcounter > 1 && state == false && push_loc.data == true){
    
    	ss.str("");
      ss.clear();
      ss << lcounter/8;
		  out_string = ss.str();
 		  marker.text = out_string;  	  	
    	
    	}else {
    	
    	counter = 30;
    	ss.str("");
      ss.clear();
      ss << lcounter;
      out_string = ss.str();
 		  
 		  switch(var){ 
    	case 1: 
    		marker.text = "OFF";
    		push_robot_running.data = true;
    		break;
    	case 2:
    		marker.text = "ON";
    		push_robot_running.data = false;
    		break;
    		}
 		  
    	}
    	
    	std::cout<<state<<"\n";
    	
    	state_rob_run = push_robot_running.data;
    	std::cout<<"push_robot_running.data ="<<state_rob_run<<".\n";

   	  
    marker_pub.publish(marker);//Publish the text marker
    robot_running_pub.publish(push_robot_running);//Publish the value of push_robot_running
    
    
   // publish TEST Cylinder and publish it by Nikkolas.
 	 	marker.id = marker_id++;
 	 	marker.ns = "inter_marker";
  	marker.type = visualization_msgs::Marker::CYLINDER;
  	marker.action = visualization_msgs::Marker::ADD;
  	marker.header.frame_id = "table";
  	marker.header.stamp = ros::Time();
  	marker.pose.position.x = info_offset_x + 0.40; //was 0.11
  	marker.pose.position.y = 0.50 + info_offset_y; //was 0.68
  	marker.pose.position.z = 0.01;
  	marker.pose.orientation.x = 0.0;
  	marker.pose.orientation.y = 0.0;
  	marker.pose.orientation.z = 0.0;
  	marker.pose.orientation.w = 1.0;
  	float size = 1.0;
  	marker.lifetime = ros::Duration(LIFETIME_PERMANENT);
  	
  	//Print count down
    if(push.data > 5.0 && lcounter > 4 && state == false && push_loc.data == true){
    
    	size = lcounter*0.01*size + size;
    	
    	}else {
    	
 		  size = 1.0;
 		  
    	}
    
    marker.scale.x = size*0.1;
  	marker.scale.y = size*0.1;
  	marker.scale.z = 0.03;
  	marker.color = color_yellow;
    		
  
		marker_pub.publish(marker);//Publish the shape
    
    
    
    
    ros::spinOnce();
    r.sleep();
		
  }
}
