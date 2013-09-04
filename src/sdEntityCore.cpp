//
//  sdEntityCore.cpp
//  

#include <string>
#include <iostream>
#include "sdEntityCore.h"
#include "sdEntityExtensionMedia.h"

using namespace std;

/*** sdEventCore ***/


sdEventCore::sdEventCore(double time, EDescriptor descriptor, void* value){
    setTime(time); // declared in the super class
    setValue(descriptor, value); // private function
}

sdEventCore::sdEventCore(string time, string descriptor, string value){
    setTime(time); // declared in the super class
    setValue(descriptor, value); // private function
}

sdEventCore::~sdEventCore(){
    // we have to cast void pointer before deleting it. because void pointer deletion is undefined
    switch (descriptor) {
        case SD_PRESENT:{
            bool* valuePointer = static_cast<bool*>(value);
            delete valuePointer;
            break;
        }
        case SD_POSITION:{
            double* valuePointer = static_cast<double*>(value);
            delete valuePointer;
            break;
        }
        case SD_ORIENTATION:{
            double* valuePointer = static_cast<double*>(value);
            delete valuePointer;
            break;
        }
    }
}

string sdEventCore::getDescriptorAsString(void){
    string str;
    switch(descriptor){
        case SD_PRESENT:
            str = string("present");break;
        case SD_POSITION:
            str = string("position");break;
        case SD_ORIENTATION:
            str =string("orientation");break;
        default:
            break;
    }
    return str;
}


string sdEventCore::getValueAsString(void){
    string str;
    switch (descriptor) {
        case SD_PRESENT:{
            bool *present;
            present = static_cast<bool*>(value);
            str = *present ? string("true") : string("false");
            break;
        }
        case SD_POSITION:{
            double *position;
            ostringstream os;
            position = static_cast<double *>(value);
            os << position[0] << ' ' << position[1] << ' ' << position[2];
            str = os.str();
            break;
        }
        case SD_ORIENTATION:{
            double *orientation;
            ostringstream os;
            orientation = static_cast<double *>(value);
            os << orientation[0] << ' ' << orientation[1] << ' ' << orientation[2];
            str = os.str();
        }
    }
    return str;
}

void sdEventCore::setValue(EDescriptor descriptor, void* value){
    // this function is called by the constructor
    sdEvent::descriptor = descriptor;
    switch (sdEvent::descriptor) {
        case SD_PRESENT:{
            sdEvent::value = static_cast<void*>(new bool);
            break;
        }
        case SD_POSITION:{
            double *positionValueTo = new double[3];
            double *positionValueFrom = static_cast<double*>(value);
            positionValueTo[0] = positionValueFrom[0];
            positionValueTo[1] = positionValueFrom[1];
            positionValueTo[2] = positionValueFrom[2];
            sdEvent::value = static_cast<void*>(positionValueTo);
            break;
        }
        case SD_ORIENTATION:{
            double *orientationValueTo = new double[3];
            double *orientationValueFrom = static_cast<double*>(value);
            orientationValueTo[0] = orientationValueFrom[0];
            orientationValueTo[1] = orientationValueFrom[1];
            orientationValueTo[2] = orientationValueFrom[2];
            sdEvent::value = static_cast<void*>(orientationValueTo);
            break;
        }
        default:{
            break;
        }
    }
}

void sdEventCore::setValue(string descriptor, string value){
    // this function is called by the constructor
    
    // set descriptor
    if(descriptor == "present") sdEventCore::descriptor = SD_PRESENT;
    else if(descriptor == "position") sdEventCore::descriptor = SD_POSITION;
    else if(descriptor == "orientation") sdEventCore::descriptor = SD_ORIENTATION;
    else return;
    
    // set value
    string str;
    switch (sdEventCore::descriptor) {
        case SD_PRESENT:
        {
            bool present = value=="true" ? true : false;
            setValue(sdEventCore::descriptor, static_cast<void*>(&present));
            break;
        }
        case SD_POSITION:
        {
            double position[3];
            istringstream is(value);
            is >> position[0] >> position[1] >> position[2];
            setValue(sdEventCore::descriptor, static_cast<void*>(&position));
            break;
        }
        case SD_ORIENTATION:
        {
            double orientation[3];
            istringstream is(value);
            is >> orientation[0] >> orientation[1] >> orientation[2];
            setValue(sdEventCore::descriptor, static_cast<void*>(&orientation));
            break;
        }
            
    }
}



