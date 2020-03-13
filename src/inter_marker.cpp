#include <ros/ros.h>
#include <visualization_msgs/Marker.h>
#include <std_msgs/Float32.h>
#include <std_msgs/Float64.h>
#include <std_msgs/Bool.h>
#include <sstream>
#include <iostream>
#include <geometry_msgs/Point.h>
#include <std_srvs/Trigger.h>


//Create a container
geometry_msgs::Point push_pos_loc;

//Create a container
bool push_pcl_1;
bool push_pcl_2;
bool push_pos_1;
bool push_pos_2;
bool release_1;
bool release_2;
std_msgs::Bool push_robot_running;
std_msgs::Float64 push_speed_factor;
std_srvs::Trigger srv;

//Subscribe to PCL_MVal topic
  ros::Subscriber pcl_sub_1;
  ros::Subscriber pcl_sub_2; 
  //Subscribe to loc_pub topic
  ros::Subscriber pos_sub_1;
  ros::Subscriber pos_sub_2;
  //Subscribe to pose_pub topic
  ros::Subscriber pos_loc_sub;
  //Create and define a publisher
  ros::Publisher marker_pub; 
  //Create and define a publisher
  ros::Publisher robot_running_pub; 
  //Create and define a publisher
  ros::Publisher speed_factor_pub; 
  //Service call
  ros::ServiceClient client;

// Temporary Variable
int var_1;
int var_2;
bool temp_var;
bool new_var;
bool is_new;
float speed;

// Threshold
int th;

double count_timeout = 3.0f;

//Set info_offset_x button 1
float info_offset_x = -0.18;

//Set info_offset_y button 1
float info_offset_y = -0.3;

//Set info_offset_x button 2
float info_offset_x_2 = -0.06;

//Set info_offset_y button 2
float info_offset_y_2 = -0.3;

//Set info_offset_x slider 1
float info_offset_x_3 = -0.07;

//Set info_offset_y slider 1
float info_offset_y_3 = -0.13;

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



void pcl_callback_1(const std_msgs::Bool& mval) {
	push_pcl_1 = mval.data;
}

void pcl_callback_2(const std_msgs::Bool& mval) {
	push_pcl_2 = mval.data;
}

void pos_callback_1(const std_msgs::Bool& mval) {
	push_pos_1 = mval.data;
}

void pos_callback_2(const std_msgs::Bool& mval) {
	push_pos_2 = mval.data;
}

void pos_loc_callback(const geometry_msgs::Point& mval) {
	push_pos_loc = mval;
}

bool compare(bool a, bool b) {
	if( a == true && b == true ){
	return true;
	}else{
	return false;
	}
}


