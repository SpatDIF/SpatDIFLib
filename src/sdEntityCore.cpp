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
        if((event->getTime() == time) && (event->getDescriptor() == descriptor)){
            return event->getValue();
        }
        ++it;
    }
    return NULL;
}

std::set <sdEvent*, sdEventCompare>sdEntityCore::getFilteredEventSet(float start, float end, EDescriptor descriptor){
    
    std::set <sdEvent*, sdEventCompare>rangedSet;
    std::set <sdEvent*, sdEventCompare>::iterator it = eventSet.begin();
    while(it != eventSet.end()){
        sdEvent* event = *it;
        if ( (event->getTime() >= start) && (event->getTime() <= end) && (event->getDescriptor() == descriptor)) {
            rangedSet.insert(*it);
        }
        ++it;
    }
    return rangedSet;
}
