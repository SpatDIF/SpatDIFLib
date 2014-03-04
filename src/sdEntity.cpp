/** @file
 *
 * @ingroup spatdiflib
 *
 * @brief
 *
 * @details
 *
 * @authors Chikashi Miyama, Trond Lossius
 *
 * @copyright Copyright (C) 2013 - 2014 Zurich University of the Arts, Institute for Computer Music and Sound Technology @n
 * This code is licensed under the terms of the "New BSD License" @n
 * http://creativecommons.org/licenses/BSD/
 */

#include <cfloat>
#include "sdEntity.h"

void sdEvent::set(double time, EDescriptor descriptor, void* value){
    setTime(time);
    setValue(descriptor, value);
}

void sdEvent::setTime(double time){
    sdEvent::time = time;
}

void sdEvent::setTime(string timeString){
    sdEvent::time = stringToDouble(timeString);
}

double sdEvent::getTime(void){
    return time;
}

string sdEvent::getTimeAsString(void){
    return doubleToString(time);
}

EDescriptor sdEvent::getDescriptor(void){
    return descriptor;
}

void* sdEvent::getValue(void){
    return value;
}


sdEntity::~sdEntity(){
    // delete all allocated values of attached events
    eventSet.clear();
}


multiset<sdEvent*, sdEventCompare> sdEntity::getEventSet(void){
    return eventSet;
}

// get current event or events in range

multiset <sdEvent*, sdEventCompare>sdEntity::getEventSet(double start, double end){
    multiset <sdEvent*, sdEventCompare>rangedSet;
    multiset <sdEvent*, sdEventCompare>::iterator it = eventSet.begin();
    while(it != eventSet.end()){
        sdEvent* event = *it;
        if ( (event->getTime() >= start) && (event->getTime() <= end)) {
            rangedSet.insert(*it);
        }
        ++it;
    }
    return rangedSet;
}

multiset <sdEvent*, sdEventCompare>sdEntity::getEventSet(double start, double end, EDescriptor descriptor){
    
    multiset <sdEvent*, sdEventCompare>rangedSet;
    multiset <sdEvent*, sdEventCompare>::iterator it = eventSet.begin();
    while(it != eventSet.end()){
        sdEvent* event = *it;
        if ( (event->getTime() >= start) && (event->getTime() <= end)) {
            if(event->getDescriptor() == descriptor){
                rangedSet.insert(*it);
            }
        }
        ++it;
    }
    return rangedSet;
}

// current event
sdEvent* sdEntity::getEvent(double time, EDescriptor descriptor){
    multiset<sdEvent*, sdEventCompare> currentEventSet = getEventSet(time);
    multiset<sdEvent*, sdEventCompare>::iterator it = currentEventSet.begin();
    while(it != currentEventSet.end()){
        sdEvent* event = *it;
        if(event->getDescriptor() == descriptor){
            return event;
        }
        ++it;
    }
    return NULL;
}

multiset<sdEvent*, sdEventCompare> sdEntity::getEventSet(double time){
    return createEventSet(time, SD_ENTITY_CURRENT);
}

// first event
sdEvent* sdEntity::getFirstEvent(EDescriptor descriptor){
    return getNextEvent(-1.0, descriptor);
}

multiset<sdEvent*, sdEventCompare> sdEntity::getFirstEventSet(){
    return getNextEventSet(-1.0);
}

double sdEntity::getFirstEventTime(){
    sdEvent* event = *(eventSet.begin());
    if (event) {
        return event->getTime();
    }else{
        return -1.0; // no event at all
    }
}

// last event
sdEvent* sdEntity::getLastEvent(EDescriptor descriptor){
    return getPreviousEvent(DBL_MAX, descriptor);
}

multiset<sdEvent*, sdEventCompare> sdEntity::getLastEventSet(){
    return getPreviousEventSet(DBL_MAX);
}

double sdEntity::getLastEventTime(){
    sdEvent* event = *(eventSet.rbegin());
    if (event) {
        return event->getTime();
    }else{
        return -1.0; // no event at all
    }
}

// next event
sdEvent* sdEntity::getNextEvent(double time, EDescriptor descriptor){
    multiset<sdEvent*, sdEventCompare> nextEventSet = getNextEventSet(time);
    multiset<sdEvent*, sdEventCompare>::iterator it = nextEventSet.begin();
    while(it != nextEventSet.end()){
        sdEvent* event = *it;
        if((event->getDescriptor() == descriptor)){
            return event;
        }
        ++it;
    }
    return NULL;
}

