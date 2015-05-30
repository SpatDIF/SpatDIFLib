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

#include "sdEntityExtensionSourceWidth.h"


const EExtension sdEntityExtensionSourceWidth::extensionName = SD_SOURCE_WIDTH;
const std::string sdEntityExtensionSourceWidth::extensionNameAsString = std::string("source-width");

sdEvent* sdEntityExtensionSourceWidth::addEvent(const double time, const EDescriptor descriptor, void* const value){
    
    sdEvent *event = NULL;
    sdEntity::removeEvent(time, descriptor);
    event = static_cast<sdEvent*>(new sdEventExtensionSourceWidth(time, descriptor, value));
    eventSet.insert(event);
    return event;
    
}

sdEvent* sdEntityExtensionSourceWidth::addEvent(const std::string time, const std::string descriptor, const std::string value){

    sdEvent *event = NULL;
    removeEvent(time, descriptor);
    event = static_cast<sdEvent*>(new sdEventExtensionSourceWidth(time, descriptor, value));
    eventSet.insert(event);
    return event;
}

void sdEntityExtensionSourceWidth::removeEvent(const std::string time, const std::string descriptor){
    EDescriptor dc;
    for(int i = 0; i < sdEventExtensionSourceWidth::numberOfDescriptors; i++){
        if(sdEventExtensionSourceWidth::descriptors[i].getDescriptorAsString() == descriptor){
            dc = sdEventExtensionSourceWidth::descriptors[i].getDescriptor();
        }
    }

    sdEntity::removeEvent(stringToDouble(time), dc);
}

std::string sdEntityExtensionSourceWidth::getValueAsString(double time, EDescriptor descriptor){
    
    return std::string("error");
}


bool sdEntityExtensionSourceWidth::activateInterpolation(const EDescriptor descriptor){
    return sdInterpolation::activateInterpolation(descriptor, sdEventExtensionSourceWidth::descriptors, sdEventExtensionSourceWidth::numberOfDescriptors);
}

bool sdEntityExtensionSourceWidth::deactivateInterpolation(const EDescriptor descriptor){
    return sdInterpolation::deactivateInterpolation(descriptor, sdEventExtensionSourceWidth::descriptors, sdEventExtensionSourceWidth::numberOfDescriptors);
}

bool sdEntityExtensionSourceWidth::isInterpolationActivated(const EDescriptor descriptor){
    return sdInterpolation::isInterpolationActivated(descriptor, sdEventExtensionSourceWidth::descriptors, sdEventExtensionSourceWidth::numberOfDescriptors);
}
