#include "map.h"
#include "cam.h"
using namespace std;
using namespace MAP;

bool isNodeLast = false;
bool onNode = false;
int nodeToGo;
int xNow, xLast = -1;

void firstLevel(ros::NodeHandle& nh);
void nodeCallback(const std_msgs::Bool::ConstPtr& is_node){
    // ROS_INFO("nodeCallback");
    bool isNode = is_node->data;

    if(isNode != isNodeLast && isNode){
        onNode = true;
    }

    isNodeLast = isNode;
}
void numberCallback(const std_msgs::Int32MultiArray::ConstPtr& the_numbers){
    // ROS_INFO("numberCallback");
    CAM::numbers.clear();
    for(auto i:the_numbers->data){
        CAM::numbers.insert(i);
    }
}

ros::Publisher orientation_pub;
ros::Subscriber node_sub;
ros::Publisher cam_pub;
ros::Subscriber number_sub;

std_msgs::Int8 orientation;
std_msgs::Int8 open_o_close;

int main(int argc, char **argv){
    ros::init(argc, argv, "main_func");
    ros::NodeHandle nh;
    orientation_pub = nh.advertise<std_msgs::Int8>("/cmd_ori", 1);
    node_sub = nh.subscribe("/node_detect",1,nodeCallback);
    cam_pub = nh.advertise<std_msgs::Int8>("/cmd_cam", 1);
    number_sub = nh.subscribe("/numbers",1,numberCallback);

    MAP::buildNode();
    MAP::initBuildEdge();

    orientation.data = 0;   //front
    open_o_close.data = 1;


    while(nh.ok()){
        firstLevel(nh);
        ROS_INFO("pass 1st Level!!");
        break;
    }
    orientation.data = -1;
    orientation_pub.publish(orientation);
    return 0;
}

void firstLevel(ros::NodeHandle& nh){
    ros::Rate rate(20);
    while(nh.ok() && MAP::nodeNow < 13){
        cam_pub.publish(open_o_close);
        ros::spinOnce();
        if(onNode){
            if(nodeNow == -1){
                CAM::capture_n_detect(1, cam_pub, orientation_pub, nh);
                nodeNow = 0;
            }
            
            xNow = MAP::node[nodeNow].second.first;
            
            if(xNow != xLast && xNow == 1)
                CAM::capture_n_detect(4, cam_pub, orientation_pub, nh);
            if(xNow != xLast && xNow == 2){
                CAM::capture_n_detect(7, cam_pub, orientation_pub, nh);
                open_o_close.data = 0;
            }

            xLast = xNow;

            auto arr = MAP::adj_list[nodeNow];
            int max = -1;
            for(auto it = arr.begin(); it != arr.end(); ++it){
                max = (max<*it)?*it:max;
            }
            if(max == -1){
                ROS_INFO("NoWay!!");
                return;
            }
            nodeToGo = max;

            ROS_INFO("On the node %d, and will go to the node %d",nodeNow,nodeToGo);

            orientation.data = MAP::cmd_ori(nodeToGo, nodeNow);
            MAP::eraseEdge(nodeToGo, nodeNow);
            nodeNow = nodeToGo;
            onNode = false;
        }
        ROS_INFO("go ahead: %d",orientation.data);
        orientation_pub.publish(orientation);
        rate.sleep();
    }
}