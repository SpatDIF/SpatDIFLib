/** @file
 *
 * @ingroup spatdiflib-examples
 *
 * @brief Example: Test extentions
 *
 * @details
 *
 * @authors Chikashi Miyama, Trond Lossius
 *
 * @copyright Copyright (C) 2013 - 2014 Zurich University of the Arts, Institute for Computer Music and Sound Technology @n
 * This code is licensed under the terms of the "New BSD License" @n
 * http://creativecommons.org/licenses/BSD/
 */


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
    
    
    if(scene.isExtensionActivated(SD_MEDIA)){ // returns false
        cout << "media extension activated!" << endl; // this line will be not posted
    }
    // now add the extension
    scene.addExtension(SD_MEDIA);

    if(scene.isExtensionActivated(SD_MEDIA)){ // returns true
        cout << "media extension activated!!" << endl; // this line will be  posted
    }

    cout << "currently " << scene.getNumberOfActivatedExtensions() << "extension(s) are activated.";

    cout << "list of activated extensions:" << endl;
    vector<string> extensionStringVector = scene.getActivatedExtensionsAsStrings();
    vector<string>::iterator it = extensionStringVector.begin();
    while(it != extensionStringVector.end()){
        cout << *it << endl;
        it++;
    }
    cout << "-----" << endl;
        // you cannot add the same extension twice. will get an error if you do.
    scene.addExtension(SD_MEDIA);


    // the message below is the message for extension. so the core does not understand.
    evt= entityOne->addEvent(4.0, SD_MEDIA_LOCATION, static_cast<void*>(&loc));
    double gain = 0.51525;
    double time_offset = 0.5;
    entityOne->addEvent(5.0, SD_MEDIA_GAIN, static_cast<void*>(&gain));
    entityOne->addEvent(5.2, SD_MEDIA_TIME_OFFSET, static_cast<void*>(&time_offset));
    time_offset = 0.4;
    
    // duplicate event will be ignored
    entityOne->addEvent(5.2, SD_MEDIA_TIME_OFFSET, static_cast<void*>(&time_offset));
    
    string id = "sound source1";
    string type = "stream";
    
    entityOne->addEvent(5.3, SD_MEDIA_ID, static_cast<void*>(&id));
    entityOne->addEvent(5.0, SD_MEDIA_TYPE, static_cast<void*>(&type));
    entityOne->addEvent(string("5.5"), string("location"), string("mysoundfile.wav"));

    // dump the scene
    scene.dump();
    
    // query event
    sdEvent* mediaLocation = entityOne->getNextEvent(5.2, SD_MEDIA_ID);
    if(mediaLocation == NULL){
        cout << "no such event" << endl;
    }else{
        cout << "time:" << mediaLocation->getTime() << " " <<  mediaLocation->getValueAsString() << endl;
    }
    // remove two events
    entityOne->removeEvent(5.2, SD_MEDIA_TIME_OFFSET);
    entityOne->removeEvent(5.0, SD_MEDIA_GAIN);

    // check
    scene.dump();
    
    // ask the entity about the extension descriptor
    cout<< "entityOne media:location at 4.0 " << *static_cast<string*>(entityOne->getValue(4.0, SD_MEDIA_LOCATION)) << endl;
    
    // remove the media extension
    scene.removeExtension(SD_MEDIA);

    // you will get error.
    scene.removeExtension(SD_MEDIA);
    
    // check again
    scene.dump();
    return 0;
}
