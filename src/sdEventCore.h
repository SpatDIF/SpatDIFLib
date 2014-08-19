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

#ifndef ___sdEventCore__
#define ___sdEventCore__

#include "sdEvent.h"


/*!
 this class stores event info (i.e. time, descriptor and value) of sdEntityCore.
 */
class sdEventCore: public sdEvent{

    /*! only sdEntityCore can make an instance of this class */
    friend class sdEntityCore;
    
public:
    /*!
     constructor with initialization.
     @param time time of the event
     @param descriptor descriptor of the event. declared in sdConst.h
     @param value void pointer to value to be copied. Proper size of memory will be automatically allocated.
     */
    sdEventCore(const double time, const EDescriptor descriptor, void* const value);
    
    /**
     * @brief constructor with string arguments, useful when loading from files etc
     * @param time time of event as a string
     * @param descriptor descriptor of event as a string
     * @param value value of event as a string
     */
    sdEventCore(const string time, const string descriptor, const string value);
    
    /*! destructor destroy all allocated memory to the value pointer*/
    ~sdEventCore();
    
    /*! overrided method. get value as string e.g. "0.3 0.5 0.2"*/
    string getValueAsString(void) const;
    
    /*! overrided function. get descriptor as string*/
    string getDescriptorAsString(void) const;
    
private:
    /*!
     @name private setter
     The modification of parameter after the instantiation is not permitted.
     All member variables of this class should be initialized through the constructor and should not be modified after that in order to increase the stability
     @{
     */
    
    /*! set value. this function allocate memory to the void pointer member variable according to the given EDescriptor */
    bool setValue(const EDescriptor descriptor, void* value);
    
    /*! same as above but you can give all arguments with strings */
    bool setValue(const string descriptor, const string value);
    
    /*! @} */
};

#pragma mark implementations

inline sdEventCore::sdEventCore(const double time, const EDescriptor descriptor, void* value){
    setTime(time); // declared in the super class
    setValue(descriptor, value); // private function
}

inline sdEventCore::sdEventCore(const string time, const string descriptor, const string value){
    setTime(time); // declared in the super class
    setValue(descriptor, value); // private function
}

inline sdEventCore::~sdEventCore(){
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

inline string sdEventCore::getValueAsString(void) const{
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

inline bool sdEventCore::setValue(const EDescriptor descriptor, void* const value){
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

inline bool sdEventCore::setValue(const string descriptor, const string value){
    // set descriptor enum from the given string and static sdDescriptor array
//    sdEventCore::descriptor =
//    getDescriptorAsEnum(descriptor, sdEntityCore::descriptors, sdEntityCore::numberOfDescriptors);
    
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

inline string sdEventCore::getDescriptorAsString(void) const{
//    return ::getDescriptorAsString(descriptor, sdEntityCore::descriptors, sdEntityCore::numberOfDescriptors); // global call
    return NULL;
}

#endif
