#include "sdScene.h"
#include "sdLoader.h"
#include "sdSaver.h"
#include "sdEntityCore.h"
#include "sdConverter.h"
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
    
    //attach two entities
    sdEntityCore *myEntity = scene.addEntity("myEntity"); // spawn an entity
    float pos[3] = {0.0, 0.1, 0.2};
    myEntity->addEvent(0.2, SD_POSITION, static_cast<void*>(pos));
    
    sdEntityCore *yourEntity = scene.addEntity("yourEntity"); // spawn an entity

    //asks how many entities are registerd
    cout << "Num Entities:" << scene.getNumberOfEntities() << endl;
    
    //get a pointer to an entity specified by name
    sdEntityCore *providedEntity = scene.getEntity("myEntity");
    
    //get num entities
    cout << "Number of Events attached to the entity:" << providedEntity->getNumberOfEvents() << endl;
    
    //get event at time
    sdEvent* positionEvent = providedEntity->getEvent(0.2, SD_POSITION);
    
    //if there is a position data at 0.2
    if(positionEvent){
        //convert recognaizes desciriptor and convert it to a string
        cout << "position value:" << sdConverter::convert(positionEvent) << endl;
        
    }
    

    return 0;
}
