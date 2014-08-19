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

#ifndef ____sdEntityCore__
#define ____sdEntityCore__

#include <set>
#include <string>
#include "sdEventCore.h"
#include "sdEntityExtension.h"
#include "sdDescriptor.h"

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
    
    typedef struct{
        EDescriptor descriptor;
        string descriptorString;
        sdEntityExtension* responsibleExtension;
    } sdRedirector;
    
    /*! the name of the entity. This name is invariable (const)*/
    const string name;
    
    /*! contains instances of sdEntityExtenstions */
    vector <sdEntityExtension*>extensionVector;
    
    /*! contains pairs of EDescriptor descriptor and pointer to a responsible extension */
    vector <sdRedirector>redirectorVector;

    /*! define the kind of the entity. This property is declared as const and invariable (const). */
    EKind kind;
    EType type;
    
    /*! constructor. intentionally declared as a private function. 
     the sdEntityCore class must be instantiated through the addEntity function of sdScene. this guarantees that all instances of sdEntityCore are be aware of their parent scene and activated extensions.
     @param name the name of new Entity
     @param kind the kind of new Entity. default = SD_SOURCE
     */
    sdEntityCore(string name, EKind kind = SD_SOURCE, EType type = SD_POINT) :name(name), kind(kind){
        sdEntityCore::type = type;
    };
    
    
    /*! adding extension to the entity. invoked only by sdScene*/
    sdEntityExtension* addExtension(EExtension extension);
    
    /*! removing extension from the entity. invoked only by sdScene*/
    void removeExtension(EExtension extension);
   
public:
    static bool isCoreDescriptor(EDescriptor descriptor);
    static bool isCoreDescriptor(string descriptor);
    
    /*!
     @name Setter/Getter
     @{
     */
    
    /*! this returns a set of sdEvents with specified descriptor whose time parameter is between start and end time
     @param start start time
     @param end end time
     @param descriptor
     */
    
    /*!  returns the name of the entity */
    string getName(void);
    
    /*! returns the kind of the entity */
    EKind getKind(void);
    
    /*! returns the kind of the entity as a string*/
    string getKindAsString(void);

    /*! returns the type of the entity */
    EType getType(void);
    string getTypeAsString(void);
    
      
    /*! This is the only method for adding an new event to the entity*/
    sdEvent* addEvent(double time, EDescriptor descriptor, void* value);
    
    /*! same as above but you can specify arguments with string */
    sdEvent* addEvent(string time, string descriptor, string value);

    /*! remove specified event
     @param time time of the event
     @param descriptor desciprot of the event
     */
    
    void removeEvent(double time, EDescriptor descriptor);
    void removeEvent(string time, string descriptor);

    /*! @name Extension Handling
        @{
     */

    /*! get the vector of sdExtensions */
    vector <sdEntityExtension*>getExtensionVector(void);
    
    /*! returns a pointer to a sdEntityExtension in the extensionVector. returns NULL if not exists. */
    sdEntityExtension* getExtension(EExtension extension);

    /*! returns a pointer to a sdEntityExtension in the extensionVector. returns NULL if not exists */
    sdEntityExtension* getResponsibleExtension(EDescriptor descriptor);
    
    /*! @} */
    

    /*! overridden function. this function sum up all events (i.e. both core and extension events) and return it */
     int getNumberOfEvents();

    /*!
     overridden function. this function forwards the query to the extensions if an extended descriptor is provided as an argument.
     @param time the time of the event in second
     @param descriptor the descriptor of the event declared in sdConst.h
     */
    sdEvent* getEvent(double time, EDescriptor descriptor);
    
    /*!
     overridden  and overloaded functions. this function forwards the query also to the attached extensions and return all events as a multiset.
     @param time the time of the event in second
     @param descriptor the descriptor of the event declared in sdConst.h
     */
    multiset<sdEvent*, sdEventCompare> getEventSet(void);
    multiset<sdEvent*, sdEventCompare> getEventSet(double time);
    multiset<sdEvent*, sdEventCompare> getEventSet(double start, double end);
    multiset<sdEvent*, sdEventCompare> getEventSet(double start, double end, EDescriptor descriptor);

    /*!
     overridden function. this function forwards the query to the extensions if an extended descriptor is provided as an argument.
     @param time the next event from this time point will be returned
     @param descriptor the descriptor of the event declared in sdConst.h
     */
    sdEvent* getNextEvent(double time, EDescriptor descriptor);
    multiset<sdEvent*, sdEventCompare> getNextEventSet(double time);
    double getNextEventTime(double time);

    sdEvent* getPreviousEvent(double time, EDescriptor descriptor);
    multiset<sdEvent*, sdEventCompare> getPreviousEventSet(double time);
    double getPreviousEventTime(double time);

    sdEvent* getFirstEvent(EDescriptor descriptor);
    multiset<sdEvent*, sdEventCompare> getFirstEventSet();
    double getFirstEventTime();

    sdEvent* getLastEvent(EDescriptor descriptor);
    multiset<sdEvent*, sdEventCompare> getLastEventSet();
    double getLastEventTime();

    /*!
     overridden function. this function forwards the query to the extensions if an extended descriptor is provided as an argument.
     @param time time of the event
     @param descriptor descriptor of the event
     */
    void* getValue(double time, EDescriptor descriptor);

    /*!static function for sorting. employed by sdSaver*/
    static bool sortAlphabetically( sdEntityCore *leftEntity, sdEntityCore *rightEntity);
};


/*** inline implementation ***/
inline string sdEntityCore::getName(void){
    return name;
}

inline EKind sdEntityCore::getKind(void){
    return kind;
}

inline EType sdEntityCore::getType(void){
    return type;
}

inline vector <sdEntityExtension*>sdEntityCore::getExtensionVector(void){
    return extensionVector;
}

inline bool sdEntityCore::sortAlphabetically( sdEntityCore *leftEntity,  sdEntityCore *rightEntity){
    return leftEntity->getName().compare(rightEntity->getName()) ? false:true;
}

inline bool sdEntityCore::isCoreDescriptor(EDescriptor descriptor){
    for(int i = 0; i< sdEventCore::numberOfDescriptors; i++){
        if(sdEventCore::descriptors[i].getDescriptor() == descriptor) return true;
    }
    return false;
}

inline bool sdEntityCore::isCoreDescriptor(string descriptor){
    for(int i = 0; i< sdEventCore::numberOfDescriptors; i++){
        if(sdEventCore::descriptors[i].getDescriptorAsString() == descriptor)
            return true;
    }
    return false;
}


#endif /* defined(____sdEntityCore__) */
