//
//  sdEntityCore.cpp
//  

#include <stdio.h>
#include <string>
#include "sdConst.h"
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
    return descriptorToString(descriptor,
                              sdEntityCore::coreDescriptorStrings,
                              sdEntityCore::coreDescriptors,
                              sdEntityCore::numberOfCoreDescriptors);
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
    // set descriptor
    sdEventCore::descriptor = stringToDescriptor(descriptor,
                                                 sdEntityCore::coreDescriptorStrings,
                                                 sdEntityCore::coreDescriptors,
                                                 sdEntityCore::numberOfCoreDescriptors);
    
    // set value
    string str;
    switch (sdEventCore::descriptor) {
        case SD_PRESENT:
        {
            bool present = stringToBool(value);
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
const EDescriptor sdEntityCore::coreDescriptors[] = {
    SD_PRESENT,
    SD_POSITION,
    SD_ORIENTATION
};
const string sdEntityCore::coreDescriptorStrings[] = {
    string("present"),
    string("position"),
    string("orientation")
};

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
    // if core descriptor
    if(isCoreDescriptor(descriptor)){
        multiset <sdEvent*>::iterator it = eventSet.begin();
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
    }else{
        // if extended descriptor
        vector <sdRedirector>::iterator it =   redirectorVector.begin();
        while(it != redirectorVector.end()){
            sdRedirector rd = *it;
            if(rd.descriptor == descriptor){
                rd.responsibleExtension->removeEvent(time, descriptor);
            }
            it++;
        }
        
    }
}

void sdEntityCore::removeEvent(string time, string descriptor){
    // convert and call the function above
    removeEvent(stringToDouble(time),
                stringToDescriptor(descriptor,
                                   sdEntityCore::coreDescriptorStrings,
                                   sdEntityCore::coreDescriptors,
                                   sdEntityCore::numberOfCoreDescriptors ));
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
    sdEntityExtension *ext= getExtension(extension);

    switch (extension) {
        case SD_MEDIA:{
            sdEntityExtensionMedia* mediaExtension = new sdEntityExtensionMedia();
            extensionVector.push_back(mediaExtension);
            for(int i = 0; i < sdEntityExtensionMedia::numberOfRelevantDescriptors; i++){
                sdRedirector rd;
                rd.descriptor = sdEntityExtensionMedia::relevantDescriptors[i];
                rd.descriptorString = sdEntityExtensionMedia::relevantDescriptorStrings[i];
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

void sdEntityCore::removeExtension(EExtension extension){

    vector <sdEntityExtension*>::iterator it =  extensionVector.begin();
    while(it != extensionVector.end()){
        if((*it)->getExtensionName() == extension){
            extensionVector.erase(it);
            return;
        }
        it++;
    }
}

sdEntityExtension* sdEntityCore::getExtension(EExtension extension){
    vector <sdEntityExtension*>::iterator it = extensionVector.begin();
    if(it != extensionVector.end()){
        sdEntityExtension* ext = *it;
        if(ext->getExtensionName() == extension){
            return *it;
        }
        it++;
    }
    return NULL;
}


sdEvent* sdEntityCore::getEvent(double time, EDescriptor descriptor){
    if(isCoreDescriptor(descriptor)){
        return sdEntity::getEvent(time, descriptor);
    }else{
        vector <sdRedirector>::iterator it = redirectorVector.begin();
        while (it != redirectorVector.end()) {
            sdRedirector rd = *it;
            if(rd.descriptor == descriptor){
                return rd.responsibleExtension->getEvent(time, descriptor);
            }
            it++;
        }
        return NULL;
    }
}

sdEvent* sdEntityCore::getNextEvent(double time, EDescriptor descriptor){
    if(isCoreDescriptor(descriptor)){
        return sdEntity::getNextEvent(time, descriptor);
    }else{
        vector <sdRedirector>::iterator it = redirectorVector.begin();
        while (it != redirectorVector.end()) {
            sdRedirector rd = *it;
            if(rd.descriptor == descriptor){
                return rd.responsibleExtension->getNextEvent(time, descriptor);
            }
            it++;
        }
        return NULL;
    }
}


