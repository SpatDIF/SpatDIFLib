
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

sdEvent* sdEntity::getEvent(double time, EDescriptor descriptor){
    multiset<sdEvent*, sdEventCompare>::iterator it = eventSet.begin();
    while(it != eventSet.end()){
        sdEvent* event = *it;
        if((event->getTime() == time) && (event->getDescriptor() == descriptor)){
            return event;
        }
        ++it;
    }
    return NULL;
}

sdEvent* sdEntity::getFirstEvent(EDescriptor descriptor){
    return getNextEvent(-1.0, descriptor);
}

multiset<sdEvent*, sdEventCompare> sdEntity::getFirstEventSet(){
    return getNextEventSet(-1.0);
}

sdEvent* sdEntity::getLastEvent(EDescriptor descriptor){
    
    sdEvent* event = getFirstEvent(descriptor);
    if(!event){
        return NULL;
    }
    while (event) {
        sdEvent* tmpEvent;
        tmpEvent = getNextEvent(event->getTime(), descriptor);
        if(tmpEvent){
            event = tmpEvent;
        }else{
            return event;
        }
    }
    return NULL;
}

double sdEntity::getFirstTimeTag(){
    sdEvent* event = *(eventSet.begin());
    if (event) {
        return event->getTime();
    }else{
        return -1.0; // no event at all
    }
}

double sdEntity::getLastTimeTag(){
    sdEvent* event = *(eventSet.rbegin());
    if (event) {
        return event->getTime();
    }else{
        return -1.0; // no event at all
    }
}

sdEvent* sdEntity::getNextEvent(double time, EDescriptor descriptor){

    multiset<sdEvent*, sdEventCompare>::iterator it = eventSet.begin();
    while(it != eventSet.end()){
        sdEvent* event = *it;
        if((event->getTime() > time) && (event->getDescriptor() == descriptor)){
            return event;
        }
        ++it;
    }
    return NULL;
}


multiset<sdEvent*, sdEventCompare> sdEntity::getNextEventSet(double time){
    
    multiset<sdEvent*, sdEventCompare> eventVector;
    multiset<sdEvent*, sdEventCompare>::iterator it = eventSet.begin();
    bool flag = false;
    double eventTime = -1.0;
    while(it != eventSet.end()){
        sdEvent* event = *it;
        if (!flag ) {
            // before the time index
            if(event->getTime() > time){
                eventVector.insert(event);
                flag = true;
                eventTime = event->getTime();
            }
        }else{
            // look for simultaneous events
            if (event->getTime() == eventTime) {
                //simultaneous event
                eventVector.insert(event);
            }else{
                return eventVector;
            }
        }
        ++it;
    }
    return eventVector;
}

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
