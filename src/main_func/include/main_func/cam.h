#ifndef _CAM_H_
#define _CAM_H_

#include "map.h"

namespace CAM{
    extern set<int> numbers;
    void openCam();
    void closeCam();
    void capture_n_identify(int op, ros::Publisher& publisher, ros::NodeHandle& nh);
    void what_to_erase(int a, int b);
}
using namespace CAM;

#endif