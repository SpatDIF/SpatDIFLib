//
//  sdEntityCore.h
//  

#ifndef ____sdEntityCore__
#define ____sdEntityCore__


#include <set>
#include <string>
#include "sdConst.h"
#include "sdEntity.h"
#include "sdEntityExtension.h"

using namespace std;

/*!
 this class stores event info (i.e. time, descriptor and value)
 */

class sdEventCore: public sdEvent{
    
    /*! only sdEntityCore can make an instantitate of this class */
    friend class sdEntityCore;

public:
    /*!
     constructor with initialization.
     @param time time of the event
     @param descriptor descriptor of the event. declared in sdConst.h
     @param value void pointer to value to be copied. Proper size of memory will be internally allocated.
     */
    sdEventCore(float time, EDescriptor descriptor, void* value);
    
    /*!
     constructor with initialization.
     @param time time of the event
     @param descriptor descriptor of the event. declared in sdConst.h
     @param value void pointer to value to be copied. Proper size of memory will be internally allocated.
     */
    sdEventCore(string time, string descriptor, string value);

    string getDescriptorAsString(void);
    string getValueAsString(void);

    void setValue(EDescriptor, void*);

    string convert(EDescriptor);
    string convert(EKind);
    string convert(EType);
    int convert(std::string);
};

/*** inline implementation ***/
inline sdEventCore::sdEventCore(float time, EDescriptor descriptor, void* value){
    sdEventCore::time = time;
    sdEventCore::descriptor = descriptor;
    setValue(descriptor, value);
}



/*!
 this constructor facilitates to load string based data to the entity. it converts all strings to proper data types and invoke the function above.
 */




class sdScene;

/*!
 sdEntityCore is responsible for
 - identifing its kind (source or sink)
 - maintaining its events
 - managing extensions
 - giveing pointer to a certain, ranged, or the entire list of event(s)
 - forwarding a query to a proper extension and answer to the client
 */

class sdEntityCore: public sdEntity{
    friend class sdScene; // only sdScene can instantiate this class
    
private:
    /*! the name of the entity. This name is invariable (const)*/
    const string name;
    
    /*! contains instances of sdEntityExtenstions */
    vector <sdEntityExtension*>extensionVector;
    
    /*! define the kind of the entity. This property is declared as const and invariable (const). */
    const EKind kind;
    
    /*! constructor. intentionally declared as a private function. 
     the sdEntityCore class must be instantiated through the addEntity function of sdScene. this guarantees that all instances of sdEntityCore are be aware of their parent scene and activated extensions.
     @param name the name of new Entity
     @param kind the kind of new Entity. default = SD_SOURCE
     */
    sdEntityCore(string name, EKind kind = SD_SOURCE) :name(name), kind(kind){} ;// constructor with an initializer
    
public:
    /*!  returns the name of the entity */
    string getName(void);
    
    /*! returns the kind of the entity */
    EKind getKind(void);
    string getKindAsString(void);

    /*! a overrieded query function. it simply returns a pointer to the buffer, where the designated data are stored. returns null if not found.
     @param time
     @param descriptor
     */
    void* getValue(float time, EDescriptor descriptor);
    
    sdEvent* addEvent(float time, EDescriptor descriptor, void* value);
    sdEvent* addEvent(string time, string descriptor, string value);
    sdEvent* getEvent(float time, EDescriptor descriptor);

    /*! this returns a set of sdEvents with specified descriptor whose time parameter is between start and end time
     @param start start time
     @param end end time
     @param descriptor
     */
    set <sdEvent*, sdEventCompare>getFilteredEventSet(float start, float end, EDescriptor descriptor);
    
    void addExtension(sdEntityExtension *extension);

    /*!
     static function for sorting. employed by sdSaver
     */
    static bool sortAlphabetically( sdEntityCore *leftEntity,  sdEntityCore *rightEntity);

};


/*** inline implementation ***/

inline EKind sdEntityCore::getKind(void){
    return kind;
}

inline string sdEntityCore::getName(void){
    return name;
}

inline void sdEntityCore::addExtension(sdEntityExtension *extension){
    extensionVector.push_back(extension);
}

inline bool sdEntityCore::sortAlphabetically( sdEntityCore *leftEntity,  sdEntityCore *rightEntity){
    return leftEntity->getName().compare(rightEntity->getName()) ? false:true;
}

#endif /* defined(____sdEntityCore__) */
