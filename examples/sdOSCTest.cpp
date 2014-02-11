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


#include "sdMain.h"
#include <string>
#include <iostream>

int main(void){
    using namespace std;
    
    sdScene scene;
    sdEntityCore *myEntity = scene.addEntity("myEntity");
    myEntity->addEvent(string("1.0"), string("position"), string("0.0 0.1 0.2"));

    sdOSCResponder oscResponder = sdOSCResponder(&scene);
    scene.dump();
    
    // query with OSC message
    
    string oscMessage = string("/spatdifcmd/setQueryTime 1.0");
    oscResponder.forwardOSCMessage(oscMessage);
    
    oscMessage = string("/spatdifcmd/getPosition myEntity");
    cout << oscResponder.forwardOSCMessage(oscMessage) << endl;

    return 0;
}
