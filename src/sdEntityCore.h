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
    
    /*! this returns a set of sdEvents with specified descriptor whose time parameter is between start and end time
     @param start start time
     @param end end time
     @param descriptor
     */
    set <sdEvent*, sdEventCompare>getFilteredEventSet(float start, float end, EDescriptor descriptor);
    
    void addExtension(sdEntityExtension *extension);

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


#endif /* defined(____sdEntityCore__) */
