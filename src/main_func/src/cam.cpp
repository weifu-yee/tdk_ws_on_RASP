#include "cam.h"

void CAM::openCam(){
    cout<<"openCam"<<endl;
}
void CAM::closeCam(){
    cout<<"closeCam"<<endl;
}
void CAM::capture_n_identity(){
    rospy.sleep(6);
    cout<<"capture_n_identity"<<endl;
}