#include "map.h"
#include "cam.h"
#include "odom.h"
using namespace std;
using namespace MAP;

bool isNodeLast = false;
bool onNode = false;
int nodeToGo;
double xNow, xLast = -1;

ros::Publisher orientation_pub;
ros::Subscriber node_sub;
ros::Publisher cam_pub;
ros::Subscriber number_sub;
ros::Subscriber odom_sub;

std_msgs::Int8 orientation;
std_msgs::Int8 open_o_close;

namespace SCRIPT{
    void firstLevel(ros::NodeHandle& nh);
    void dustBox(ros::NodeHandle& nh);
}
void nodeCallback(const std_msgs::Bool::ConstPtr& is_node){
    bool isNode = is_node->data;

    if(isNode != isNodeLast && isNode){
        onNode = true;
    }

    isNodeLast = isNode;
}
void numberCallback(const std_msgs::Int32MultiArray::ConstPtr& the_numbers){
    CAM::numbers.clear();
    for(auto i:the_numbers->data){
        CAM::numbers.insert(i);
    }
}
void odomCallback(const geometry_msgs::Twist::ConstPtr& ins_vel){
    odometry.update(ins_vel);
    ROS_INFO("(%.4lf, %.4lf, %.4lf) to: %d",odometry.x, odometry.y, odometry.theta, orientation.data);
}

int main(int argc, char **argv){
    ros::init(argc, argv, "main_func");
    ros::NodeHandle nh;
    orientation_pub = nh.advertise<std_msgs::Int8>("/cmd_ori", 1);
    node_sub = nh.subscribe("/node_detect",1,nodeCallback);
    cam_pub = nh.advertise<std_msgs::Int8>("/cmd_cam", 1);
    number_sub = nh.subscribe("/numbers",1,numberCallback);
    odom_sub = nh.subscribe("/cmd_vel",1,odomCallback);     //fake odom
    // odom_sub = nh.subscribe("/realspeed",1,odomCallback);

    MAP::buildNode();
    MAP::initBuildEdge();

    orientation.data = 0;   //front
    open_o_close.data = 1;


    ROS_INFO("On %d, -> %d",nodeNow,nodeToGo);
    ROS_INFO("(%lf, %lf, %lf)",odometry.x, odometry.y, odometry.theta);
    ROS_INFO("go ahead: %d",orientation.data);

    SCRIPT::firstLevel(nh);
    ROS_INFO("pass 1st Level!!");
    while(nh.ok()){
        orientation.data = -1;
        orientation_pub.publish(orientation);
    }
    return 0;
}

void SCRIPT::firstLevel(ros::NodeHandle& nh){
    ros::Rate rate(20);
    while(nh.ok() && MAP::nodeNow < 13){
        cam_pub.publish(open_o_close);
        ros::spinOnce();

        if(onNode){
            // if(!MAP::check_onNode(nodeToGo)){
            //     ROS_INFO("Node misjudgment!!");
            //     onNode = false;
            //     continue;
            // }
            
            if(nodeNow == -1)   CAM::capture_n_detect(1, cam_pub, orientation_pub, nh);
            nodeNow = nodeToGo;

            odometry.x = MAP::node[nodeNow].second.first;
            odometry.y = MAP::node[nodeNow].second.second;

            xNow = MAP::node[nodeNow].second.first;
            if(xNow != xLast && xNow == 394.5)
                CAM::capture_n_detect(4, cam_pub, orientation_pub, nh);
            if(xNow != xLast && xNow == 530){
                CAM::capture_n_detect(7, cam_pub, orientation_pub, nh);
                open_o_close.data = 0;
            }
            xLast = xNow;

            auto arr = MAP::adj_list[nodeNow];
            int max = -1;
            for(auto it = arr.begin(); it != arr.end(); ++it)   max = (max<*it)?*it:max;
            if(max == -1){
                ROS_INFO("NoWay!!");
                return;
            }
            nodeToGo = max;

            orientation.data = MAP::cmd_ori(nodeToGo, nodeNow);
            if(nodeNow == 1 && nodeToGo == 4
            || nodeNow == 2 && nodeToGo == 5
            || nodeNow == 3 && nodeToGo == 6){
                orientation.data = 6;
            }
            MAP::eraseEdge(nodeToGo, nodeNow);
            // nodeNow = nodeToGo;
            onNode = false;            

            ROS_INFO("On %d, -> %d",nodeNow,nodeToGo);
            ROS_INFO("go ahead: %d",orientation.data);
            cout<<endl;
        }
        
        if(MAP::dis_of_Odom_n_ToGo(nodeToGo) < decelerationZone)    orientation.data = -2;
        orientation_pub.publish(orientation);
        rate.sleep();
    }
}
void SCRIPT::dustBox(ros::NodeHandle& nh){
    ros::Rate rate(20);
    while(nh.ok()){
        
    }
}