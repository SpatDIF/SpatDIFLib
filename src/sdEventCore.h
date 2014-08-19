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

#ifndef ____sdEventCore__
#define ____sdEventCore__

#include "sdEvent.h"
#include "sdDescriptor.h"

#pragma mark declarations

/*!
 this class stores event info (i.e. time, descriptor and value) of sdEntityCore.
 */
class sdEventCore: public sdEvent{

    /*! only sdEntityCore can make an instance of this class */
    friend class sdEntityCore;
    
public:
    
    /*!
     @name static constants
     @{
     */
    
    const static unsigned int numberOfDescriptors;
    const static sdDescriptor descriptors[];
    /*!
     @}
     */
    
    /*!
     @name constructors/destructors
     @{
     */
    
    /*!
     constructor with initialization. simply call the identical constructor of the superclass
     @param time time of the event
     @param descriptor descriptor of the event. declared in sdConst.h
     @param value void pointer to value to be copied. Proper size of memory will be automatically allocated.
     */
    sdEventCore(const double time, const EDescriptor descriptor, void* const value):
    sdEvent(time, descriptor, value){}; // initializer list
    
    /**
     * @brief constructor with string arguments, useful when loading from files etc
     * @param time time of event as a string
     * @param descriptor descriptor of event as a string
     * @param value value of event as a string
     */
    sdEventCore(const std::string time, const std::string descriptor, const std::string value);
    
    /*! destructor destroy all allocated memory to the value pointer*/
    ~sdEventCore();
    
    /*! @} */

    /*! overrided function. get value as string e.g. "0.3 0.5 0.2"*/
    std::string getValueAsString(void) const;
    
    /*! overrided function. get descriptor as string* e.g. "position" */
    std::string getDescriptorAsString(void) const;

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
    bool setValue(const std::string descriptor, const std::string value);
    
    /*! @} */
};

#pragma mark implementations

inline sdEventCore::sdEventCore(const std::string time, const std::string descriptor, const std::string value){
    setTime(time); // declared in the super class
    setValue(descriptor, value); // private function
}

inline std::string sdEventCore::getDescriptorAsString(void) const{
    return ::getDescriptorAsString(descriptor, descriptors, numberOfDescriptors); // global call
    return NULL;
}

#endif
