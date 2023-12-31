#ifndef _MAP_H_
#define _MAP_H_

#include <iostream>
#include <fstream>
#include <vector>
#include <yaml-cpp/yaml.h>
#include <utility>
#include <set>
#include "ros/ros.h"
#include "std_msgs/Bool.h"
#include "std_msgs/Int8.h"
#include "std_msgs/Int64.h"
#include "std_msgs/Int32MultiArray.h"

#define buildNodeFilePath "/home/ditrobotics/tdk_ws/src/main_func/params/buildNode.yaml"
#define initBuildEdgeFilePath "/home/ditrobotics/tdk_ws/src/main_func/params/initBuildEdge.yaml"
#define num_of_nodes 16
#define tolerence 6.0
#define decelerationZone 0.5

using namespace std;

namespace MAP{
    extern int nodeNow;
    extern vector<pair<int, pair<double, double>>> node;    //<index, x, y>
    extern vector<set<int>> adj_list;     //adjacency_list
    void buildNode();
    void initBuildEdge();
    void eraseEdge(int u, int v);
    int cmd_ori(int u, int v);
    int dis_of_Odom_n_ToGo(int u);
    bool check_onNode(int u);
}

using namespace MAP;

#endif