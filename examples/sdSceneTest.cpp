/** @file
 *
 * @ingroup spatdiflib-examples
 *
 * @brief Example: Test #sdScene
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
#include <set>

// a sdScene test
// extension tests not included

using namespace std;

int main(void){
    
    sdInfo info(
                string("Chikashi"),
                string("sdSceneTest"),
                string("2013-08-04"),
                string("1.2"),
                string("Cologne"),
                string("this a scene test"));
    
    //set info and create a scene
    sdScene scene(info);
    sdInfo providedInfo = scene.getInfo();
    
    //activate media extension
    scene.addExtension(SD_MEDIA);

    //query how many extension(s) are active
    int numExtensions = scene.getNumberOfActivatedExtensions();
    cout << "number of active extensions: " << numExtensions << endl;
    
    cout << "making scene" << endl;
    //attach two entities
    sdEntityCore *myEntity = scene.addEntity("myEntity"); // spawn an entity
    double firstPos[3] = {0.0, 0.1, 0.2};
    double secondPos[3] = {0.3, 0.4, 0.5};
    double gain = 0.5523;
    //addEvent to myEntity
    myEntity->addEvent(string("1.0"), string("position"), string("0.0 0.1 0.2"));
    myEntity->addEvent(2.0, SD_POSITION, static_cast<void*>(firstPos));
    myEntity->addEvent(2.0, SD_MEDIA_GAIN, static_cast<void*>(&gain));
    cout << "end of making scene" << endl;
    scene.dump();

    cout << "------ queries ------" << endl;
    
    sdEntityCore *yourEntity = scene.addEntity("yourEntity"); // spawn an entity

    //asks how many entities are registerd
    cout << "Num Entities:" << scene.getNumberOfEntities() << endl;
    
    //get a pointer to an entity specified by name
    sdEntityCore *providedEntity = scene.getEntity("myEntity");
    
    //get num events
    cout << "Number of Events attached to the entity:" << providedEntity->getNumberOfEvents() << endl;
    
    //get event at time
    sdEvent* positionEvent = providedEntity->getEvent(0.2, SD_POSITION);
    
    //if there is a position data at 0.2
    if(positionEvent){
        //convert recognaizes desciriptor and convert it to a string
        cout << "position value:" << positionEvent->getValueAsString() << endl;
    }

    //vector<sdReport> eventSetVector = scene.getNextEventSets(0.4);
    vector<sdReport> eventSetVector = scene.getNextEventSetsFromAllEntities(0.4);


    //you can ask just the time of next event
    double nextEventTime = scene.getNextEventTime(0.4);
    cout << "Next Event Time:" << nextEventTime << endl;

    //you can ask just the time of next event
    double deltaTime = scene.getDeltaTimeToNextEvent(0.4);
    cout << "Delta time to next event:" << deltaTime << endl;


    sdEntityCore* duplicated = scene.addEntity("myEntity"); //if the name of existing entity, returns pointer to existing one
    //returns 2 not 3
    cout << "Num Entities:" << scene.getNumberOfEntities() << endl;
    for(int i = 0; i < scene.getNumberOfEntities(); i++){
	cout << "entity no." << i << ": " << scene.getEntityName(i) << endl;
    }


    
    return 0;
}
