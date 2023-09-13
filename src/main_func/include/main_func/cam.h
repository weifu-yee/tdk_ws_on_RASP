#ifndef _CAM_H_
#define _CAM_H_

#include "map.h"

namespace CAM{
    void openCam();
    void closeCam();
    void capture_n_identity(int op);
    void what_to_erase(int a, int b);
}
using namespace CAM;

#endif