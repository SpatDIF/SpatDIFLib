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
    double gain = 0.51525;
    double time_offset = 0.5;
    entityOne->addEvent(5.0, SD_MEDIA_GAIN, static_cast<void*>(&gain));
    entityOne->addEvent(5.2, SD_MEDIA_TIME_OFFSET, static_cast<void*>(&time_offset));
    time_offset = 0.4;
    entityOne->addEvent(5.2, SD_MEDIA_TIME_OFFSET, static_cast<void*>(&time_offset));
    entityOne->addEvent(5.3, SD_MEDIA_ID, static_cast<void*>(&string("sound source1")));
    entityOne->addEvent(5.0, SD_MEDIA_TYPE, static_cast<void*>(&string("stream")));
    entityOne->addEvent(5.5, SD_MEDIA_LOCATION, static_cast<void*>(&string("mySoundFile.wav")));

    // dump the scene
    scene.dump();
    
    // remove two events
    entityOne->removeEvent(5.2, SD_MEDIA_TIME_OFFSET);
    entityOne->removeEvent(5.0, SD_MEDIA_GAIN);

    // check
    scene.dump();
    
    // ask the entity about the extension descriptor
    cout<< "entityOne media:location at 4.0 " << *static_cast<string*>(entityOne->getValue(4.0, SD_MEDIA_LOCATION)) << endl;
    
    return 0;
}
