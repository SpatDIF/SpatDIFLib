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


#ifndef ____sdEventExtensionSourceWidth__
#define ____sdEventExtensionSourceWidth__

#include "sdEvent.h"
#include "sdDescriptor.h"
/*!
 this class stores event info (i.e. time, descriptor and value) of sdEntityExtensionSourceWidth.
 */

class sdEventExtensionSourceWidth: public sdEvent{
    
    /*! only sdEntityExtensionSourceWidth can make an instance of this class */
    friend class sdEntityCore;
    
public:
    /*! @name const static variables
     @{
     */
    static const int numberOfDescriptors;
    static const sdDescriptor descriptors[];
    
    /*!
     @}
     */
    
    /*!
     constructor with initialization.
     @param time time of the event
     @param descriptor descriptor of the event. declared in sdConst.h
     @param value void pointer to value to be copied. Proper size of memory will be automatically allocated.
     */
    
    sdEventExtensionSourceWidth(const double time, const EDescriptor descriptor, void* const value);
    sdEventExtensionSourceWidth(const std::string time, const std::string descriptor, const std::string value);
    
    /*! destructor destroy all allocated memory to the value pointer*/
    ~sdEventExtensionSourceWidth();
    
    /*! overridden method. get descriptor as string e.g. "location" */
    std::string getDescriptorAsString(void) const;
    
    /*! overridden method. get value as string e.g. "0.3 0.5 0.2"*/
    std::string getValueAsString(void) const;
    
private:
    /*!
     @name private setter
     The modification of parameter after the instantiation is not allowed.
     All member variables of this class should be initialized through the constructor and should not be modified after that in order to increase the stability
     @{
     */
    
    /*! set value. this function allocate memory to the void pointer member variable according to the given EDescriptor */
    bool setValue(const EDescriptor descriptor, void* const value);
    
    /*! same as above but you can give all arguments with strings */
    bool setValue(const std::string descriptor, const std::string value);
    
};

#endif