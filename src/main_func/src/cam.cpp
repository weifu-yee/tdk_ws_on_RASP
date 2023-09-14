#include "cam.h"
set<int> CAM::numbers;

void CAM::capture_n_detect(int op, ros::Publisher& cam_pub, 
        ros::Publisher& orientation_pub, ros::NodeHandle& nh){
    CAM::numbers.clear();
    std_msgs::Int64 cmd_cam;
    cmd_cam.data = 2;
    std_msgs::Int64 cease;
    cease.data = -1;

    ROS_INFO("cease!!");

    ros::Rate rate(20); //20Hz
    bool flag = 0;
    int _a = 0, _b = 0;
    do{     //the capture and detect process
        ros::spinOnce();
        cam_pub.publish(cmd_cam);
        orientation_pub.publish(cease);
        int a = 0, b = 0;
        for(int i = op; i <= op + 2; i++){
            if(numbers.find(i) != numbers.end())
            if(!a){     a = i;      flag = 0;}
            else if(!b){    b = i;    flag = 1;}
            else    flag = 0;
        }
        _a = a;     _b = b;
        rate.sleep();
    }while(!flag && nh.ok());
    what_to_erase(_a, _b);
}
void CAM::what_to_erase(int a, int b){
    auto eraseBox = [&](int u){
        if(u < 7)
            MAP::eraseEdge(u, u+3);
        else{
            if(u == 7)  MAP::eraseEdge(7, 12);
            else if(u == 8)  MAP::eraseEdge(8, 11);
            else if(u == 9)  MAP::eraseEdge(9, 10);
        }
    };
    eraseBox(a);
    eraseBox(b);
    if(a == 1 && b == 3)    MAP::eraseEdge(2, 3);
    if(a == 2 && b == 3)    MAP::eraseEdge(0, 2);
    if(a == 5 && b == 6 && MAP::nodeNow == 5)    MAP::eraseEdge(5, 6);
    if(a == 8 && b == 9 && MAP::nodeNow == 8)    MAP::eraseEdge(8, 9);
}