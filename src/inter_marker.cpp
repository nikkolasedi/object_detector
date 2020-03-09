#include <ros/ros.h>
#include <visualization_msgs/Marker.h>
#include <std_msgs/Float32.h>
#include <std_msgs/Bool.h>
#include <sstream>
#include <iostream>
#include <geometry_msgs/Point.h>

//Create a container
std_msgs::Float32 push;

//Create a container
geometry_msgs::Point push_pos;

//Create a container
std_msgs::Bool push_loc;
std_msgs::Bool push_robot_running;  

// Temporary Variable
int temp_var;

// Threshold
int th;

//Timer
ros::Time time_last_count;
double count_timeout = 3.0f;

//Set info_offset_x button 1
float info_offset_x = 0.05;

//Set info_offset_y button 1
float info_offset_y = 0.03;

//Set info_offset_x button 2
float info_offset_x_2 = 0.2;

//Set info_offset_y button 2
float info_offset_y_2 = 0.03;

//Set info_offset_x slider 1
float info_offset_x_3 = 0.3;

//Set info_offset_y slider 1
float info_offset_y_3 = 0.03;

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

void pos_callback(const geometry_msgs::Point& mval_pos) {
	push_pos = mval_pos;
}

class TimeCounter { 
private: 
    int counter, lcounter, var;
    bool state;
  
public: 
    // Parameterized Constructor 
    TimeCounter(int counter1, int lcounter1, int var1, bool state1) 
    { 
        counter = counter1; 
        lcounter = lcounter1; 
        var = var1;
        state = state1;
    } 
  
    int getCounter() 
    { 
        return counter; 
    } 
    int getLcounter() 
    { 
        return lcounter; 
    } 
    int getVar() 
    { 
        return var; 
    } 
    bool getState() 
    { 
        return state; 
    } 
    void setCounter(int x)
    {
        counter = x;
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
	
	if(push.data < 5.0 || push_loc.data != true){
	state = false;
	}
	
}
    
    
    
}; 


