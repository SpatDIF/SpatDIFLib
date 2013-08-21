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
    sdEvent *event = new sdEvent;
    event->time = time;
    event->descriptor = descriptor;
    switch (descriptor) {
        case SD_POSITION:{
            float *positionValueTo = new float[3];
            float *positionValueFrom = static_cast<float*>(value);
            //copy
            positionValueTo[0] = positionValueFrom[0];
            positionValueTo[1] = positionValueFrom[1];
            positionValueTo[2] = positionValueFrom[2];
            event->value = static_cast<void*>(positionValueTo);
            break;
        }
        case SD_PRESENT:{
            bool *presentValueTo = new bool;
            bool *presentValueFrom = static_cast<bool*>(value);
            //copy
            *presentValueTo = *presentValueFrom;
            event->value = static_cast<void*>(presentValueTo);
            break;
        }
    }
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