multiset<sdEvent*, sdEventCompare> sdEntity::getNextEventSet(double time){
    return createEventSet(time, SD_ENTITY_NEXT);
}

double sdEntity::getNextEventTime(double time){
    multiset<sdEvent*, sdEventCompare> nextEventSet = getNextEventSet(time);
    multiset<sdEvent*, sdEventCompare>::iterator it = nextEventSet.begin();
    sdEvent* event = *it;
    return event->getTime();
}

// previous event
sdEvent* sdEntity::getPreviousEvent(double time, EDescriptor descriptor){
    multiset<sdEvent*, sdEventCompare> previousEventSet = getPreviousEventSet(time);
    multiset<sdEvent*, sdEventCompare>::iterator it = previousEventSet.begin();
    while(it != previousEventSet.end()){
        sdEvent* event = *it;
        if((event->getDescriptor() == descriptor)){
            return event;
        }
        ++it;
    }
    return NULL;
}

multiset<sdEvent*, sdEventCompare> sdEntity::getPreviousEventSet(double time){
    return createEventSet(time, SD_ENTITY_PREVIOUS);
}

double sdEntity::getPreviousEventTime(double time){
    multiset<sdEvent*, sdEventCompare> previousEventSet = getPreviousEventSet(time);
    multiset<sdEvent*, sdEventCompare>::iterator it = previousEventSet.begin();
    sdEvent* event = *it;
    return event->getTime();
}

// number of events
int sdEntity::getNumberOfEvents(){
    return static_cast<int>(eventSet.size());
}

void sdEntity::removeEvent(double time, EDescriptor descriptor){
    multiset<sdEvent*, sdEventCompare>::iterator it = eventSet.begin();
    while(it != eventSet.end()){
        sdEvent* event = *it;
        if((event->getTime() == time) && (event->getDescriptor() == descriptor)){
            eventSet.erase(it++);
        }else{
            ++it;
        }
    }
}

void sdEntity::removeAllEvents(){
    eventSet.clear();
}

void* sdEntity::getValue(double time, EDescriptor descriptor){
    sdEvent *evt = getEvent(time, descriptor);
    if(evt){
        return evt->getValue();
    }
    return NULL;
}

void* sdEntity::getNextValue(double time, EDescriptor descriptor){
    sdEvent *evt = getNextEvent(time, descriptor);
    if(evt){
        return evt->getValue();
    }
    return NULL;
}


void* sdEntity::getPreviousValue(double time, EDescriptor descriptor){
    sdEvent *evt = getPreviousEvent(time, descriptor);
    if(evt){
        return evt->getValue();
    }
    return NULL;
}

multiset<sdEvent*, sdEventCompare> sdEntity::createEventSet(double time, EMode mode ){
    multiset<sdEvent*, sdEventCompare> createdSet;
    multiset<sdEvent*, sdEventCompare>::iterator it;
    multiset<sdEvent*, sdEventCompare>::reverse_iterator rt;
    bool flag = false;
    double eventTime = 0.0;
    switch (mode) {
        case SD_ENTITY_PREVIOUS:
            rt = eventSet.rbegin();
            while(rt != eventSet.rend()){
                sdEvent* event = *rt;
                if (!flag ) {
                    if(event->getTime() < time){
                        createdSet.insert(event);
                        flag = true;
                        eventTime = event->getTime();
                    }
                }else{
                    if(event->getTime() == eventTime) {
                            createdSet.insert(event);
                        }
                    }
                ++rt;
            }
            break;
        case SD_ENTITY_NEXT:
            it = eventSet.begin();
            while(it != eventSet.end()){
                sdEvent* event = *it;
                if (!flag ) {
                    if(event->getTime() > time){
                        createdSet.insert(event);
                        flag = true;
                        eventTime = event->getTime();
                    }
                }else{
                    if(event->getTime() == eventTime) {
                        createdSet.insert(event);
                    }
                }
                ++it;
            }
            break;
        case SD_ENTITY_CURRENT:
            it = eventSet.begin();
            while(it != eventSet.end()){
                sdEvent* event = *it;
                if (!flag ) {
                    if(event->getTime() == time){
                        createdSet.insert(event);
                        flag = true;
                        eventTime = event->getTime();
                    }
                }else{
                    if(event->getTime() == eventTime) {
                        createdSet.insert(event);
                    }
                }
                ++it;
            }
            break;
    }
    return createdSet;
}
