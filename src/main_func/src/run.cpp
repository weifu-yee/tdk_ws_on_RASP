#include "map.h"
#include "cam.h"
using namespace std;
using namespace MAP;

bool isNodeLast = false;
bool onNode = false;
int nodeToGo;
int xNow, xLast;

void nodeCallback(const std_msgs::Bool::ConstPtr& is_node){
    ROS_INFO("nodeCallback");
    bool isNode = is_node->data;

    if(isNode){
        onNode = true;
    }
    isNode = false;

    // if(isNode != isNodeLast && isNode){
    //     onNode = true;
    // }

    isNodeLast = isNode;
}
void numberCallback(const std_msgs::Int32MultiArray::ConstPtr& the_numbers){
    ROS_INFO("numberCallback");
    CAM::numbers.clear();
    for(auto i:the_numbers->data){
        CAM::numbers.insert(i);
    }
}

int main(int argc, char **argv){
    ros::init(argc, argv, "main_func");
    ros::NodeHandle nh;
    ros::Publisher orientation_pub = nh.advertise<std_msgs::Int64>("/cmd_ori", 1);
    ros::Subscriber node_sub = nh.subscribe("/node_detect",1,nodeCallback);
    ros::Publisher cam_pub = nh.advertise<std_msgs::Int64>("/cmd_cam", 1);
    ros::Subscriber number_sub = nh.subscribe("/numbers",1,numberCallback);
    ros::Rate rate(20); //20Hz
    
    std_msgs::Int64 orientation;
    orientation.data = 0;   //front

    MAP::buildNode();
    MAP::initBuildEdge();

    CAM::openCam();

    // CAM::what_to_erase(1, 2);
    // CAM::what_to_erase(5, 6);
    // CAM::what_to_erase(9, 7);

    while(nh.ok()){
        ros::spinOnce();
        if(onNode){
            if(nodeNow == -1){
                CAM::capture_n_identify(1, cam_pub, nh);
                nodeNow = 0;
            }

            auto arr = MAP::adj_list[nodeNow];
            int max = -1;
            for(auto it = arr.begin(); it != arr.end(); ++it){
                max = (max<*it)?*it:max;
            }
            if(max == -1){
                ROS_INFO("NoWay!!");
                break;
            }
            nodeToGo = max;
            
            if(MAP::node[nodeNow].second.first == 0 && MAP::node[nodeToGo].second.first == 1)
                CAM::capture_n_identify(4, cam_pub, nh);
            if(MAP::node[nodeNow].second.first == 1 && MAP::node[nodeToGo].second.first == 2)
                CAM::capture_n_identify(7, cam_pub, nh);
            cout<<nodeNow<<" to "<<nodeToGo<<endl;

            orientation.data = MAP::cmd_ori(nodeToGo, nodeNow);
            MAP::eraseEdge(nodeToGo, nodeNow);
            nodeNow = nodeToGo;
            onNode = false;
        }
        orientation_pub.publish(orientation);
        rate.sleep();
    }
    
    // for(auto i:MAP::node){
    //     cout<<i.first<<" "<<i.second.first<<" "<<i.second.second<<endl;
    // }
    // cout<<"------"<<endl;
    // for(auto i:MAP::adj_list){
    //     for(auto j:i){
    //         cout<<j<<" ";
    //     }
    //     cout<<endl;
    // }


    return 0;
}