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
 * @copyright Copyright (C) 2013 - 2014 Zurich University of the Arts, Institute for Computer Music and Sound Technology @n
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

    sdOSCResponder oscResponder(&scene);
    scene.dump();
    
    // query with OSC message
    
    string oscMessage = string("/spatdifcmd/setQueryTime 1.0");
    oscResponder.forwardOSCMessage(oscMessage);
    
    oscMessage = string("/spatdifcmd/getPosition myEntity");
    vector<string> returnedMessages = oscResponder.forwardOSCMessage(oscMessage);
    vector<string>::iterator it = returnedMessages.begin();
    
    while (it!=returnedMessages.end()) {
        string str = *it;
        cout << str << endl;
        it++;
    }

    return 0;
}
