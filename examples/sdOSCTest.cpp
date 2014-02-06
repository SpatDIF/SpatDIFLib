/** @file
 *
 * @ingroup spatdiflib-examples
 *
 * @brief Example: Test #sdOSCResponder
 *
 * @details
 *
 * @authors Chikashi Miyama, Trond Lossius
 *
 * @copyright Copyright © 2013 by ??? @n
 * This code is licensed under the terms of the "New BSD License" @n
 * http://creativecommons.org/licenses/BSD/
 */


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

    oscMessage = string("/spatdif/time 20.0"); // set current time using oscMessage
    oscResponder.forwardOSCMessage(oscMessage);
    oscMessage = string("/spatdif/source/myEntity/orientation ,fff 3 4 2");
    oscResponder.forwardOSCMessage(oscMessage);
    
    oscResponder.setCurrentTime(30.0); // set current time by calling function
    oscMessage = string("/spatdif/source/myEntity/position 2 3 5.55"); // without type tag
    oscResponder.forwardOSCMessage(oscMessage);

    oscMessage = string("/spatdif/source/myEntity/orientation 1 0 1"); // set orientation
    oscResponder.forwardOSCMessage(oscMessage, 40.0); // with time tag
    
    scene.dump();
    return 0;
}