int main( int argc, char** argv )
{
  ros::init(argc, argv, "inter_marker");
  ros::NodeHandle n;
  ros::NodeHandle nh;
  //Subscribe to PCL_MVal topic
  ros::Subscriber val_sub = n.subscribe ("PCL_MVal", 1, pcl_callback);
  //Subscribe to loc_pub topic
  ros::Subscriber loc_sub = n.subscribe ("loc_pub", 1, loc_callback);
  //Subscribe to pose_pub topic
  ros::Subscriber pos_sub = n.subscribe ("pose_pub2", 1, pos_callback);
  //Create and define a publisher
  ros::Publisher marker_pub = n.advertise<visualization_msgs::Marker>("visualization_marker", 1);
  //Create and define a publisher
  ros::Publisher robot_running_pub = n.advertise<std_msgs::Bool>("robot_running", 1);
  
  //Timer 1
  TimeCounter t1(30, 1, 1, false);
  
  //Timer 2
  TimeCounter t2(30, 1, 1, false);
  
  // Running at 10Hz
  ros::Timer timer1 = nh.createTimer(ros::Duration(0.1), &TimeCounter::countdown, &t1);
  
  // Running at 10Hz
  ros::Timer timer2 = nh.createTimer(ros::Duration(0.1), &TimeCounter::countdown, &t2);
  
  //Threshold
  th = 20;
  ros::Time time_last;
	bool is_new;
	int pos_last_x;
	int pos_last_y;
	int pos_last_z;
	float speed = 0.0;
  
  // Running at 10Hz
  ros::Rate r(10);
  ros::spinOnce();
  
  while (ros::ok())
  {
		
		// Create a marker
    visualization_msgs::Marker marker;
    int marker_id = 0;
    
    
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
  	marker.color = color_blue;
  
  	marker.lifetime = ros::Duration(LIFETIME_PERMANENT);
  	
  	// Change color if having an input
    
    switch(t1.getVar()){ 
    	case 1: 
    		marker.color = color_red;
    		break;
    	case 2:
    		marker.color = color_blue;
    		break;
    		}
    		
  
		marker_pub.publish(marker);//Publish the shape
  	
  	
    // Create a text marker for Status
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
    
    // Create a text marker on button for the count down
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
    marker.color = color_white;
    
    
    //Typecasting int to string
    std::string out_string;
		std::stringstream ss;
		
 		//Print count down
    if(push.data > 5.0 && t1.getLcounter() > 1 && t1.getState() == false && push_loc.data == true)
    {
    
    	ss.str("");
      ss.clear();
      ss << t1.getLcounter()/8;
		  out_string = ss.str();
 		  marker.text = out_string;  	  	
    	
    	}else {
    	
    	t1.setCounter(30);
    	ss.str("");
      ss.clear();
      ss << t1.getLcounter();
      out_string = ss.str();
 		  
 		  switch(t1.getVar())
 		  	{ 
    	case 1: 
    		marker.text = "OFF";
    		push_robot_running.data = true;

    		break;
    	case 2:
    		marker.text = "ON";
    		push_robot_running.data = false;
    		break;
    	default:
    		marker.text = "Error";
    		}
    		
    	}
    	
    marker_pub.publish(marker);//Publish the text marker
    
		if(temp_var != push_robot_running.data){
    		robot_running_pub.publish(push_robot_running);//Publish the value of push_robot_running
    		std::cout<<"push_robot_running.data ="<<push_robot_running.data<<".\n";
    		}
    		
    std::cout<<t1.getState()<<"\n";
    temp_var = push_robot_running.data;
    
   // publish a Cylinder for animation, when the button pressed
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
    if(push.data > 5.0 && t1.getLcounter() > 4 && t1.getState() == false && push_loc.data == true){
    
    	size = t1.getLcounter()*0.01*size + size;
    	
    	}else {
    	
 		  size = 1.0;
 		  
    	}
    
    marker.scale.x = size*0.1;
  	marker.scale.y = size*0.1;
  	marker.scale.z = 0.03;
  	marker.color = color_yellow;
    		
  
		marker_pub.publish(marker);//Publish the shape
		
		
		
		
		
		////////////////////////////////////////////////////////////////////
		
		/// speed_scaling Button ///
    
    // publish a Cylinder for speed_scaling Button.
 	 	marker.id = marker_id++;
 	 	marker.ns = "inter_marker";
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
  	marker.color = color_blue;
  
  	marker.lifetime = ros::Duration(LIFETIME_PERMANENT);
  	
  	// Change color if having an input
    
    switch(t2.getVar()){ 
    	case 1: 
    		marker.color = color_red;
    		break;
    	case 2:
    		marker.color = color_blue;
    		break;
    		}
    		
  
		marker_pub.publish(marker);//Publish the shape
  	
  	
    // Create a text marker for Status
    marker.id = marker_id++;
    marker.ns = "inter_marker";
    marker.type = visualization_msgs::Marker::TEXT_VIEW_FACING;
    marker.action = visualization_msgs::Marker::ADD;
    marker.header.frame_id = "table";
    marker.lifetime = ros::Duration();
    marker.pose.position.x = info_offset_x_2 + 0.45;
    marker.pose.position.y = 0.60 + info_offset_y_2;
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
    
    marker.id = marker_id++;
    marker.id = marker_id++;
    marker.ns = "inter_marker";
    marker.type = visualization_msgs::Marker::TEXT_VIEW_FACING;
    marker.action = visualization_msgs::Marker::ADD;
    marker.header.frame_id = "table";
    marker.lifetime = ros::Duration();
		marker.pose.position.x = info_offset_x_2 + 0.405; //was 0.11
  	marker.pose.position.y = 0.495 + info_offset_y_2; //was 0.68
  	marker.pose.position.z = 0.06;
    marker.scale.z = 0.05;
    marker.color = color_white;
    
    
    //Typecasting int to string
    std::string out_string_2;
		std::stringstream ss_2;
		
 		//Print count down
    if(push.data > 5.0 && t2.getLcounter() > 1 && t2.getState() == false && push_loc.data == true)
    {
    
    	ss_2.str("");
      ss_2.clear();
      ss_2 << t2.getLcounter()/8;
		  out_string_2 = ss_2.str();
 		  marker.text = out_string_2;  	  	
    	
    	}else {
    	
    	t2.setCounter(30);
    	ss_2.str("");
      ss_2.clear();
      out_string_2 = ss_2.str();
 		  
 		  switch(t2.getVar())
 		  	{ 
    	case 1: 
    		marker.text = "OFF";
//    		push_robot_running.data = true;

    		break;
    	case 2:
    		marker.text = "ON";
//    		push_robot_running.data = false;
    		break;
    	default:
    		marker.text = "Error";
    		}
    		
    	}
    	
    marker_pub.publish(marker);//Publish the text marker
    
   // publish a Cylinder for animation, when the button pressed
 	 	marker.id = marker_id++;
 	 	marker.ns = "inter_marker";
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
  	float size2 = 1.0;
  	marker.lifetime = ros::Duration();
  	
  	//Print count down
    if(push.data > 5.0 && t2.getLcounter() > 4 && t2.getState() == false && push_loc.data == true){
    
    	size2 = t2.getLcounter()*0.01*size2 + size2;
    	
    	}else {
    	
 		  size2 = 1.0;
 		  
    	}
    
    marker.scale.x = size2*0.1;
  	marker.scale.y = size2*0.1;
  	marker.scale.z = 0.03;
  	marker.color = color_yellow;
    		
  
		marker_pub.publish(marker);//Publish the shape
		
		
		// publish a outer Cube for animation for speed scaling
		marker.id = marker_id++;
 	 	marker.ns = "inter_marker";
  	marker.type = visualization_msgs::Marker::CUBE;
  	marker.action = visualization_msgs::Marker::ADD;
  	marker.header.frame_id = "table";
  	marker.header.stamp = ros::Time();
  	marker.pose.position.x = info_offset_x_3 + 0.40; //was 0.11
  	marker.pose.position.y = 0.50 + info_offset_y_3; //was 0.68
  	marker.pose.position.z = 0.01;
  	marker.pose.orientation.x = 0.0;
  	marker.pose.orientation.y = 0.0;
  	marker.pose.orientation.z = 0.0;
  	marker.pose.orientation.w = 1.0;

  	marker.scale.x = 0.02;
  	marker.scale.y = 0.1;
  	marker.scale.z = 0.05;
  	
  
  	marker.lifetime = ros::Duration();
		
		switch(t2.getVar())
 		  	{ 
    	case 1: 
//    		marker.text = "OFF";
		marker.color = color_red;
  	
  	
    		break;
    	case 2:
//    		marker.text = "ON";
		marker.color.a = 0;
    		break;
    	default:
    		marker.text = "Error";
    		}
    		
    marker_pub.publish(marker);//Publish the shape
		
		
		// publish a inner Cube for animation for speed scaling
		marker.id = marker_id++;
 	 	marker.ns = "inter_marker";
  	marker.type = visualization_msgs::Marker::CUBE;
  	marker.action = visualization_msgs::Marker::ADD;
  	marker.header.frame_id = "table";
  	marker.header.stamp = ros::Time();
  	marker.pose.position.x = info_offset_x_3 + 0.40; //was 0.11
  	marker.pose.position.y = 0.50 + info_offset_y_3; //was 0.68
  	marker.pose.position.z = 0.01;
  	marker.pose.orientation.x = 0.0;
  	marker.pose.orientation.y = 0.0;
  	marker.pose.orientation.z = 0.0;
  	marker.pose.orientation.w = 1.0;

  	marker.scale.x = 0.02;
  	
  	marker.scale.z = 0.05;
  	
  	marker.color = color_yellow;
  
  	marker.lifetime = ros::Duration();
		
		if(t2.getState() == true)
		{
		
			if(is_new == true)
			{
			time_last = ros::Time::now();
			pos_last_x = push_pos.x;
			pos_last_y = push_pos.y;
			pos_last_z = push_pos.z;
			is_new = false;
			}
			
			if(pos_last_x < (push_pos.x - th) || pos_last_y < (push_pos.y - th))
			{
				if(speed>0.0)
				{
				speed = speed - 0.1;
				}
			is_new = true;
			}
			if(pos_last_x > (push_pos.x + th) || pos_last_y > (push_pos.y + th))
			{
				if(speed<1.0)
				{
				speed = speed + 0.1;
				}
			is_new = true;
			}
		}
		marker.scale.y = 0.1*speed;
		
		marker_pub.publish(marker);//Publish the shape
		
		
		
		
		
    
    
    ros::spinOnce();
    r.sleep();
		
  }
}
