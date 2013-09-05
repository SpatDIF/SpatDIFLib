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

    sdEventExtensionMedia(float time, EDescriptor descriptor, void* value);
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
    void setValue(EDescriptor descriptor, void* value);
    
    /*! same as above but you can give all arguments with strings */
    void setValue(string descriptor, string value);
    

    
};



/*!
 a media extension. This extension contains information about an audio media
 */
class sdEntityExtensionMedia: public sdEntityExtension{
    friend class sdEntityCore;
        

    string id; /*< unique identifier */
    string type; /*< where the content comes from */
    string location; /*< location of the file or stream*/
    int channel; /*< If type has more channels, define the channel that is taken as input*/
    double timeOffset;/*< Starting position within media file */
    double gain;/*< gain value of the media*/
    
    static const EExtension extensionName; /*< identification of the class */
    static const int numberOfRelevantDescriptors;
    static const EDescriptor relevantDescriptors[]; /*< this extension stores data with the descriptors stored in this array */
    
    /*!
     this constructor should be involed from an instance of sdScene
     */
    
    sdEntityExtensionMedia(string id = "unknown", string type = "none", string location ="", int channel = 1, double timeOffset = 0.0, double gain = 1.0);
    
public:
    
    EExtension getExtensionName(void);
    string getExtensionNameAsString(void);
    
    /*!
     query method. it simply returns a pointer to the buffer, where the designated data are stored. returns null if not found. This is a pure abstract function.
     @param time specify time
     @param descriptor specify descriptor defined in sdConst.h
     */
    void* getValue(double time, EDescriptor descriptor);

    /*!
     return all events related to the given descriptor
     @param descriptor the descriptor of the event declared in sdConst.h
     @param time the time of the event in second
     */
    sdEvent* getEvent(double time, EDescriptor descriptor);
    
    /*! a method for adding a new event to the entity*/
    sdEvent* addEvent(double time, EDescriptor descriptor, void* value);
    
    /*! same as above but you can specify arguments with strings */
    sdEvent* addEvent(string time, string descriptor, string value);
    
    /*! returns true if the provided descriptor in the relevantDescriptorArray */
    bool isDescriptorRelevant(EDescriptor descriptor);

};


inline sdEntityExtensionMedia::sdEntityExtensionMedia(string id, string type, string location, int channel, double timeOffset, double gain){

    sdEntityExtensionMedia::id = id;
    sdEntityExtensionMedia::type = type;
    sdEntityExtensionMedia::location = location;
    sdEntityExtensionMedia::channel = channel;
    sdEntityExtensionMedia::timeOffset = timeOffset;
    sdEntityExtensionMedia::gain = gain;
}

/*** inline implementation ***/
inline EExtension sdEntityExtensionMedia::getExtensionName(void){
    return extensionName;
}

inline string sdEntityExtensionMedia::getExtensionNameAsString(void){
    return string("media");
}

#endif
