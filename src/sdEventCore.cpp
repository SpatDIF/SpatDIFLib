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

#include "sdEventCore.h"

#pragma mark static constant

const unsigned int sdEventCore::numberOfDescriptors = 3;
const sdDescriptor sdEventCore::descriptors[sdEventCore::numberOfDescriptors] = {
    sdDescriptor(SD_PRESENT, std::string("present"), false),
    sdDescriptor(SD_POSITION, std::string("position"), true),
    sdDescriptor(SD_ORIENTATION, std::string("orientation"), true)
};


#pragma mark constructor/destructor

inline sdEventCore::~sdEventCore(){
    
    if(!value) return;
    
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

#pragma mark accessors

std::string sdEventCore::getValueAsString(void) const{
    std::string str;
    switch (descriptor) {
        case SD_PRESENT:{
            bool *present;
            present = static_cast<bool*>(value);
            str = *present ? std::string("true") : std::string("false");
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

bool sdEventCore::setValue(const EDescriptor descriptor, void* const value){
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


inline bool sdEventCore::setValue(const std::string descriptor, const std::string value){
    // set descriptor enum from the given string and static sdDescriptor array
    
    sdEventCore::descriptor = getDescriptorAsEnum(descriptor, descriptors, numberOfDescriptors);
    
    // set value
    std::string str;
    switch (this->descriptor) {
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
            std::cout << "Error: sdEventCore::setValue()\nunknown descriptor " << descriptor << std::endl;
            return false;
            break;
        }
    }
    return true;
}

