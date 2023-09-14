#include "cam.h"
set<int> CAM::numbers;

void CAM::openCam(){
    cout<<"openCam"<<endl;
}
void CAM::closeCam(){
    cout<<"closeCam"<<endl;
}
void CAM::capture_n_identify(int op, ros::Publisher& publisher, ros::NodeHandle& nh){
    std_msgs::Int64 cmd_cam;
    cmd_cam.data = 2;
    ros::Rate rate(20); //20Hz
    // for(int round = 0; round < 60; round++){
    //     publisher.publish(cmd_cam);
    //     rate.sleep();
    // }

    // ros.sleep(3);
    bool flag = 0;
    int _a = 0, _b = 0;
    do{
        ros::spinOnce();
        publisher.publish(cmd_cam);
        int a = 0, b = 0;
        for(int i = op; i <= op + 2; i++){
            if(numbers.find(i) != numbers.end())
            if(!a){     a = i;      flag = 0;}
            else if(!b){    b = i;    flag = 1;}
            else    flag = 0;
        }
        _a = a;     _b = b;
        rate.sleep();
        ROS_INFO("doWhile %d",MAP::nodeNow);
    }while(!flag && nh.ok());
    what_to_erase(_a, _b);
    //直到確實拍到2個數字為止，設定區間1~3、4~6、7~9
    cout<<"capture_n_identify"<<endl;
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
    if(a == 5 && b == 6 && MAP::nodeNow == 2)    MAP::eraseEdge(5, 6);
    if(a == 8 && b == 9 && MAP::nodeNow == 5)    MAP::eraseEdge(8, 9);
}