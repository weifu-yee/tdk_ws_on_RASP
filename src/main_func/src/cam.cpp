#include "cam.h"

void CAM::openCam(){
    cout<<"openCam"<<endl;
}
void CAM::closeCam(){
    cout<<"closeCam"<<endl;
}
void CAM::capture_n_identity(int op){
    switch(op){
    case 123:
        break;
    case 456:
        break;
    case 789:
        break;
    default:    break;
    }
    // ros.sleep(3);
    //直到確實拍到2個數字為止，設定區間1~3、4~6、7~9
    // CAM::what_to_erase(int a, int b);
    cout<<"capture_n_identity"<<endl;
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
    if(a == 1 && b == 3 || a == 3 && b == 1)    MAP::eraseEdge(2, 3);
    if(a == 2 && b == 3 || a == 3 && b == 2)    MAP::eraseEdge(0, 2);
    if(a == 5 && b == 6 || a == 6 && b == 5)    MAP::eraseEdge(5, 6);
    if(a == 8 && b == 9 || a == 9 && b == 8)    MAP::eraseEdge(8, 9);
}