/** @file
 *
 * @ingroup spatdiflib-examples
 *
 * @brief Example: Test #sdSaver
 *
 * @details
 *
 * @authors Chikashi Miyama, Trond Lossius
 *
 * @copyright Copyright (C) 2013 - 2014 Zurich University of the Arts, Institute for Computer Music and Sound Technology @n
 * This code is licensed under the terms of the "New BSD License" @n
 * http://creativecommons.org/licenses/BSD/
 */


#include "sdScene.h"
#include "sdLoader.h"
#include "sdSaver.h"
#include "sdEntityCore.h"
#include <string>
#include <iostream>
#include <fstream>

int main(void){
    using namespace std;
    
    sdInfo info(string("chikashi miyama"), string("sdSaverTest"), string("2013-08-04"), string("Cologne"), string("1.2"), string("this is a test"));
    sdScene scene(info);
    
    scene.addExtension(SD_MEDIA);
    //the scene has one entities
    sdEntityCore *myEntity = scene.addEntity("myEntity", SD_SOURCE);
    sdEntityCore *yourEntity = scene.addEntity("yourEntity", SD_SOURCE);

    
    //set first event and attach it to myEntity
    double position[3];
    
    position[0]= 1.0;
    position[1] = 2.0;
    position[2] = 3.0;
    myEntity->addEvent(9.15 , SD_POSITION, (void*)&position);
    
    //set second event and attach it to myEntity
    position[0] = 0.0;
    position[1] = 1.0;
    position[2] = 2.0;
    myEntity->addEvent(2.12 , SD_ORIENTATION, (void*)&position);
    
    
    //set thrid event and attach it to yourEntity
    position[0] = 5.0;
    position[1] = 4.0;
    position[2] = 3.0;
    yourEntity->addEvent(4.2 , SD_POSITION, (void*)&position);
    
    scene.dump();
    
    // convert current scene to a XMLstring
    string generatedString = sdSaver::XMLFromScene(&scene);

    // save string as a file
    ofstream ofsTime("generated-scene-time.xml");
    ofsTime << generatedString << endl;
    ofsTime.close();

    scene.setOrdering(SD_TRACK);
    generatedString = sdSaver::XMLFromScene(&scene);
    ofstream ofsTrack("generated-scene-track.xml");
    ofsTrack << generatedString << endl;
    ofsTrack.close();
    
    return 0;
}
