#include <ros/ros.h>
#include <pcl_ros/point_cloud.h>
#include <pcl/point_types.h>
#include <boost/foreach.hpp>
#include <pcl/filters/crop_box.h>
#include <sensor_msgs/PointCloud2.h>
#include <sensor_msgs/point_cloud2_iterator.h>
#include <std_msgs/Float32.h>
#include <std_msgs/Bool.h>

 // Create publisher
 ros::Publisher pub_1; 	        //for PCL
 ros::Publisher pub_2;
 ros::Publisher pcl_mval_pub_1; 	//for the number of PCL
 ros::Publisher pcl_mval_pub_2;
 std_msgs::Bool state_1;
 std_msgs::Bool state_2;
 int threshold = 100;

 //	Function to crop PCL
 void cloud_cb_1 (const sensor_msgs::PointCloud2ConstPtr& cloud_msg)
 {
 
 // Create container
 pcl::PCLPointCloud2* cloud = new pcl::PCLPointCloud2;
 pcl::PCLPointCloud2ConstPtr cloudPtr(cloud);
 pcl::PCLPointCloud2 cloud_filtered;

 // Convert type sensor_msgs to pcl
 pcl_conversions::toPCL(*cloud_msg, *cloud);

 // Crop the PCL
 pcl::CropBox<pcl::PCLPointCloud2> boxFilter;
 boxFilter.setInputCloud(cloudPtr);
 boxFilter.setMin(Eigen::Vector4f(0.0, 0.0, 0.0, 0));
 boxFilter.setMax(Eigen::Vector4f(0.15, 0.1, 0.2, 0));
 boxFilter.setTranslation(Eigen::Vector3f(0.2, 0.35, 0.9));
 boxFilter.setRotation(Eigen::Vector3f(0.1, 0.0, 0.0));
 boxFilter.filter(cloud_filtered);

 // Convert back the type
 sensor_msgs::PointCloud2 output;
 pcl_conversions::fromPCL(cloud_filtered, output);
 int pcl_count = 0;
 for (sensor_msgs::PointCloud2ConstIterator<float> it(output, "x"); it != it.end(); ++it) {
     // TODO: do something with the values of x, y, z
	   //std::cout << pcl_count << "\n";
	   pcl_count++;
     //std::cout <<  it[0] << "/ ";
     //std::cout <<  it[1]<< "/ ";
     //std::cout <<  it[2]<< "/ ";
     //std::cout << std::endl;
 }

 //Bool
 if(pcl_count > threshold)
 {
 		state_1.data = true;
 		}
 		else
 		{
 		state_1.data = false;
 }
 std::cout << "PCL On Button 1 :" << pcl_count << "/ "<< threshold;
 std::cout << std::endl<< std::endl;
 


 // Publish the data
 pub_1.publish (output);
 pcl_mval_pub_1.publish (state_1);
 
 }

void cloud_cb_2 (const sensor_msgs::PointCloud2ConstPtr& cloud_msg)
 {
 
 // Create container
 pcl::PCLPointCloud2* cloud = new pcl::PCLPointCloud2;
 pcl::PCLPointCloud2ConstPtr cloudPtr(cloud);
 pcl::PCLPointCloud2 cloud_filtered;

 // Convert type sensor_msgs to pcl
 pcl_conversions::toPCL(*cloud_msg, *cloud);

 // Crop the PCL
 pcl::CropBox<pcl::PCLPointCloud2> boxFilter;
 boxFilter.setInputCloud(cloudPtr);
 boxFilter.setMin(Eigen::Vector4f(0.0, 0.0, 0.0, 0));
 boxFilter.setMax(Eigen::Vector4f(0.15, 0.1, 0.2, 0));
 boxFilter.setTranslation(Eigen::Vector3f(0.05, 0.35, 0.9));
 boxFilter.setRotation(Eigen::Vector3f(0.1, 0.0, 0.0));
 boxFilter.filter(cloud_filtered);

 // Convert back the type
 sensor_msgs::PointCloud2 output;
 pcl_conversions::fromPCL(cloud_filtered, output);
 int pcl_count = 0;
 for (sensor_msgs::PointCloud2ConstIterator<float> it(output, "x"); it != it.end(); ++it) {
     // TODO: do something with the values of x, y, z
	   //std::cout << pcl_count << "\n";
	   pcl_count++;
     //std::cout <<  it[0] << "/ ";
     //std::cout <<  it[1]<< "/ ";
     //std::cout <<  it[2]<< "/ ";
     //std::cout << std::endl;
 }

 if(pcl_count > threshold)
 {
 		state_2.data = true;
 		}
 		else
 		{
 		state_2.data = false;
 }
 
 std::cout << "PCL On Button 2 :" << pcl_count << "/ "<< threshold;
 std::cout << std::endl<< std::endl<< std::endl;
 
 // Publish the data
 pub_2.publish (output);
 pcl_mval_pub_2.publish (state_2);
 
 }

 int main (int argc, char** argv)
{
// Initialize ROS
ros::init (argc, argv, "pcl_crop");
ros::NodeHandle nh;

// Create a ROS subscriber for the input point cloud
ros::Subscriber sub_1 = nh.subscribe ("Cloud_Filtered", 1, cloud_cb_1);
ros::Subscriber sub_2 = nh.subscribe ("Cloud_Filtered", 1, cloud_cb_2);

// Create a ROS publisher for the output point cloud
pub_1 = nh.advertise<sensor_msgs::PointCloud2> ("/Cloud_Cropped_1", 1);
pub_2 = nh.advertise<sensor_msgs::PointCloud2> ("/Cloud_Cropped_2", 1);
pcl_mval_pub_1 = nh.advertise<std_msgs::Bool> ("/PCL_MVal_1", 1);
pcl_mval_pub_2 = nh.advertise<std_msgs::Bool> ("/PCL_MVal_2", 1);
// Spin
ros::spin ();
}