/*** sdEntityCore ***/

string sdEntityCore::getKindAsString(void){
    string str;
    switch(kind){
        case SD_SOURCE:
            str = string("source");break;
        case SD_SINK:
            str = string("sink");break;
        default:
            break;
    }
    return str;
}


string sdEntityCore::getTypeAsString(void){
    string str;
    switch(type){
        case SD_POINT:
            str = string("point");break;
        default:
            break;
    }
    return str;
}


sdEvent* sdEntityCore::getEvent(double time, EDescriptor descriptor){
    
    // descriptor check if not found in the core desciptor list, forward the query to attached extensions
    set <sdEvent*, sdEventCompare>::iterator it = eventSet.begin();
    while( it != eventSet.end()){
        double eventTime = (*it)->getTime();
        if(eventTime == time){ // time matched?
            EDescriptor eventDescriptpr = (*it)->getDescriptor();
            if(eventDescriptpr == descriptor){ // descriptor matched?
                return *it; // returns pointer to sdEvent
            }
        }
		it++;
    }
}

set <sdEvent*, sdEventCompare>sdEntityCore::getRangedEventSet(double start, double end){
    set <sdEvent*, sdEventCompare>rangedSet;
    set <sdEvent*, sdEventCompare>::iterator it = eventSet.begin();
    while(it != eventSet.end()){
        sdEvent* event = *it;
        if ( (event->getTime() >= start) && (event->getTime() <= end)) {
                rangedSet.insert(*it);
            }
        ++it;
    }
    return rangedSet;
}

set <sdEvent*, sdEventCompare>sdEntityCore::getFilteredEventSet(double start, double end, EDescriptor descriptor){
    
    set <sdEvent*, sdEventCompare>rangedSet;
    set <sdEvent*, sdEventCompare>::iterator it = eventSet.begin();
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

sdEvent* sdEntityCore::addEvent(double time, EDescriptor descriptor, void* value){
    
    //duplication check. if exist just delete the old one and make a new one
    set <sdEvent*>::iterator it = eventSet.begin();
    while(it != eventSet.end()) {
        sdEvent* event = *it;
        if(event->getTime() == time){
            if(event->getDescriptor() == descriptor)
            {
                eventSet.erase(it++);
            }
        }
        it++;
    }
    
    // if not in the set
    //create a new instance of sdEvent
    sdEvent *event = static_cast<sdEvent*>(new sdEventCore(time, descriptor, value));
    //and insert it in the set
    eventSet.insert(event);
    //returns a point to the newly created sdEvent
    return event;
}

sdEvent* sdEntityCore::addEvent(string time, string descriptor, string value){
    sdEvent *event = static_cast<sdEvent*>(new sdEventCore(time, descriptor, value));
    eventSet.insert(event);
    return event;
}


void* sdEntityCore::getValue(double time, EDescriptor descriptor){
    set<sdEvent*, sdEventCompare>::iterator it = eventSet.begin();

    while(it != eventSet.end()){
        sdEvent* event = *it;
        if((event->getTime() == time) && (event->getDescriptor() == descriptor)){
            return event->getValue();
        }
        ++it;
    }
    return NULL;
}


sdEntityExtension* sdEntityCore::addExtension(EExtension extension){
    
    switch (extension) {
        case SD_MEDIA:{
            sdEntityExtensionMedia* mediaExtension = new sdEntityExtensionMedia();
            extensionVector.push_back(mediaExtension);
            break;
        }
        default:
            break;
    }
    
}




