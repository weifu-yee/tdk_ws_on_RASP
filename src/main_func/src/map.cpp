#include "map.h"

int MAP::nodeNow = -1;
vector<pair<int, pair<double, double>>> MAP::node;     //<index, x, y>
vector<set<int>> MAP::adj_list(num_of_nodes);       //adjacency_list
// pair<int, int> MAP::reset1;
// pair<int, int> MAP::reset2;

void MAP::buildNode(){
    std::ifstream file(buildNodeFilePath);
    if (!file.is_open()) {
        std::cerr << "Error opening file!" << std::endl;
        return;
    }
    YAML::Node pathConfig = YAML::Load(file);
    for (auto nodeElement : pathConfig) {
        auto node = nodeElement["node"];
        int index = node[0].as<int>();
        double x = node[1].as<double>();
        double y = node[2].as<double>();
        // if(index == -1){
        //     MAP::reset1.first = x;
        //     MAP::reset1.second = y;
        //     continue;
        // }
        // if(index == -2){
        //     MAP::reset2.first = x;
        //     MAP::reset2.second = y;
        //     continue;
        // }
        MAP::node.push_back(make_pair(index, make_pair(x, y)));
    }
}
void MAP::initBuildEdge(){
    auto buildEdge = [&](int u, int v){
        MAP::adj_list[u].insert(v);
        MAP::adj_list[v].insert(u);
    };
    
    std::ifstream file(initBuildEdgeFilePath);
    if (!file.is_open()) {
        std::cerr << "Error opening file!" << std::endl;
        return;
    }
    YAML::Node pathConfig = YAML::Load(file);
    for (auto nodeElement : pathConfig) {
        auto edge = nodeElement["initEdge"];
        int u = edge[0].as<int>();
        int v = edge[1].as<int>();
        buildEdge(u,v);
    }
}
void MAP::eraseEdge(int u, int v){
    if(v == -1)  return;
    auto find_n_erase = [&](int x, int y){
        MAP::adj_list[x].erase(y);
    };
    find_n_erase(u,v);
    find_n_erase(v,u);
}
int MAP::cmd_ori(int u, int v){
    if(v == -1)  return 0;
    double ux = MAP::node[u].second.first;
    double uy = MAP::node[u].second.second;
    double vx = MAP::node[v].second.first;
    double vy = MAP::node[v].second.second;
    if(ux > vx)     return 0;
    if(uy > vy)     return 1;
    if(ux < vx)     return 2;
    if(uy < vy)     return 3;
    return -1;
}