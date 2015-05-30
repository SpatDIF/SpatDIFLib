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

#include "sdEventExtensionSourceWidth.h"


const int sdEventExtensionSourceWidth::numberOfDescriptors = 1;
const sdDescriptor sdEventExtensionSourceWidth::descriptors[sdEventExtensionSourceWidth::numberOfDescriptors] = {
    sdDescriptor(SD_SOURCE_WIDTH_WIDTH, std::string("width"), true)
};

// constructors
sdEventExtensionSourceWidth::sdEventExtensionSourceWidth(double time, EDescriptor descriptor, void* value){
    setTime(time); // declared in sdEntity.h
    setValue(descriptor, value);
}

sdEventExtensionSourceWidth::sdEventExtensionSourceWidth(std::string time, std::string descriptor, std::string value){
    setTime(time);
    setValue(descriptor, value);
}

// destructor
sdEventExtensionSourceWidth::~sdEventExtensionSourceWidth(){
    
    switch (descriptor) {
        case SD_SOURCE_WIDTH_WIDTH:{
            double *valuePointer = static_cast<double*>(value);
            delete valuePointer;
            break;
        }
        default:
            break;
    }
}

// returns descriptor of the event as a string
std::string sdEventExtensionSourceWidth::getDescriptorAsString(void) const{
    for(int i = 0; i< sdEventExtensionSourceWidth::numberOfDescriptors; i++){
        if(sdEventExtensionSourceWidth::descriptors[i].getDescriptor() == descriptor){
            return sdEventExtensionSourceWidth::descriptors[i].getDescriptorAsString();
        }
    }
    return std::string("error");
}

// returns value of the event as a string
std::string sdEventExtensionSourceWidth::getValueAsString(void) const{
    std::string str;
    switch (descriptor) {
  
        case SD_SOURCE_WIDTH_WIDTH:{
            str = doubleToString(*static_cast<double *>(value));
            break;
        }
        default:
            break;
    }
    return str;
}

// set value with enum and pointer
bool sdEventExtensionSourceWidth::setValue(const EDescriptor descriptor, void* const value){
    
    sdEventExtensionSourceWidth::descriptor = descriptor;
    switch (sdEventExtensionSourceWidth::descriptor) {

        case SD_SOURCE_WIDTH_WIDTH:{
            double *doubleValue = static_cast<double*>(value);
            sdEvent::value = static_cast<double*>(new double(*doubleValue));
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
bool sdEventExtensionSourceWidth::setValue(const std::string descriptor, const std::string value){
    
    for(int i = 0; i < sdEventExtensionSourceWidth::numberOfDescriptors; i++){
        if(sdEventExtensionSourceWidth::descriptors[i].getDescriptorAsString() == descriptor){
            sdEventExtensionSourceWidth::descriptor =
            sdEventExtensionSourceWidth::descriptors[i].getDescriptor();
        }
    }
    
    switch (sdEventExtensionSourceWidth::descriptor) {
        case SD_SOURCE_WIDTH_WIDTH:{
            sdEventExtensionSourceWidth::value = static_cast<void*>(new double(stringToDouble(value)));
            break;
        }
        default:{
            return false;
            break;
        }
    }
    
    return true;
}

