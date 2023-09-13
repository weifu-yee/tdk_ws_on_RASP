#include "map.h"
using namespace std;

bool isNodeLast = false;
bool onNode = false;
int nodeNow = -1;
int nodeLast = -1;

void Callback(const std_msgs::Bool::ConstPtr& is_node){
    cout<<"1";
    bool isNode = is_node->data;
    if(isNode != isNodeLast && isNode){
        onNode = true;
    }
    isNodeLast = isNode;
}

int main(int argc, char **argv){
    ros::init(argc, argv, "main_func");
    ros::NodeHandle nh;
    ros::Publisher orientation_pub = nh.advertise<std_msgs::Int64>("/cmd_ori", 1);
    ros::Subscriber node_sub = nh.subscribe("/node_detect",1,Callback);
    ros::Publisher cam_pub = nh.advertise<std_msgs::Int64>("/cmd_cam", 1);
    ros::Subscriber number_sub = nh.subscribe("/numbers",1,Callback);
    ros::Rate rate(20); //20Hz

    MAP::buildNode();
    MAP::initBuildEdge();
    
    std_msgs::Int64 orientation;
    orientation.data = 0;   //front

    while(nh.ok()){
        ros::spinOnce();
        if(onNode){
            if(nodeNow == -1)   nodeNow = 0;
            else    nodeNow = MAP::adj_list[nodeNow].front();
            orientation.data = MAP::cmd_ori(nodeNow, nodeLast);
            // MAP::eraseEdge(u,v);
            nodeLast = nodeNow;
        }
        orientation_pub.publish(orientation);
        rate.sleep();
        cout<<"2";
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