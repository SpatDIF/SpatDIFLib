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

void populateMultiset(multiset<sdEvent*, sdEventCompare> eventSet);

int main(void){
    
    using namespace std;
    
    // a scene without extensions
    sdScene scene;
    sdEntityCore* entityOne = scene.addEntity("voice1");
    
    // the following messages add event to the core
    double position1[3] = {0.0,0.3,0.4};
    double position2[3] = {3.0,4.3,5.0};
    entityOne->addEvent(1.0, SD_POSITION, position1);
    entityOne->addEvent(5.0, SD_POSITION, position2);

    // this addEvent returns null to evt because extension is not added to the scene
    string loc = string("/User/john/sample.wav");
    sdEvent* evt= entityOne->addEvent(1.0, SD_MEDIA_LOCATION, static_cast<void*>(&loc));
    
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

    cout << "Number of activated extensions: " << scene.getNumberOfActivatedExtensions();

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
    double gain = 0.51525;
    double time_offset = 0.5;
    evt= entityOne->addEvent(1.0, SD_MEDIA_LOCATION, static_cast<void*>(&loc));
    entityOne->addEvent(5.0, SD_MEDIA_GAIN, static_cast<void*>(&gain));
    entityOne->addEvent(5.2, SD_MEDIA_TIME_OFFSET, static_cast<void*>(&time_offset));
    string id = "sound source1";
    string type = "stream";
    entityOne->addEvent(5.3, SD_MEDIA_ID, static_cast<void*>(&id));
    entityOne->addEvent(5.0, SD_MEDIA_TYPE, static_cast<void*>(&type));
    entityOne->addEvent(string("5.5"), string("location"), string("mysoundfile.wav"));

    // scene with extension events
    scene.dump();
    
    // check each function of entity with extensions
    cout << "function tests";
    {
        cout << "getNumberOfEvents()" << endl;
        cout << "entityOne number of events(incl. events of extensions):" << entityOne->getNumberOfEvents() << endl;
    }
    cout << "-----" << endl;
    {
        cout << "getEvent(4.0, SD_MEDIA_LOCATION)" << endl;
        cout << "media location event at time 1.0:" << entityOne->getEvent(1.0, SD_MEDIA_LOCATION)->getValueAsString() << endl;    
    }
    cout << "-----" << endl;
    {
        cout << "getEventSet()" << endl;
        multiset <sdEvent*, sdEventCompare> eventSet = entityOne->getEventSet();
        cout << "number of all events attached to entity : " << eventSet.size() << endl; 
        populateMultiset(eventSet);
    }
    cout << "-----" << endl;
    {
        cout << "getEventSet(5.0)" << endl;
        multiset <sdEvent*, sdEventCompare> eventSet = entityOne->getEventSet(5.0);
        cout << "number of events at 5.0: " << eventSet.size() << endl; 
        populateMultiset(eventSet);
    }
    cout << "-----" << endl;
    {
        cout << "getEventSet(5.1, 5.4)" << endl;
        multiset <sdEvent*, sdEventCompare> eventSet = entityOne->getEventSet(5.1, 5.4);
        cout << "number of events between 5.1 and 5.4: " << eventSet.size() << endl; 
        populateMultiset(eventSet);
    }
    cout << "-----" << endl;
    {
        cout << "getEventSet(5.1, 5.4, SD_MEDIA_TIME_OFFSET)" << endl;
        multiset <sdEvent*, sdEventCompare> eventSet = entityOne->getEventSet(5.1, 5.4, SD_MEDIA_TIME_OFFSET);
        cout << "number of time offset events between 5.1 and 5.4: " << eventSet.size() << endl; 
        populateMultiset(eventSet);
    }
    cout << "-----" << endl;
    {
        cout << "getNextEvent(4.0, SD_MEDIA_GAIN)" << endl;
        sdEvent* event = entityOne->getNextEvent(4.0, SD_MEDIA_GAIN);
        cout << "   -" << event->getTime() << ":" << event->getDescriptorAsString() << " " << event->getValueAsString() << endl;
    }
    cout << "-----" << endl;
    {
        cout << "getNextEvent(4.0, SD_POSITION)" << endl;
        sdEvent* event = entityOne->getNextEvent(4.0, SD_POSITION);
        cout << "   -" << event->getTime() << ":" << event->getDescriptorAsString() << " " << event->getValueAsString() << endl;
    }
    cout << "-----" << endl;
    {
        cout << "getNextEventSet(5.1)" << endl;
        multiset <sdEvent*, sdEventCompare> eventSet = entityOne->getNextEventSet(5.1);
        populateMultiset(eventSet);
    }
    cout << "-----" << endl;
    {
        cout << "getFirstEvent(SD_MEDIA_LOCATION)" << endl;
        sdEvent* event = entityOne->getFirstEvent(SD_MEDIA_LOCATION);
        cout << "   -" << event->getTime() << ":" << event->getDescriptorAsString() << " " << event->getValueAsString() << endl;
    }
    cout << "-----" << endl;

        // query event
    // sdEvent* mediaLocation = entityOne->getNextEvent(5.2, SD_MEDIA_ID);
    // if(mediaLocation == NULL){
    //     cout << "no such event" << endl;
    // }else{
    //     cout << "time:" << mediaLocation->getTime() << " " <<  mediaLocation->getValueAsString() << endl;
    // }
    // // remove two events
    // entityOne->removeEvent(5.2, SD_MEDIA_TIME_OFFSET);
    // entityOne->removeEvent(5.0, SD_MEDIA_GAIN);

    // // check
    // scene.dump();
    
    // // ask the entity about the extension descriptor
    // cout<< "entityOne media:location at 4.0 " << *static_cast<string*>(entityOne->getValue(4.0, SD_MEDIA_LOCATION)) << endl;
    
    // // remove the media extension
    // scene.removeExtension(SD_MEDIA);

    // // you will get error.
    // scene.removeExtension(SD_MEDIA);
    
    // // check again
    // scene.dump();
    return 0;
}

void populateMultiset(multiset<sdEvent*, sdEventCompare> eventSet){
    multiset <sdEvent*, sdEventCompare>::iterator eit = eventSet.begin();
    while(eit != eventSet.end()){
        sdEvent* event = *eit;
        cout << "   -" << event->getTime() << ":" << event->getDescriptorAsString() << " " << event->getValueAsString() << endl;
        eit++;
    }
}
