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


sdEntity::~sdEntity(){
    // delete all allocated values of attached events
    eventSet.clear();
}

// current event
sdEvent* sdEntity::getEvent(const double time, EDescriptor descriptor) const{
    std::multiset<sdEvent*, sdEventCompare> currentEventSet = getEventSet(time);
    std::multiset<sdEvent*, sdEventCompare>::iterator it = currentEventSet.begin();
    while(it != currentEventSet.end()){
        sdEvent* event = *it;
        if(event->getDescriptor() == descriptor){
            return event;
        }
        ++it;
    }
    return NULL;
}

std::multiset<sdEvent*, sdEventCompare> sdEntity::getEventSet(void) const{
    return eventSet;
}

// get current event or events in range

std::multiset <sdEvent*, sdEventCompare>sdEntity::getEventSet(const double start, const double end) const{
    std::multiset <sdEvent*, sdEventCompare>rangedSet;
    std::multiset <sdEvent*, sdEventCompare>::iterator it = eventSet.begin();
    while(it != eventSet.end()){
        sdEvent* event = *it;
        if ( (event->getTime() >= start) && (event->getTime() <= end)) {
            rangedSet.insert(*it);
        }
        ++it;
    }
    return rangedSet;
}

std::multiset <sdEvent*, sdEventCompare>sdEntity::getEventSet(const double start, const double end, const EDescriptor descriptor)const {
    
    std::multiset <sdEvent*, sdEventCompare>rangedSet;
    std::multiset <sdEvent*, sdEventCompare>::iterator it = eventSet.begin();
    while(it != eventSet.end()){
        sdEvent* event = *it;
        if ( (event->getTime() >= start) && (event->getTime() <= end)) {
            if(event->getDescriptor() == descriptor){
                rangedSet.insert(*it);
            }
        }
        it++;
    }
    return rangedSet;
}

std::multiset<sdEvent*, sdEventCompare> sdEntity::getEventSet(const double time) const{
    return createEventSet(time, SD_ENTITY_CURRENT);
}

sdEvent* sdEntity::getNextEvent(const double time, EDescriptor descriptor)const{
    std::multiset<sdEvent*, sdEventCompare>::iterator it = eventSet.begin();
    while(it != eventSet.end()){
        sdEvent* event = *it;
        if((event->getDescriptor() == descriptor) && (event->getTime() > time )){
            return event;
        }
        it++;
    }
    return NULL;
}

std::multiset<sdEvent*, sdEventCompare> sdEntity::getNextEventSet(const double time) const{
    return createEventSet(time, SD_ENTITY_NEXT);
}

double sdEntity::getNextEventTime(const double time) const{
    std::multiset<sdEvent*, sdEventCompare> nextEventSet = getNextEventSet(time);
    if(nextEventSet.size() == 0){
        return -1.0; // indicate no next event
    }
    std::multiset<sdEvent*, sdEventCompare>::iterator it = nextEventSet.begin();
    sdEvent* event = *it;
    return event->getTime();
}

// previous event
sdEvent* sdEntity::getPreviousEvent(const double time, const EDescriptor descriptor) const{
    std::multiset<sdEvent*, sdEventCompare>::reverse_iterator rit = eventSet.rbegin();
    while(rit != eventSet.rend()){
        sdEvent* event = *rit;
        if((event->getDescriptor() == descriptor) && (event->getTime() < time )){
            return event;
        }
        rit++;
    }
    return NULL;
}

std::multiset<sdEvent*, sdEventCompare> sdEntity::getPreviousEventSet(const double time) const{
    return createEventSet(time, SD_ENTITY_PREVIOUS);
}

double sdEntity::getPreviousEventTime(const double time) const{
    std::multiset<sdEvent*, sdEventCompare> previousEventSet = getPreviousEventSet(time);
    std::multiset<sdEvent*, sdEventCompare>::iterator it = previousEventSet.begin();
    sdEvent* event = *it;
    return event->getTime();
}
// first event
sdEvent* sdEntity::getFirstEvent(const EDescriptor descriptor) const{
    return getNextEvent(-1.0, descriptor);
}

std::multiset<sdEvent*, sdEventCompare> sdEntity::getFirstEventSet() const{
    return getNextEventSet(-1.0);
}

double sdEntity::getFirstEventTime() const{
    sdEvent* event = *(eventSet.begin());
    if (event) {
        return event->getTime();
    }else{
        return -1.0; // no event at all
    }
}

// last event
sdEvent* sdEntity::getLastEvent(EDescriptor descriptor) const{
    return getPreviousEvent(DBL_MAX, descriptor);
}

std::multiset<sdEvent*, sdEventCompare> sdEntity::getLastEventSet() const{
    return getPreviousEventSet(DBL_MAX);
}

double sdEntity::getLastEventTime() const{
    sdEvent* event = *(eventSet.rbegin());
    if (event) {
        return event->getTime();
    }else{
        return -1.0; // no event at all
    }
}

// number of events
int sdEntity::getNumberOfEvents() const{
    return static_cast<int>(eventSet.size());
}

void sdEntity::removeEvent(double time, EDescriptor descriptor){
    std::multiset<sdEvent*, sdEventCompare>::iterator it = eventSet.begin();
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

std::multiset<sdEvent*, sdEventCompare> sdEntity::createEventSet(const double time, const EMode mode ) const{
    std::multiset<sdEvent*, sdEventCompare> createdSet;
    std::multiset<sdEvent*, sdEventCompare>::iterator it;
    std::multiset<sdEvent*, sdEventCompare>::reverse_iterator rt;
    bool flag = false;
    double eventTime = 0.0;
    switch (mode) {
        case SD_ENTITY_NEXT:
            it = eventSet.begin();
            while(it != eventSet.end()){
                sdEvent* event = *it;
                if (!flag ) {
                    if(event->getTime() > time){
                        flag = true; // find first event whose time tag is greater than time
                        eventTime = event->getTime(); // store the time of event as eventTime
                        createdSet.insert(event);
                    }
                }else{
                    if(event->getTime() == eventTime) {
                        createdSet.insert(event);
                    }
                }
                ++it;
            }
            break;
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
