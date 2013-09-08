#ifndef _sdEntityExtensionMedia_h
#define _sdEntityExtensionMedia_h

#include <string>
#include "sdConst.h"
#include "sdEntityExtension.h"

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

    sdEventExtensionMedia(double time, EDescriptor descriptor, void* value);
    sdEventExtensionMedia(string time, string descriptor, string value);

    /*! destructor destroy all allocated memory to the value pointer*/
    ~sdEventExtensionMedia();
    
    /*! overrided method. get descriptor as string e.g. "location" */
    string getDescriptorAsString(void);
    
    /*! overrided method. get value as string e.g. "0.3 0.5 0.2"*/
    string getValueAsString(void);
    
private:
    /*!
     @name private setter
     The modification of parameter after the instantiation is not allowed.
     All member variables of this class should be initialized through the constructor and should not be modified after that in order to increase the stability
     @{
     */
    
    /*! set value. this function allocate memory to the void pointer member variable according to the given EDescriptor */
    bool setValue(EDescriptor descriptor, void* value);
    
    /*! same as above but you can give all arguments with strings */
    bool setValue(string descriptor, string value);

};



/*!
 a media extension. This extension contains information about an audio media
 */
class sdEntityExtensionMedia: public sdEntityExtension{
    friend class sdEntityCore;
        
    static const EExtension extensionName; /*< identification of the class */
    static const string extensionNameAsString; /*< identification of the class */

    static const int numberOfRelevantDescriptors;
    static const EDescriptor relevantDescriptors[]; 
    static const string relevantDescriptorStrings[]; 
    /*!
     this constructor should be involed from an instance of sdScene
     */
    sdEntityExtensionMedia(){};
    
public:
    
    EExtension getExtensionName(void);
    string getExtensionNameAsString(void);
        
    /*! a method for adding a new event to the entity*/
    sdEvent* addEvent(double time, EDescriptor descriptor, void* value);
    
    /*! same as above but you can specify arguments with strings */
    sdEvent* addEvent(string time, string descriptor, string value);
    
    void removeEvent(double time, EDescriptor descriptor);
    
    static const EDescriptor stringToDescriptor(string str);

};


/*** inline implementation ***/
inline EExtension sdEntityExtensionMedia::getExtensionName(void){
    return extensionName;
}

inline string sdEntityExtensionMedia::getExtensionNameAsString(void){
    return extensionNameAsString;
}

#endif
