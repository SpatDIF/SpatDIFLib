/* this example examines all functionalities of sdDate class */

#include "sdOSCResponder.h"
#include "sdScene.h"
#include <string>
#include <iostream>

int main(void){
    using namespace std;
    
    sdScene scene;
    sdOSCResponder oscResponder = sdOSCResponder(&scene);
    
    string oscMessage = string("/spatdif/source/myEntity/position  ,fff    1.41 1.41 0.0");
    oscResponder.forwardOSCMessage(oscMessage);

     oscMessage = string("/spatdif/source/myEntity/orientation  ,fff    3 4 2");
    oscResponder.forwardOSCMessage(oscMessage);
    
    scene.dump();
    return 0;
}
