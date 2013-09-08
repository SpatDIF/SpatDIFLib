/* this example examines all functionalities of sdDate class */

#include <string>

#include "sdScene.h"
#include "sdEntityCore.h"

int main(void){
    
    using namespace std;

    // a scene without extensions
    sdScene scene;
    
    // add an entity to the scene
    sdEntityCore* entityOne = scene.addEntity("voice1");
    
    // the following messages add event to the core
    double position[3] = {0.0,0.3,0.4};

    entityOne->addEvent(1.0, SD_POSITION, position);
    entityOne->addEvent(2.0, SD_POSITION, position);

    // this addEvent returns null to evt because extension is not added to the scene
    string loc = string("/User/john/sample.wav");
    sdEvent* evt= entityOne->addEvent(3.0, SD_MEDIA_LOCATION, static_cast<void*>(&loc));
    
    if(!evt){
        cout << "the scene does not understand provided descriptor" << endl;
    }
    
    // now add the extension
    scene.addExtension(SD_MEDIA);

    // the message below is the message for extension. so the core does not understand.
    evt= entityOne->addEvent(4.0, SD_MEDIA_LOCATION, static_cast<void*>(&loc));

    // dump the scene
    scene.dump();
    
    // ask the entity about the extension descriptor
    cout<< "entityOne media:location at 4.0 " << *static_cast<string*>(entityOne->getValue(4.0, SD_MEDIA_LOCATION)) << endl;
    
    return 0;
}
