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
    
    
    // myEntity setup
    double pos1[3] = {0.0, 0.0, 0.0};
    double pos2[3] = {3.0, -4.0, 25.0};

    myEntity->addEvent(0.0, SD_POSITION, pos1);
    myEntity->addEvent(10.0, SD_POSITION, pos2);
    
    sdEvent* event = myEntity->getEvent(5.0, SD_POSITION);
    if(!event){
        cout << "no event of position at 5.0 " << endl;
    }
    
    // activate interpolation of SD_POSITION
    myEntity->activateInterpolation(SD_POSITION);
    
    if(myEntity->isInterpolationActivated(SD_POSITION)){
        cout << "interpolation of position is activated" << endl;
    }else{
        cout << "interpolation of position is not activated" << endl;
    }
    for(double time = 0.0 ; time < 10.0 ; time+= 0.1){
        double* positions = static_cast<double*>(myEntity->getValue(time, SD_POSITION));
        if(positions){
            cout << "interpolated position at time" << time << ":"  << doublesToString(positions, 3) << endl;
        }else{
            cout << "invalid time:" << time << endl;
        }
    }
}