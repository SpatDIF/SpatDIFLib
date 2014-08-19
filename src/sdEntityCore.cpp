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

#include <string>
#include <cfloat>
#include "sdEntityCore.h"
#include "sdEntityExtensionMedia.h"

std::string sdEntityCore::getKindAsString(void){
    std::string str;
    switch(kind){
        case SD_SOURCE:
            str = std::string("source");break;
        case SD_SINK:
            str = std::string("sink");break;
        default:
            break;
    }
    return str;
}

std::string sdEntityCore::getTypeAsString(void){
    std::string str;
    switch(type){
        case SD_POINT:
            str = std::string("point");break;
        default:
            break;
    }
    return str;
}

sdEvent* sdEntityCore::addEvent(double time, EDescriptor descriptor, void* const value){
    
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

sdEvent* sdEntityCore::addEvent(std::string time, std::string descriptor, std::string value){

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
                    eventSet.erase(it); // very important incrementor
                    break;
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

void sdEntityCore::removeEvent(std::string time, std::string descriptor){
    EDescriptor ds = SD_ERROR;
    for(int i = 0; i < sdEventCore::numberOfDescriptors; i++){
        if(sdEventCore::descriptors[i].getDescriptorAsString() == descriptor){
            ds = sdEventCore::descriptors[i].getDescriptor();
        }
    }
    // convert and call the function above
    removeEvent(stringToDouble(time), ds);
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
            for(int i = 0; i < sdEntityExtensionMedia::numberOfDescriptors; i++){
                sdRedirector rd;
                rd.descriptor = sdEntityExtensionMedia::descriptors[i].getDescriptor();
                rd.descriptorString = sdEntityExtensionMedia::descriptors[i].getDescriptorAsString();
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

sdEntityExtension* sdEntityCore::getResponsibleExtension(EDescriptor descriptor){
    vector <sdRedirector>::iterator it = redirectorVector.begin();
    while(it != redirectorVector.end()){
        sdRedirector rd = *it;
        if(rd.descriptor == descriptor){
            return rd.responsibleExtension;
        }
        it++;
    }
    return NULL;
}

int sdEntityCore::getNumberOfEvents(){
    int sum = eventSet.size();
    vector<sdEntityExtension*>::iterator it = extensionVector.begin();
    while(it != extensionVector.end()){
        sdEntityExtension* entityExtension = *it;
        sum += entityExtension->getEventSet().size(); 
        it++;
    }
    return sum;
}

sdEvent* sdEntityCore::getEvent(double time, EDescriptor descriptor){
    if(isCoreDescriptor(descriptor)){
        return sdEntity::getEvent(time, descriptor);
    }else{
        sdEntityExtension* extension = getResponsibleExtension(descriptor);
        if (!extension) {
            return NULL;
        }
        return extension->getEvent(time, descriptor);
    }
    return NULL;
}

multiset<sdEvent*, sdEventCompare> sdEntityCore::getEventSet(void){
    multiset<sdEvent*, sdEventCompare> returnSet = eventSet;
    vector<sdEntityExtension*>::iterator it = extensionVector.begin();
    while(it != extensionVector.end()){
        sdEntityExtension* entityExtension = *it;
        multiset<sdEvent*, sdEventCompare> extensionSet = entityExtension->getEventSet(); 
        returnSet.insert(extensionSet.begin(), extensionSet.end());
        it++;
    }
    return returnSet;
}
    
multiset<sdEvent*, sdEventCompare> sdEntityCore::getEventSet(double time){
    multiset<sdEvent*, sdEventCompare> returnSet = sdEntity::getEventSet(time); // use non overrided function
    vector<sdEntityExtension*>::iterator it = extensionVector.begin();
    while(it != extensionVector.end()){
        sdEntityExtension* entityExtension = *it;
        multiset<sdEvent*, sdEventCompare> extensionSet = entityExtension->getEventSet(time); // function derived from the superclass
        returnSet.insert(extensionSet.begin(), extensionSet.end());
        it++;
    }
    return returnSet;
}

multiset<sdEvent*, sdEventCompare> sdEntityCore::getEventSet(double start, double end){
    multiset<sdEvent*, sdEventCompare> returnSet = sdEntity::getEventSet(start, end); // use non overrided function
    vector<sdEntityExtension*>::iterator it = extensionVector.begin();
    while(it != extensionVector.end()){
        sdEntityExtension* entityExtension = *it;
        multiset<sdEvent*, sdEventCompare> extensionSet = entityExtension->getEventSet(start, end); // function derived from the superclass
        returnSet.insert(extensionSet.begin(), extensionSet.end());
        it++;
    }
    return returnSet;
}

multiset<sdEvent*, sdEventCompare> sdEntityCore::getEventSet(double start, double end, EDescriptor descriptor){
    multiset<sdEvent*, sdEventCompare> returnSet = sdEntity::getEventSet(start, end, descriptor); // use non overrided function
    vector<sdEntityExtension*>::iterator it = extensionVector.begin();
    while(it != extensionVector.end()){
        sdEntityExtension* entityExtension = *it;
        multiset<sdEvent*, sdEventCompare> extensionSet = entityExtension->getEventSet(start, end, descriptor); // function derived from the superclass
        returnSet.insert(extensionSet.begin(), extensionSet.end());
        it++;
    }
    return returnSet; 
}

sdEvent* sdEntityCore::getNextEvent(double time, EDescriptor descriptor){
    if(isCoreDescriptor(descriptor)){
        return sdEntity::getNextEvent(time, descriptor);
    }else{
        sdEntityExtension* extension = getResponsibleExtension(descriptor);
        if (!extension) {
            return NULL;
        }
        return extension->getNextEvent(time, descriptor);
    }
    return NULL;
}

multiset<sdEvent*, sdEventCompare> sdEntityCore::getNextEventSet(double time){
    multiset<sdEvent*, sdEventCompare> returnSet = sdEntity::getNextEventSet(time); //get events with core descriptors
    double nextEventTime = sdEntity::getNextEventTime(time); // its time
    if(nextEventTime < 0){ // if no core event found
        nextEventTime = DBL_MAX; 
    }
    vector<sdEntityExtension*>::iterator it = extensionVector.begin();
    while(it != extensionVector.end()){
        sdEntityExtension* entityExtension = *it;
        double nextExtensionEventTime = entityExtension->getNextEventTime(time);
        if(nextExtensionEventTime >= 0.0){
            if(nextExtensionEventTime < nextEventTime){ // if sooner replace returnSet
                returnSet = entityExtension->getNextEventSet(time);
                nextEventTime = nextExtensionEventTime;
            }else if(nextExtensionEventTime == nextEventTime){ // if equal, merge
                multiset<sdEvent*, sdEventCompare> eventSet = entityExtension->getNextEventSet(time);
                returnSet.insert(eventSet.begin(), eventSet.end());
            }
        }   
        it++;
    }
    return returnSet;
}

double sdEntityCore::getNextEventTime(double time ){
    multiset<sdEvent*, sdEventCompare> eventSet = getNextEventSet(time);    
    if(eventSet.empty()){
        return -1.0;
    }
    multiset<sdEvent*, sdEventCompare>::iterator it = eventSet.begin();
    sdEvent* event = *it;
    return event->getTime();
}

sdEvent* sdEntityCore::getPreviousEvent(double time, EDescriptor descriptor){
    if(isCoreDescriptor(descriptor)){
        return sdEntity::getPreviousEvent(time, descriptor);
    }else{
        sdEntityExtension* extension = getResponsibleExtension(descriptor);
        if (!extension) {
            return NULL;
        }
        return extension->getPreviousEvent(time, descriptor);
    }
    return NULL;
}

multiset<sdEvent*, sdEventCompare> sdEntityCore::getPreviousEventSet(double time){
    multiset<sdEvent*, sdEventCompare> returnSet = sdEntity::getPreviousEventSet(time); 
    double previousEventTime = sdEntity::getPreviousEventTime(time);
    if(previousEventTime < 0){
        previousEventTime = -1.0; //must be later replaced by the max value of double 
    }
    vector<sdEntityExtension*>::iterator it = extensionVector.begin();
    while(it != extensionVector.end()){
        sdEntityExtension* entityExtension = *it;
        double previousExtensionEventTime = entityExtension->getPreviousEventTime(time);
        if(previousExtensionEventTime >= 0.0){ // if exists
            if(previousExtensionEventTime > previousEventTime){ // if sooner replace returnSet
                returnSet = entityExtension->getPreviousEventSet(time);
                previousEventTime = previousExtensionEventTime;
            }else if(previousExtensionEventTime == previousEventTime){ // if equal, merge
                multiset<sdEvent*, sdEventCompare> eventSet = entityExtension->getPreviousEventSet(time);
                returnSet.insert(eventSet.begin(), eventSet.end());
            }
        }
        it++;
    }
    return returnSet;
}

double sdEntityCore::getPreviousEventTime(double time ){
    multiset<sdEvent*, sdEventCompare> eventSet = getPreviousEventSet(time);    
    if(eventSet.empty()){
        return -1.0;
    }
    multiset<sdEvent*, sdEventCompare>::iterator it = eventSet.begin();
    sdEvent* event = *it;
    return event->getTime();
}


sdEvent* sdEntityCore::getFirstEvent(EDescriptor descriptor){
    if(isCoreDescriptor(descriptor)){
        return sdEntity::getFirstEvent(descriptor);
    }else{
        sdEntityExtension* extension = getResponsibleExtension(descriptor);
        if (!extension) {
            return NULL;
        }
        return extension->getFirstEvent(descriptor);
    }
    return NULL;
}

multiset<sdEvent*, sdEventCompare> sdEntityCore::getFirstEventSet(){
    double firstEventTime = sdEntity::getFirstEventTime(); // ask parent, because this function is overrided
    multiset<sdEvent*, sdEventCompare> returnSet = sdEntity::getFirstEventSet();
    vector<sdEntityExtension*>::iterator it = extensionVector.begin();
    while(it != extensionVector.end()){
        sdEntityExtension* entityExtension = *it;
        double extensionFirstEventTime = entityExtension->getFirstEventTime(); // function derived from the superclass
        if(extensionFirstEventTime < firstEventTime ){
            firstEventTime = extensionFirstEventTime;
            returnSet = entityExtension->getFirstEventSet();
        }else if(extensionFirstEventTime == firstEventTime){
            multiset<sdEvent*, sdEventCompare> firstExtenstionSet = entityExtension->getFirstEventSet();
            returnSet.insert(firstExtenstionSet.begin(), firstExtenstionSet.end());
        }
        it++;
    }    
    return returnSet;
}

double sdEntityCore::getFirstEventTime(){
    double firstEventTime = sdEntity::getFirstEventTime(); // use parental function
    vector<sdEntityExtension*>::iterator it = extensionVector.begin();
    while(it != extensionVector.end()){
        double extensionFirstEventTime  = (*it)->getFirstEventTime();
        if(extensionFirstEventTime < firstEventTime){
            firstEventTime = extensionFirstEventTime; //update
        }
        it++;
    }
    return firstEventTime;
}

sdEvent* sdEntityCore::getLastEvent(EDescriptor descriptor){
    if(isCoreDescriptor(descriptor)){
        return sdEntity::getLastEvent(descriptor);
    }else{
        sdEntityExtension* extension = getResponsibleExtension(descriptor);
        if (!extension) {
            return NULL;
        }
        return extension->getLastEvent(descriptor);
    }
    return NULL;
}

multiset<sdEvent*, sdEventCompare> sdEntityCore::getLastEventSet(){
    double lastEventTime = sdEntity::getLastEventTime(); // ask parent, because this function is overrided
    multiset<sdEvent*, sdEventCompare> returnSet = sdEntity::getLastEventSet();
    vector<sdEntityExtension*>::iterator it = extensionVector.begin();
    while(it != extensionVector.end()){
        sdEntityExtension* entityExtension = *it;
        double extensionLastEventTime = entityExtension->getLastEventTime(); // function derived from the superclass
        if(extensionLastEventTime > lastEventTime ){
            lastEventTime = extensionLastEventTime;
            returnSet = entityExtension->getLastEventSet();
        }else if(extensionLastEventTime == lastEventTime){
            multiset<sdEvent*, sdEventCompare> lastExtenstionSet = entityExtension->getLastEventSet();
            returnSet.insert(lastExtenstionSet.begin(), lastExtenstionSet.end());
        }
        it++;
    }    
    return returnSet;
}

double sdEntityCore::getLastEventTime(){
    double lastEventTime = sdEntity::getLastEventTime(); // use parental function
    vector<sdEntityExtension*>::iterator it = extensionVector.begin();
    while(it != extensionVector.end()){
        double extensionLastEventTime  = (*it)->getLastEventTime();
        if(extensionLastEventTime > lastEventTime){
            lastEventTime = extensionLastEventTime; //update
        }
        it++;
    }
    return lastEventTime;
}


