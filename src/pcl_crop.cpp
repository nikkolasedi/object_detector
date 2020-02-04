#include <ros/ros.h>
#include <pcl_ros/point_cloud.h>
#include <pcl/point_types.h>
#include <boost/foreach.hpp>
#include <pcl/filters/crop_box.h>
#include <sensor_msgs/PointCloud2.h>
#include <sensor_msgs/point_cloud2_iterator.h>
#include <std_msgs/Float32.h>

 // Create publisher
 ros::Publisher pub; 						//for PCL
 ros::Publisher pcl_mval_pub; 	//for the number of PCL

 //	Function to crop PCL
 void cloud_cb (const sensor_msgs::PointCloud2ConstPtr& cloud_msg)
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
 boxFilter.setMax(Eigen::Vector4f(0.3, 0.3, 0.3, 0));
 boxFilter.setTranslation(Eigen::Vector3f(0.0, -0.1, 0.5));
 boxFilter.filter(cloud_filtered);

 // Convert back the type
 sensor_msgs::PointCloud2 output;
 pcl_conversions::fromPCL(cloud_filtered, output);
 int pcl_count = 0;
 std_msgs::Float32 pcl_max;
 for (sensor_msgs::PointCloud2ConstIterator<float> it(output, "x"); it != it.end(); ++it) {
     // TODO: do something with the values of x, y, z
	   std::cout << pcl_count << "\n";
	   pcl_count++;
     std::cout <<  it[0] << "/ ";
     std::cout <<  it[1]<< "/ ";
     std::cout <<  it[2]<< "/ ";
     std::cout << std::endl;
 }

 // Publish the pcl_max
 pcl_max.data = pcl_count;

 // Publish the data
 pub.publish (output);
 pcl_mval_pub.publish (pcl_max);
 
 }

 int main (int argc, char** argv)
{
// Initialize ROS
ros::init (argc, argv, "pcl_crop");
ros::NodeHandle nh;

// Create a ROS subscriber for the input point cloud
ros::Subscriber sub = nh.subscribe ("Cloud_Filtered", 1, cloud_cb);

// Create a ROS publisher for the output point cloud
pub = nh.advertise<sensor_msgs::PointCloud2> ("/Cloud_Cropped", 1);
pcl_mval_pub = nh.advertise<std_msgs::Float32> ("/PCL_MVal", 1);
// Spin
ros::spin ();
}
