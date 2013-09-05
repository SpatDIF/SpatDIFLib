#include "sdScene.h"
#include "sdLoader.h"
#include "sdSaver.h"
#include "sdEntityCore.h"
#include <string>
#include <iostream>
#include <fstream>

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
    
    
    //attach two entities
    sdEntityCore *myEntity = scene.addEntity("myEntity"); // spawn an entity
    double firstPos[3] = {0.0, 0.1, 0.2};
    double secondPos[3] = {0.3, 0.4, 0.5};
    double thirdPos[3] = {0.6, 0.7, 0.8};

    //addEvent to myEntity
    myEntity->addEvent(string("0.2"), string("position"), string("0.0 0.1 0.2"));
    myEntity->addEvent(0.5, SD_POSITION, static_cast<void*>(secondPos));

    cout << "num events attached to myEntity after second addition:" << myEntity->getNumberOfEvents() << endl;
    cout << "values:" << myEntity->getEvent(0.5, SD_POSITION)->getValueAsString() << endl;

    //add event but with same time and desciptor .. this will replace the previous one
    sdEvent* returnedEvent = myEntity->addEvent(0.5, SD_POSITION, static_cast<void*>(thirdPos));
    
    cout << "num events attached to myEntity after third addition:" << myEntity->getNumberOfEvents() << endl;
    cout << "descriptor:" << returnedEvent->getDescriptorAsString() << endl;
    cout << "values:" << myEntity->getEvent(0.5, SD_POSITION)->getValueAsString() << endl;
    
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
    
    sdEntityCore* duplicated = scene.addEntity("myEntity"); //if the name of existing entity, returns pointer to existing one
    //returns 2 not 3
    cout << "Num Entities:" << scene.getNumberOfEntities() << endl;
    if(duplicated == providedEntity){
        cout << "same pointer" << endl;
    }
    
    return 0;
}
