/* this example examines all functionalities of sdDate class */

#include "sdOSCResponder.h"
#include "sdScene.h"
#include <string>
#include <iostream>

int main(void){
    using namespace std;
    
    sdScene scene;
    sdOSCResponder oscResponder = sdOSCResponder(&scene);
    
    string oscMessage = string("/spatdif/source/2/position  ,fff    1.41 1.41 0.0");
    oscResponder.forwardOSCMessage(oscMessage);
    
    return 0;
}
