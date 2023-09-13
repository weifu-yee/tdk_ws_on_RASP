#include "map.h"
#include "cam.h"
using namespace std;

bool isNodeLast = false;
bool onNode = false;
int nodeNow = -1;
int nodeToGo;
set<int> numbers;
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
void numberCallback(const std_msgs::Bool::ConstPtr& the_numbers){
    ROS_INFO("numberCallback");
    // numbers.insert();
    // CAM::what_to_erase(int a, int b);
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

    while(nh.ok()){
        ros::spinOnce();
        if(onNode){
            if(nodeNow == -1){
                CAM::capture_n_identity(123);
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
                CAM::capture_n_identity(456);
            if(MAP::node[nodeNow].second.first == 1 && MAP::node[nodeToGo].second.first == 2)
                CAM::capture_n_identity(789);
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