//
//  sdEntityCore.cpp
//  

#include <stdio.h>
#include <string>
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
        default:
            break;
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
            double *position = static_cast<double *>(value);
            str = doublesToString(position, 3);
            break;
        }
        case SD_ORIENTATION:{
            double *orientation = static_cast<double *>(value);
            str = doublesToString(orientation, 3);
        }
        default:
            break;
    }
    return str;
}

bool sdEventCore::setValue(EDescriptor descriptor, void* value){
    // this function is called by the constructor
    sdEvent::descriptor = descriptor;
    switch (sdEventCore::descriptor) {
        case SD_PRESENT:{
            bool *presentValue = static_cast<bool*>(value);
            sdEvent::value = static_cast<void*>(new bool(*presentValue));
            break;
        }
        case SD_POSITION:{
            double *positionValueTo = new double[3];
            double *positionValueFrom = static_cast<double*>(value);
            memcpy(positionValueTo, positionValueFrom, sizeof(double) * 3);
            sdEvent::value = static_cast<void*>(positionValueTo);
            break;
        }
        case SD_ORIENTATION:{
            double *orientationValueTo = new double[3];
            double *orientationValueFrom = static_cast<double*>(value);
            memcpy(orientationValueTo, orientationValueFrom, sizeof(double) * 3);
            sdEvent::value = static_cast<void*>(orientationValueTo);
            break;
        }
        default:{
            return false;
            break;
        }
    }
    return true;
}

bool sdEventCore::setValue(string descriptor, string value){
    // this function is called by the constructor
    
    // set descriptor
    if(descriptor == "present") sdEventCore::descriptor = SD_PRESENT;
    else if(descriptor == "position") sdEventCore::descriptor = SD_POSITION;
    else if(descriptor == "orientation") sdEventCore::descriptor = SD_ORIENTATION;
    else return false;
    
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
            stringToDoubles(value, position, 3);
            setValue(sdEventCore::descriptor, static_cast<void*>(&position));
            break;
        }
        case SD_ORIENTATION:
        {
            double orientation[3];
            stringToDoubles(value, orientation, 3);
            setValue(sdEventCore::descriptor, static_cast<void*>(&orientation));
            break;
        }
        default:{
            return false;
            break;
        }
    }
    return true;
}



/*** sdEntityCore ***/

const int sdEntityCore::numberOfCoreDescriptors = 3;
const EDescriptor sdEntityCore::coreDescriptors[] = {SD_PRESENT, SD_POSITION, SD_ORIENTATION};
const string sdEntityCore::coreDescriptorStrings[] = {string("present"), string("position"), string("orientation")};

bool sdEntityCore::isCoreDescriptor(EDescriptor descriptor){
    for(int i = 0; i< numberOfCoreDescriptors; i++){
        if(coreDescriptors[i] == descriptor) return true;
    }
    return false;
}

bool sdEntityCore::isCoreDescriptor(string descriptor){
    for(int i = 0; i< numberOfCoreDescriptors; i++){
        if(coreDescriptorStrings[i] == descriptor) return true;
    }
    return false;
}

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


sdEvent* sdEntityCore::addEvent(double time, EDescriptor descriptor, void* value){
    
    sdEvent *event = NULL;
    if(isCoreDescriptor(descriptor)){
            removeEvent(time, descriptor);
            event = static_cast<sdEvent*>(new sdEventCore(time, descriptor, value));
            eventSet.insert(event);
    }else{
            vector <sdRedirector>::iterator it =   redirectorVector.begin();
            while(it != redirectorVector.end()){
                sdRedirector rd = *it;
                if(rd.descriptor == descriptor){
                    rd.responsibleExtension->addEvent(time, descriptor, value );
                }
                it++;
            }
    }
    return event;
}

sdEvent* sdEntityCore::addEvent(string time, string descriptor, string value){

    sdEvent *event = NULL;
    if(isCoreDescriptor(descriptor)){
        removeEvent(time, descriptor);
        event = static_cast<sdEvent*>(new sdEventCore(time, descriptor, value));
        eventSet.insert(event);
    }else{
        vector <sdRedirector>::iterator it =   redirectorVector.begin();
        while(it != redirectorVector.end()){
            sdRedirector rd = *it;
            if(rd.descriptorString == descriptor){
                rd.responsibleExtension->addEvent(time, descriptor, value );
            }
            it++;
        }
    }
    return event;
}

void sdEntityCore::removeEvent(double time, EDescriptor descriptor){
    set <sdEvent*>::iterator it = eventSet.begin();
    while(it != eventSet.end()) {
        sdEvent* event = *it;
        if(event->getTime() == time){
            if(event->getDescriptor() == descriptor)
            {
                eventSet.erase(it++); // very important incrementor
            }
        }
        it++;
    }
}

void sdEntityCore::removeEvent(string time, string descriptor){
    EDescriptor des;
    if(descriptor == "present") des = SD_PRESENT;
    else if(descriptor == "position") des = SD_POSITION;
    else if(descriptor == "orientation") des = SD_ORIENTATION;
    
    removeEvent(stringToDouble(time), des);
}

void* sdEntityCore::getValue(double time, EDescriptor descriptor){

    if(isCoreDescriptor(descriptor)){
        return sdEntity::getValue(time, descriptor);
    }else{
        vector <sdRedirector>::iterator it = redirectorVector.begin();
        while (it != redirectorVector.end()) {
            sdRedirector rd = *it;
            if(rd.descriptor == descriptor){
                return rd.responsibleExtension->getValue(time, descriptor);
            }
            it++;
        }
        return NULL;
    }
}

sdEntityExtension* sdEntityCore::addExtension(EExtension extension){
    
    switch (extension) {
        case SD_MEDIA:{
            sdEntityExtensionMedia* mediaExtension = new sdEntityExtensionMedia();
            extensionVector.push_back(mediaExtension);
            for(int i = 0; i < sdEntityExtensionMedia::numberOfRelevantDescriptors; i++){
                sdRedirector rd;
                rd.descriptor = sdEntityExtensionMedia::relevantDescriptors[i];
                rd.responsibleExtension = static_cast<sdEntityExtension*>(mediaExtension);
                redirectorVector.push_back(rd);
            }
            return mediaExtension;
            break;
        }
    
        default:
            break;
    }
    return NULL;
}



