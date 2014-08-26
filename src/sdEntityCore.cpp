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
            std::vector <sdRedirector>::iterator it =   redirectorVector.begin();
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
        std::vector <sdRedirector>::iterator it =   redirectorVector.begin();
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
        
        std::multiset <sdEvent*>::iterator it = eventSet.begin();
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
        std::vector <sdRedirector>::iterator it =   redirectorVector.begin();
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
        void* vPtr = NULL;
        vPtr = sdEntity::getValue(time, descriptor);
        if(vPtr) return vPtr;
        // no event found at the specified time
        // is the interpolation of the descriptor activated?
        
        for(int i = 0; i < sdEventCore::numberOfDescriptors; i++){
            if(sdEventCore::descriptors[i].getDescriptor() == descriptor){
                if(interpolationVector[i] == SD_ENTITY_INTERPOLATION_ACTIVATED){
                    // interpolation
                    sdEvent* preEvent = getPreviousEvent(time, descriptor);
                    sdEvent* nextEvent = getNextEvent(time, descriptor);
                    if(!(preEvent && nextEvent))
                        return NULL;
                    
                    //case specific
                    switch (descriptor){
                        case SD_POSITION:
                        case SD_ORIENTATION:{
                            double* preValuePtr = static_cast<double*>(preEvent->getValue());
                            double* nextValuePtr = static_cast<double*>(nextEvent->getValue());
                            std::vector<double> preVector(preValuePtr, preValuePtr+3);
                            std::vector<double> nextVector(nextValuePtr, nextValuePtr+3);
                            double weight = (time - preEvent->getTime()) / (nextEvent->getTime() - preEvent->getTime());
                            std::vector<double> interpolatedVector = interpolate(preVector, nextVector, weight);
                            return static_cast<void *>(&interpolatedVector[0]);
                            break;
                        }
                        default:
                            return NULL;
                    }
                }
            }
        }
        return NULL;
    }else{
        std::vector <sdRedirector>::iterator it = redirectorVector.begin();
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

std::string sdEntityCore::getValueAsString(double time, EDescriptor descriptor){
    
    void* vPtr = getValue(time, descriptor);
    if(!vPtr){
        return std::string("error");
    }
    if(isCoreDescriptor(descriptor)){
        switch (descriptor) {
            case SD_POSITION:
            case SD_ORIENTATION:
                return doublesToString(static_cast<double*>(vPtr), 3);
                break;
            case SD_PRESENT:
                return (*(static_cast<bool*>(vPtr))) ? std::string("true") : std::string("false");
            default:
                break;
        }
    }else{
        // forward to extension
    }
    return std::string("error");
}

int sdEntityCore::getNumberOfEvents(){
    int sum = eventSet.size();
    std::vector<sdEntityExtension*>::iterator it = extensionVector.begin();
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

std::multiset<sdEvent*, sdEventCompare> sdEntityCore::getEventSet(void){
    std::multiset<sdEvent*, sdEventCompare> returnSet = eventSet;
    std::vector<sdEntityExtension*>::iterator it = extensionVector.begin();
    while(it != extensionVector.end()){
        sdEntityExtension* entityExtension = *it;
        std::multiset<sdEvent*, sdEventCompare> extensionSet = entityExtension->getEventSet(); 
        returnSet.insert(extensionSet.begin(), extensionSet.end());
        it++;
    }
    return returnSet;
}
    
std::multiset<sdEvent*, sdEventCompare> sdEntityCore::getEventSet(double time){
    std::multiset<sdEvent*, sdEventCompare> returnSet = sdEntity::getEventSet(time); // use non overridden function
    std::vector<sdEntityExtension*>::iterator it = extensionVector.begin();
    while(it != extensionVector.end()){
        sdEntityExtension* entityExtension = *it;
        std::multiset<sdEvent*, sdEventCompare> extensionSet = entityExtension->getEventSet(time); // function derived from the superclass
        returnSet.insert(extensionSet.begin(), extensionSet.end());
        it++;
    }
    return returnSet;
}

std::multiset<sdEvent*, sdEventCompare> sdEntityCore::getEventSet(double start, double end){
    std::multiset<sdEvent*, sdEventCompare> returnSet = sdEntity::getEventSet(start, end); // use non overridden function
    std::vector<sdEntityExtension*>::iterator it = extensionVector.begin();
    while(it != extensionVector.end()){
        sdEntityExtension* entityExtension = *it;
        std::multiset<sdEvent*, sdEventCompare> extensionSet = entityExtension->getEventSet(start, end); // function derived from the superclass
        returnSet.insert(extensionSet.begin(), extensionSet.end());
        it++;
    }
    return returnSet;
}

std::multiset<sdEvent*, sdEventCompare> sdEntityCore::getEventSet(double start, double end, EDescriptor descriptor){
    std::multiset<sdEvent*, sdEventCompare> returnSet = sdEntity::getEventSet(start, end, descriptor); // use non overridden function
    std::vector<sdEntityExtension*>::iterator it = extensionVector.begin();
    while(it != extensionVector.end()){
        sdEntityExtension* entityExtension = *it;
        std::multiset<sdEvent*, sdEventCompare> extensionSet = entityExtension->getEventSet(start, end, descriptor); // function derived from the superclass
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

std::multiset<sdEvent*, sdEventCompare> sdEntityCore::getNextEventSet(double time){
    std::multiset<sdEvent*, sdEventCompare> returnSet = sdEntity::getNextEventSet(time); //get events with core descriptors
    double nextEventTime = sdEntity::getNextEventTime(time); // its time
    if(nextEventTime < 0){ // if no core event found
        nextEventTime = DBL_MAX; 
    }
    std::vector<sdEntityExtension*>::iterator it = extensionVector.begin();
    while(it != extensionVector.end()){
        sdEntityExtension* entityExtension = *it;
        double nextExtensionEventTime = entityExtension->getNextEventTime(time);
        if(nextExtensionEventTime >= 0.0){
            if(nextExtensionEventTime < nextEventTime){ // if sooner replace returnSet
                returnSet = entityExtension->getNextEventSet(time);
                nextEventTime = nextExtensionEventTime;
            }else if(nextExtensionEventTime == nextEventTime){ // if equal, merge
                std::multiset<sdEvent*, sdEventCompare> eventSet = entityExtension->getNextEventSet(time);
                returnSet.insert(eventSet.begin(), eventSet.end());
            }
        }   
        it++;
    }
    return returnSet;
}

double sdEntityCore::getNextEventTime(double time ){
    std::multiset<sdEvent*, sdEventCompare> eventSet = getNextEventSet(time);    
    if(eventSet.empty()){
        return -1.0;
    }
    std::multiset<sdEvent*, sdEventCompare>::iterator it = eventSet.begin();
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

std::multiset<sdEvent*, sdEventCompare> sdEntityCore::getPreviousEventSet(double time){
    std::multiset<sdEvent*, sdEventCompare> returnSet = sdEntity::getPreviousEventSet(time); 
    double previousEventTime = sdEntity::getPreviousEventTime(time);
    if(previousEventTime < 0){
        previousEventTime = -1.0; //must be later replaced by the max value of double 
    }
    std::vector<sdEntityExtension*>::iterator it = extensionVector.begin();
    while(it != extensionVector.end()){
        sdEntityExtension* entityExtension = *it;
        double previousExtensionEventTime = entityExtension->getPreviousEventTime(time);
        if(previousExtensionEventTime >= 0.0){ // if exists
            if(previousExtensionEventTime > previousEventTime){ // if sooner replace returnSet
                returnSet = entityExtension->getPreviousEventSet(time);
                previousEventTime = previousExtensionEventTime;
            }else if(previousExtensionEventTime == previousEventTime){ // if equal, merge
                std::multiset<sdEvent*, sdEventCompare> eventSet = entityExtension->getPreviousEventSet(time);
                returnSet.insert(eventSet.begin(), eventSet.end());
            }
        }
        it++;
    }
    return returnSet;
}

double sdEntityCore::getPreviousEventTime(double time ){
    std::multiset<sdEvent*, sdEventCompare> eventSet = getPreviousEventSet(time);    
    if(eventSet.empty()){
        return -1.0;
    }
    std::multiset<sdEvent*, sdEventCompare>::iterator it = eventSet.begin();
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

std::multiset<sdEvent*, sdEventCompare> sdEntityCore::getFirstEventSet(){
    double firstEventTime = sdEntity::getFirstEventTime(); // ask parent, because this function is overridden
    std::multiset<sdEvent*, sdEventCompare> returnSet = sdEntity::getFirstEventSet();
    std::vector<sdEntityExtension*>::iterator it = extensionVector.begin();
    while(it != extensionVector.end()){
        sdEntityExtension* entityExtension = *it;
        double extensionFirstEventTime = entityExtension->getFirstEventTime(); // function derived from the superclass
        if(extensionFirstEventTime < firstEventTime ){
            firstEventTime = extensionFirstEventTime;
            returnSet = entityExtension->getFirstEventSet();
        }else if(extensionFirstEventTime == firstEventTime){
            std::multiset<sdEvent*, sdEventCompare> firstExtenstionSet = entityExtension->getFirstEventSet();
            returnSet.insert(firstExtenstionSet.begin(), firstExtenstionSet.end());
        }
        it++;
    }    
    return returnSet;
}

double sdEntityCore::getFirstEventTime(){
    double firstEventTime = sdEntity::getFirstEventTime(); // use parental function
    std::vector<sdEntityExtension*>::iterator it = extensionVector.begin();
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

std::multiset<sdEvent*, sdEventCompare> sdEntityCore::getLastEventSet(){
    double lastEventTime = sdEntity::getLastEventTime(); // ask parent, because this function is overridden
    std::multiset<sdEvent*, sdEventCompare> returnSet = sdEntity::getLastEventSet();
    std::vector<sdEntityExtension*>::iterator it = extensionVector.begin();
    while(it != extensionVector.end()){
        sdEntityExtension* entityExtension = *it;
        double extensionLastEventTime = entityExtension->getLastEventTime(); // function derived from the superclass
        if(extensionLastEventTime > lastEventTime ){
            lastEventTime = extensionLastEventTime;
            returnSet = entityExtension->getLastEventSet();
        }else if(extensionLastEventTime == lastEventTime){
            std::multiset<sdEvent*, sdEventCompare> lastExtenstionSet = entityExtension->getLastEventSet();
            returnSet.insert(lastExtenstionSet.begin(), lastExtenstionSet.end());
        }
        it++;
    }    
    return returnSet;
}

double sdEntityCore::getLastEventTime(){
    double lastEventTime = sdEntity::getLastEventTime(); // use parental function
    std::vector<sdEntityExtension*>::iterator it = extensionVector.begin();
    while(it != extensionVector.end()){
        double extensionLastEventTime  = (*it)->getLastEventTime();
        if(extensionLastEventTime > lastEventTime){
            lastEventTime = extensionLastEventTime; //update
        }
        it++;
    }
    return lastEventTime;
}

bool sdEntityCore::activateInterpolation(const EDescriptor descriptor){
    return sdInterpolation::activateInterpolation(descriptor, sdEventCore::descriptors, sdEventCore::numberOfDescriptors);
}

bool sdEntityCore::deactivateInterpolation(const EDescriptor descriptor ){
    return sdInterpolation::deactivateInterpolation(descriptor, sdEventCore::descriptors, sdEventCore::numberOfDescriptors);
}

bool sdEntityCore::isInterpolationActivated(const EDescriptor descriptor){
    return sdInterpolation::isInterpolationActivated(descriptor, sdEventCore::descriptors, sdEventCore::numberOfDescriptors);

}

