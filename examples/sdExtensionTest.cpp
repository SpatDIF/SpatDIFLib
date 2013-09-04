/* this example examines all functionalities of sdDate class */

#include <string>

#include "sdScene.h"
#include "sdEntityCore.h"
#include "sdEntityExtensionMedia.h"

int main(void){
    
    using namespace std;
    
    // a scene without extensions
    sdScene scene;
    
    // add an entity to the scene
    sdEntityCore* voice1 = scene.addEntity("voice1");
    
    // the following messages add event to the core
    float position[3] = {0.0,0.3,0.4};
    voice1->addEvent(0.0, SD_POSITION, position);
    
    // the message below is the message for extension. so the core does not understand.
    sdEvent* evt= voice1->addEvent(0.0, SD_MEDIA_LOCATION, "/User/john/sample.wav");
    
    // Thus the pointer points NULL
    if(!evt){
        cout << "event not created" << endl;
    }
    
    // now add an extension to the scene
    scene.addExtension(SD_MEDIA);
    
    // ...now the sdScene automatically attach sdEntityExtensionMedia to all existing entityCores in the scene.
    
    
    // once again
    evt= voice1->addEvent(0.0, SD_MEDIA_LOCATION, "/User/john/sample.wav");
    
    // now the evt points at an instance of sdEventExtensionMedia
    
    if(evt){
        cout << "event created properly" << endl;
    }

    // now query entity
    
    
    
    return 0;
}
