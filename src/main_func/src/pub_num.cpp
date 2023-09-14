#include "ros/ros.h"
#include "std_msgs/Int32MultiArray.h"

int main(int argc, char **argv)
{
    ros::init(argc, argv, "pub_num");
    ros::NodeHandle nh;
    ros::Publisher pub = nh.advertise<std_msgs::Int32MultiArray>("/numbers", 10);
    ros::Rate rate(1);  // 1 Hz

    while (ros::ok())
    {
        std_msgs::Int32MultiArray multi_array_msg;
        multi_array_msg.data = {2, 2, 9, 6, 8 ,4, 3};  // Example data

        pub.publish(multi_array_msg);
        rate.sleep();
    }

    return 0;
}
