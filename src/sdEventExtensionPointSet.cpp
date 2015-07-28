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

#include "sdEventExtensionPointSet.h"


const int sdEventExtensionPointSet::numberOfDescriptors = 1;
const sdDescriptor sdEventExtensionPointSet::descriptors[sdEventExtensionPointSet::numberOfDescriptors] = {
    sdDescriptor(SD_POINT_SET_NAME, std::string("name"), true)
};

// constructors
sdEventExtensionPointSet::sdEventExtensionPointSet(double time, EDescriptor descriptor, void* value){
    setTime(time); // declared in sdEntity.h
    setValue(descriptor, value);
}

sdEventExtensionPointSet::sdEventExtensionPointSet(std::string time, std::string descriptor, std::string value){
    setTime(time);
    setValue(descriptor, value);
}

// destructor
sdEventExtensionPointSet::~sdEventExtensionPointSet(){
    
}

// returns descriptor of the event as a string
std::string sdEventExtensionPointSet::getDescriptorAsString(void) const{
    for(int i = 0; i< sdEventExtensionPointSet::numberOfDescriptors; i++){
        if(sdEventExtensionPointSet::descriptors[i].getDescriptor() == descriptor){
            return sdEventExtensionPointSet::descriptors[i].getDescriptorAsString();
        }
    }
    return std::string("error");
}

// returns value of the event as a string
std::string sdEventExtensionPointSet::getValueAsString(void) const{
    std::string str;
    switch (descriptor) {
            
        case SD_POINT_SET_NAME:{
            str = doubleToString(*static_cast<double *>(value));
            break;
        }
        default:
            break;
    }
    return str;
}

// set value with enum and pointer
bool sdEventExtensionPointSet::setValue(const EDescriptor descriptor, void* const value){
    
    sdEventExtensionPointSet::descriptor = descriptor;
    switch (sdEventExtensionPointSet::descriptor) {
            
        case SD_POINT_SET_NAME:{
            sdEvent::value = static_cast<void*>(value);
            break;
        }
        default:{
            return false;
            break;
        }
    }
    return true;
}

// set value with strings
bool sdEventExtensionPointSet::setValue(const std::string descriptor, const std::string value){
    
    for(int i = 0; i < sdEventExtensionPointSet::numberOfDescriptors; i++){
        if(sdEventExtensionPointSet::descriptors[i].getDescriptorAsString() == descriptor){
            sdEventExtensionPointSet::descriptor =
            sdEventExtensionPointSet::descriptors[i].getDescriptor();
        }
    }
    
    switch (sdEventExtensionPointSet::descriptor) {
        case SD_POINT_SET_NAME:{
            sdEventExtensionPointSet::value = static_cast<void*>(new std::string(value));
            break;
        }
        default:{
            return false;
            break;
        }
    }
    
    return true;
}

