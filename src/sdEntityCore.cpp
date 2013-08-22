//
//  sdEntityCore.cpp
//  

#include <string>
#include <iostream>
#include "sdEntityCore.h"

void* sdEntityCore::getValue(float time, EDescriptor descriptor){
    std::set<sdEvent*, sdEventCompare>::iterator it = eventSet.begin();

    while(it != eventSet.end()){
        sdEvent* event = *it;
        if((event->time == time) && (event->descriptor == descriptor)){
            return event->value;
        }
        ++it;
    }
    return NULL;
}


std::set <sdEvent*, sdEventCompare>sdEntityCore::getValue(float start, float end, EDescriptor descriptor){
    
    std::set <sdEvent*, sdEventCompare>rangedSet;
    std::set <sdEvent*, sdEventCompare>::iterator it = eventSet.begin();
    while(it != eventSet.end()){
        sdEvent* event = *it;
        if ( (event->time >= start) && (event->time <= end) && (event->descriptor == descriptor)) {
            rangedSet.insert(*it);
        }
        ++it;
    }
    return rangedSet;
}