void publishInformation()
{
    int marker_id = 0;
    
    visualization_msgs::Marker marker;
    
    //std_srvs::Trigger srv;
    
    /// robot_running Button ///
    
    // publish a Cylinder for ON/OFF Button.
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
  
  	marker.lifetime = ros::Duration(LIFETIME_PERMANENT);
  	
  	// Change color if having an input
  	
    if (compare(push_pcl_1, push_pos_1) && release_1 ){
    
    switch(var_1){ 
    	case 1:
    		var_1 = 2;
    		break;
    	case 2:
    		var_1 = 1;
    		break;
    		}
    		
    	release_1 = false;
    	
    	}
    	
    	switch(var_1){ 
    	case 1: 
    		marker.color = color_red;
    		break;
    	case 2:
    		marker.color = color_blue;
    		break;
    		}
    		
    		
    if (compare(push_pcl_1, push_pos_1) == false )
    {
       release_1 = true;
    }
    	
		marker_pub.publish(marker);//Publish the shape
  	
  	
    // Create a text marker for Status
    marker.id = marker_id++;
    marker.type = visualization_msgs::Marker::TEXT_VIEW_FACING;
    marker.action = visualization_msgs::Marker::ADD;
    marker.header.frame_id = "table";
    marker.lifetime = ros::Duration();
    marker.pose.position.x = info_offset_x + 0.38;
    marker.pose.position.y = info_offset_y + 0.60;
    marker.pose.position.z = 0.01;
    marker.scale.z = 0.02;
    marker.color = color_green;
    
      // Change text if receiving an input   

      switch(var_1){ 
    	case 1: 
    		marker.text = "Press\nto stop";
    		//ROS_INFO("Press to stop");
    		break;
    	case 2:
    		marker.text = "Press\nto start";
    		//ROS_INFO("Press to start");
    		break;
    		}

  
    marker_pub.publish(marker);//Publish the text marker
    
    // Create a text marker on button for the count down
    marker.id = marker_id++;
    marker.type = visualization_msgs::Marker::TEXT_VIEW_FACING;
    marker.action = visualization_msgs::Marker::ADD;
    marker.header.frame_id = "table";
    marker.lifetime = ros::Duration();
		marker.pose.position.x = info_offset_x + 0.408; //was 0.11
  	marker.pose.position.y = 0.495 + info_offset_y; //was 0.68
  	marker.pose.position.z = 0.06;
    marker.scale.z = 0.05;
    marker.color = color_white;
    
    
    //Typecasting int to string
    std::string out_string;
		std::stringstream ss;
		
 		//Print count down

 		  switch(var_1)
 		  	{ 
    	case 1: 
    		marker.text = "OFF";
    		push_robot_running.data = true;
    		new_var = true;
    		break;
    	case 2:
    		marker.text = "ON";
    		push_robot_running.data = false;
    		new_var = false;
    		break;
    	default:
    		marker.text = "Error";
    		}

    	
    marker_pub.publish(marker);//Publish the text marker
    /*
		if(temp_var != push_robot_running.data){
		
		switch(var_1)
 		  	{ 
    	case 1: 
    		client = n.serviceClient<std_srvs::Trigger>("/ur_hardware_interface/dashboard/play");
   		
    		break;
    	case 2:
    		client = n.serviceClient<std_srvs::Trigger>("/ur_hardware_interface/dashboard/pause");
    		break;
    		}
    		
    if (client.call(srv))
  			{
    		ROS_INFO("success: %d \n message: %s", srv.response.success, srv.response.message);
  			}
  			else
  			{
    		ROS_ERROR("Failed to call service");
  			}
  			
		    temp_var = push_robot_running.data;
    		robot_running_pub.publish(push_robot_running);//Publish the value of push_robot_running
    		std::cout<<"push_robot_running.data ="<<temp_var<<".\n";
    		}	
    */
    
   // publish a Cylinder for animation, when the button pressed
 	 	marker.id = marker_id++;
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
  	float size = 0.05;
  	marker.lifetime = ros::Duration(LIFETIME_PERMANENT);
  	
  	//Print count down
    if(compare(push_pcl_1, push_pos_1)){
    
    	size = 0.13;
    	
    	}else {
    	
 		  size = 0.105;
 		  
    	}
    
    marker.scale.x = size;
  	marker.scale.y = size;
  	marker.scale.z = 0.03;
  	marker.color = color_yellow;
    		
  
		marker_pub.publish(marker);//Publish the shape
		
		
		
		
		
		////////////////////////////////////////////////////////////////////
		
		/// speed_scaling Button ///
    
    // publish a Cylinder for speed_scaling Button.
 	 	marker.id = marker_id++;
  	marker.type = visualization_msgs::Marker::CYLINDER;
  	marker.action = visualization_msgs::Marker::ADD;
  	marker.header.frame_id = "table";
  	marker.header.stamp = ros::Time();
  	marker.pose.position.x = info_offset_x_2 + 0.40; //was 0.11
  	marker.pose.position.y = 0.50 + info_offset_y_2; //was 0.68
  	marker.pose.position.z = 0.01;
  	marker.pose.orientation.x = 0.0;
  	marker.pose.orientation.y = 0.0;
  	marker.pose.orientation.z = 0.0;
  	marker.pose.orientation.w = 1.0;

  	marker.scale.x = 0.1;
  	marker.scale.y = 0.1;
  	marker.scale.z = 0.05;
  
  	marker.lifetime = ros::Duration(LIFETIME_PERMANENT);
  	
  	// Change color if having an input
    if (compare(push_pcl_2, push_pos_2) && release_2)
    {
    
    switch(var_2){ 
    	case 1:
    		var_2 = 2;
    		break;
    	case 2:
    		var_2 = 1;
    		break;
    		}
    		
    release_2 = false;
    	
    }
    
    switch(var_2){ 
    	case 1: 
    		marker.color = color_red;
    		break;
    	case 2:
    		marker.color = color_blue;
    		break;
    		}
    	
    
    	
    if (compare(push_pcl_2, push_pos_2) == false)
    {
    	release_2 = true;
    }
  
		marker_pub.publish(marker);//Publish the shape
  	
  	
    // Create a text marker for Status
    marker.id = marker_id++;
    marker.type = visualization_msgs::Marker::TEXT_VIEW_FACING;
    marker.action = visualization_msgs::Marker::ADD;
    marker.header.frame_id = "table";
    marker.lifetime = ros::Duration();
    marker.pose.position.x = info_offset_x_2 + 0.39;
    marker.pose.position.y = 0.60 + info_offset_y_2;
    marker.pose.position.z = 0.01;
    marker.scale.z = 0.02;
    marker.color = color_green;
    marker.text = "Press to\nchange speed"; //Display text
    
      // Change text if receiving an input   
    if(compare(push_pcl_2, push_pos_2)){
      
 		  marker.text = "Release to\nset the speed";
    	//ROS_INFO("Release to set the speed");
      
    	}else{
      
    	marker.text = "Press to\nchange speed";
    	//ROS_INFO("Press to change speed");
      
    	}
  
    marker_pub.publish(marker);//Publish the text marker

    marker.id = marker_id++;
    marker.type = visualization_msgs::Marker::TEXT_VIEW_FACING;
    marker.action = visualization_msgs::Marker::ADD;
    marker.header.frame_id = "table";
    marker.lifetime = ros::Duration();
		marker.pose.position.x = info_offset_x_2 + 0.408; //was 0.11
  	marker.pose.position.y = 0.495 + info_offset_y_2; //was 0.68
  	marker.pose.position.z = 0.06;
    marker.scale.z = 0.05;
    marker.color = color_white;
    
    
    //Typecasting int to string
    std::string out_string_2;
		std::stringstream ss_2;
		
 		//Print count down
    if(compare(push_pcl_2, push_pos_2))
    {
 		  
 		  switch(var_2)
 		  	{ 
    	case 1: 
    		marker.text = "OFF";
    		break;
    	case 2:
    		marker.text = "ON";
    		break;
    		}
    		
    }
    marker.text = ">>";
    	
    marker_pub.publish(marker);//Publish the text marker
    
   // publish a Cylinder for animation, when the button pressed
 	 	marker.id = marker_id++;
  	marker.type = visualization_msgs::Marker::CYLINDER;
  	marker.action = visualization_msgs::Marker::ADD;
  	marker.header.frame_id = "table";
  	marker.header.stamp = ros::Time();
  	marker.pose.position.x = info_offset_x_2 + 0.40; //was 0.11
  	marker.pose.position.y = 0.50 + info_offset_y_2; //was 0.68
  	marker.pose.position.z = 0.01;
  	marker.pose.orientation.x = 0.0;
  	marker.pose.orientation.y = 0.0;
  	marker.pose.orientation.z = 0.0;
  	marker.pose.orientation.w = 1.0;
  	float size2 = 0.05;
  	marker.lifetime = ros::Duration(LIFETIME_PERMANENT);
  	
  	//Print count down
    if(compare(push_pcl_2, push_pos_2)){
    
    	size2 = 0.13;
    	
    	}else {
    	
 		  size2 = 0.105;
 		  
    	}
    
    marker.scale.x = size2;
  	marker.scale.y = size2;
  	marker.scale.z = 0.03;
  	marker.color = color_yellow;
    		
  
		marker_pub.publish(marker);//Publish the shape
		
		/*
		// publish a outer Cube for animation for speed scaling
		marker.id = marker_id++;
  	marker.type = visualization_msgs::Marker::CUBE;
  	marker.action = visualization_msgs::Marker::ADD;
  	marker.header.frame_id = "table";
  	marker.header.stamp = ros::Time();
  	marker.pose.position.x = info_offset_x_3 + 0.40; //was 0.11
  	marker.pose.position.y = 0.48 + info_offset_y_3; //was 0.68
  	marker.pose.position.z = 0.005;
  	marker.pose.orientation.x = 0.0;
  	marker.pose.orientation.y = 0.0;
  	marker.pose.orientation.z = 0.0;
  	marker.pose.orientation.w = 1.0;


  	marker.scale.x = 0.1;
  	marker.scale.y = 0.02;
  	marker.scale.z = 0.05;
  	
  
  	marker.lifetime = ros::Duration(LIFETIME_PERMANENT);
		
		switch(var_2)
 		  	{ 
    	case 1: 
//    	marker.text = "OFF";
		marker.color = color_red;
    		break;
    	case 2:
//    		marker.text = "ON";
		marker.color = color_black;
    		break;
    		}
    		
    marker_pub.publish(marker);//Publish the shape
		
		*/
		// publish a inner Cube for animation for speed scaling
		marker.id = marker_id++;
  	marker.type = visualization_msgs::Marker::CUBE;
  	marker.action = visualization_msgs::Marker::ADD;
  	marker.header.frame_id = "table";
  	marker.header.stamp = ros::Time();
  	marker.pose.position.x = info_offset_x_3 + 0.40; //was 0.11
  	marker.pose.position.y = 0.48 + info_offset_y_3; //was 0.68
  	marker.pose.position.z = 0.01;
  	marker.pose.orientation.x = 0.0;
  	marker.pose.orientation.y = 0.0;
  	marker.pose.orientation.z = 0.0;
  	marker.pose.orientation.w = 1.0;

  	marker.scale.y = 0.02;
  	marker.scale.z = 0.05;
  	
  
  	marker.lifetime = ros::Duration(LIFETIME_PERMANENT);
  	/*
  	int pos_last_y;
		
		if(compare(push_pcl_2, push_pos_2))
		{
		  marker.color = color_yellow;
			if(is_new == true)
			{
			  pos_last_y = push_pos_loc.y;
			  std::cout<<pos_last_y<<"\n";
			  is_new = false;
			}
			
			if(push_pos_loc.y < (pos_last_y-th) && is_new == false)
			{
			
				speed = speed - 1;
			  std::cout<<push_pos_loc.y - th<<"\n------\n";	
			  is_new = true;
			  if(speed<2)
				{
				1;
				}
				
			}else if(push_pos_loc.y > (pos_last_y+th) && is_new == false)
			{
				
				speed = speed + 1;
			  is_new = true;
			  std::cout<<push_pos_loc.y + th<<"\n+++++++++\n";
			  if(speed>10)
				{
				speed = 10;
				}
			}else{
			
			}
		}
		else 
		{
		marker.color = color_red;
		}
		*/
		
		if(compare(push_pcl_2, push_pos_2))
		{ 
		
			//ros::param::set("~scaling_enabled", true);
			
			if(push_pos_loc.y > 330 && speed > 2){
			
			speed = speed - 0.5;
			
			}
			if(push_pos_loc.y < 270 && speed < 10){
			
			speed = speed + 0.5; 
			
			}
			
			
		}else{
		
			//ros::param::set("~scaling_enabled", false);
			
		}
		
		if(speed>0 && speed<10){
		
		marker.scale.x = 0.01*speed;
		push_speed_factor.data = speed*0.1;
		
		if(compare(push_pcl_2, push_pos_2)){
			
			std::cout<< speed*0.1 << "\n";
			speed_factor_pub.publish(push_speed_factor);//Publish the speed_factor
			
			}
		
		}else{
		
		speed = 10;
		marker.scale.x = 0.1;
		
		}
		
		
		marker_pub.publish(marker);//Publish the shape
		
	
		// Create a text marker for speed
    marker.id = marker_id++;
    marker.type = visualization_msgs::Marker::TEXT_VIEW_FACING;
    marker.action = visualization_msgs::Marker::ADD;
    marker.header.frame_id = "table";
		marker.pose.position.x = info_offset_x_3 + 0.39; //was 0.11
  	marker.pose.position.y = 0.52 + info_offset_y_3; //was 0.68
  	marker.pose.position.z = 0.01;
    marker.scale.z = 0.05;
    marker.color = color_white;
    marker.lifetime = ros::Duration(LIFETIME_PERMANENT);
    
    
    //Typecasting int to string
    std::string out_string_3;
		std::stringstream ss_3;
		
 		//Print speed
    ss_3.str("");
    ss_3.clear();
    ss_3 << speed*10;
		out_string_3 = ss_3.str();
 		marker.text = out_string_3 + "%";  	  	
     	
    marker_pub.publish(marker);//Publish the text marker
		
}



