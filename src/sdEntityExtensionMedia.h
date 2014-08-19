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

#ifndef ____sdEntityExtensionMedia__
#define ____sdEntityExtensionMedia__

#include <string>
#include "sdConst.h"
#include "sdEntityExtension.h"
#include "sdDescriptor.h"

using namespace std;


/*!
 this class stores event info (i.e. time, descriptor and value) of sdEntityExtensionMedia.
 */

class sdEventExtensionMedia: public sdEvent{
    
    /*! only sdEntityExtensionMedia can make an instance of this class */
    friend class sdEntityCore;

public:
    /*!
     constructor with initialization.
     @param time time of the event
     @param descriptor descriptor of the event. declared in sdConst.h
     @param value void pointer to value to be copied. Proper size of memory will be automatically allocated.
     */

    sdEventExtensionMedia(const double time, const EDescriptor descriptor, void* const value);
    sdEventExtensionMedia(const string time, const string descriptor, const string value);

    /*! destructor destroy all allocated memory to the value pointer*/
    ~sdEventExtensionMedia();
    
    /*! overridden method. get descriptor as string e.g. "location" */
    string getDescriptorAsString(void) const;
    
    /*! overridden method. get value as string e.g. "0.3 0.5 0.2"*/
    string getValueAsString(void) const;
    
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
    bool setValue(const string descriptor, const string value);

};



/*!
 a media extension. This extension contains information about an audio media
 */
class sdEntityExtensionMedia: public sdEntityExtension{
    
    friend class sdEntityCore; /*< only entityCore can spawn instances of this class*/

private:

    
    /*!
     this constructor should be invoked from an instance of sdEntityCore
     */
    sdEntityExtensionMedia(){};
    
public:
    /*! @name const static variables
     @{
     */
    static const EExtension extensionName; /*< identification of the class */
    static const string extensionNameAsString; /*< identification of the class */
    static const int numberOfDescriptors;
    static const sdDescriptor descriptors[];
  
    /*! @name Event handling
     @{
     */
    
    /*! a method for adding a new event to the entity*/
    sdEvent* addEvent(const double time, const EDescriptor descriptor, void* const value);
    
    /*! same as above but you can specify arguments with strings */
    sdEvent* addEvent(const string time, const string descriptor, const string value);
    
    void removeEvent(const string time, const string descriptor);

    /*!
     @}
    */
    
    EExtension getExtensionName(void);
    string getExtensionNameAsString(void);
    
};

inline EExtension sdEntityExtensionMedia::getExtensionName(void){
    return extensionName;
}

inline string sdEntityExtensionMedia::getExtensionNameAsString(void){
    return extensionNameAsString;
}

#endif
