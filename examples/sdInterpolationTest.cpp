/** @file
 *
 * @ingroup spatdiflib-examples
 *
 * @brief Example: Test all functionalities of the #sdInfo class
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
#include <iostream>

using namespace std;
int main(){
    
    sdScene scene;
    sdEntityCore * myEntity = scene.addEntity("myEntity");
    sdEntityCore * yourEntity = scene.addEntity("yourEntity");
    
    // myEntity setup
    double pos1[3] = {0.0, 0.0, 0.0};
    double pos2[3] = {10.0, 10.0, 10.0};

    myEntity->addEvent(0.0, SD_POSITION, pos1);
    myEntity->addEvent(10.0, SD_POSITION, pos2);
    
    sdEvent* event = myEntity->getEvent(5.0, SD_POSITION);
    if(!event){
        cout << "no event of position at 5.0 " << endl;
    }
    
    myEntity->activateInterpolation(SD_POSITION);
    event = myEntity->getEvent(5.0, SD_POSITION);

    if(event){
        cout << "interpolated position at 5.0:" << event->getValueAsString() << endl;
    }
}