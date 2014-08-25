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

#include "sdEntityExtensionMedia.h"


const EExtension sdEntityExtensionMedia::extensionName = SD_MEDIA;
const std::string sdEntityExtensionMedia::extensionNameAsString = std::string("media");

sdEvent* sdEntityExtensionMedia::addEvent(const double time, const EDescriptor descriptor, void* const value){
    
    sdEvent *event = NULL;
    sdEntity::removeEvent(time, descriptor);
    event = static_cast<sdEvent*>(new sdEventExtensionMedia(time, descriptor, value));
    eventSet.insert(event);
    return event;
    
}

sdEvent* sdEntityExtensionMedia::addEvent(const std::string time, const std::string descriptor, const std::string value){

    sdEvent *event = NULL;
    removeEvent(time, descriptor);
    event = static_cast<sdEvent*>(new sdEventExtensionMedia(time, descriptor, value));
    eventSet.insert(event);
    return event;
}

void sdEntityExtensionMedia::removeEvent(const std::string time, const std::string descriptor){
    EDescriptor dc;
    for(int i = 0; i < sdEventExtensionMedia::numberOfDescriptors; i++){
        if(sdEventExtensionMedia::descriptors[i].getDescriptorAsString() == descriptor){
            dc = sdEventExtensionMedia::descriptors[i].getDescriptor();
        }
    }

    sdEntity::removeEvent(stringToDouble(time), dc);
}

bool sdEntityExtensionMedia::activateInterpolation(const EDescriptor descriptor){
    return true;
}

bool sdEntityExtensionMedia::deactivateInterpolation(const EDescriptor descriptor){
    return true;
}

bool sdEntityExtensionMedia::isInterpolationActivated(const EDescriptor descriptor){
    return true;
}