int main( int argc, char** argv )
{
  ros::init(argc, argv, "inter_marker");
  ros::NodeHandle n;
  //Subscribe to PCL_MVal topic
  pcl_sub_1 = n.subscribe ("PCL_MVal_1", 1, pcl_callback_1);
  pcl_sub_2 = n.subscribe ("PCL_MVal_2", 1, pcl_callback_2);
  //Subscribe to loc_pub topic
  pos_sub_1 = n.subscribe ("loc_pub_1", 1, pos_callback_1);
  pos_sub_2 = n.subscribe ("loc_pub_2", 1, pos_callback_2);
  //Subscribe to pose_pub topic
  pos_loc_sub = n.subscribe ("pose_pub2", 1, pos_loc_callback);
  //Create and define a publisher
  marker_pub = n.advertise<visualization_msgs::Marker>("visualization_marker", 1);
  //Create and define a publisher
  robot_running_pub = n.advertise<std_msgs::Bool>("robot_running", 1);
  //Create and define a publisher
  speed_factor_pub = n.advertise<std_msgs::Float64>("speed_factor", 1);
  //Service call
  

  var_1 = 1;
  var_2 = 1;
  release_1 = true;
  release_2 = true;
  is_new = true;
  speed = 10;
  th = 50;
  temp_var = true;
  
  
  ros::Rate loop_rate(15);
  
  while (ros::ok())
  {
    // receive callbacks
    ros::spinOnce();
    
    publishInformation();
    
    if(temp_var != new_var){
		
		switch(var_1)
 		  	{ 
    	case 1: 
    		client = n.serviceClient<std_srvs::Trigger>("/ur_hardware_interface/dashboard/play");
    		break;
    		
    	case 2:
    		client = n.serviceClient<std_srvs::Trigger>("/ur_hardware_interface/dashboard/pause");
    		break;
    		
    		}
    		
    if (client.call(srv))
  			{
    		ROS_INFO("success: %d \n message: %s", srv.response.success, srv.response.message);
  			}
  			else
  			{
    		ROS_ERROR("Failed to call service");
  			}
  			std::cout<<"temp_var: "<<temp_var<<"\n";
  			std::cout<<"temp_var: "<<new_var<<"\n";
  			
		    temp_var = new_var;
    		//robot_running_pub.publish(push_robot_running);//Publish the value of push_robot_running
    		//std::cout<<"push_robot_running.data ="<<temp_var<<".\n";
    		}	

    loop_rate.sleep();
    ros::spinOnce();
  }

  
}
