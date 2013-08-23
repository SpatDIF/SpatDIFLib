//
//  sdEntity.cpp
//  

#include <string>
#include <iostream>
#include <set>
#include "sdEntity.h"

sdEntity::sdEntity(){
}

sdEntity::~sdEntity(){
}

unsigned int sdEntity::getNumberOfEvents(){
    return eventSet.size();
}

sdEvent* sdEntity::addEvent(float time, EDescriptor descriptor, void* value){
    //create a new instance of sdEvent
    sdEvent *event = new sdEvent(time, descriptor, value);
    //and insert it in the set
    eventSet.insert(event);
    //returns a point to the newly created sdEvent
    return event;
}

sdEvent* sdEntity::addEvent(string time, string descriptor, string value){

    sdEvent *event = new sdEvent(time, descriptor, value);
    eventSet.insert(event);
    return event;
}

sdEvent* sdEntity::getEvent(float time, EDescriptor descriptor){
    
    // descriptor check if not found in the core desciptor list, forward the query to attached extensions
    
    set <sdEvent*, sdEventCompare>::iterator it = eventSet.begin();
    while( it != eventSet.end()){
        float eventTime = (*it)->time;
        if(eventTime == time){
            EDescriptor eventDescriptpr = (*it)->descriptor;
            if(eventDescriptpr == descriptor){
                return *it; // returns pointer to sdEvent
            }
        }
		it++;
    }
}

void sdEntity::removeAllEvents(){
    eventSet.clear();
}

