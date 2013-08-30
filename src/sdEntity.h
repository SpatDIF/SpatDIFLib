
#ifndef ____sdEntity__
#define ____sdEntity__

#include <set>
#include <string>
#include <iostream>
#include <sstream>
#include "sdConst.h"

using namespace std;

/*!
 maintains a set of event data. The constructor is intentionally declared as a protected function. This must be invoked through  sdEntity or its subclasses. This process ensures that all existing instances of sdEvents belong to a sdEntity and are aware of required extensions. The class is also responsible for the allocation of void* value by checking the descriptor. Thus, desciptor and value should be simultaneously set by set or setVlaue functions.
 */
class sdEvent{
    
protected:
    float time;/*!< time of the event */
    EDescriptor descriptor;/*!<descriptor of the event*/
    void *value;/*!<value of the event. sdEntity allocates the void pointer automatically*/
    
    /*!
     construcor is intentionally declared as protected. this class must be instantiated through sdEntity::addEvent
     */
    sdEvent(void){};
    
    /*!
     internal functions for conversion from Enum to string
     @param descriptor descriptor declared in sdConst.h
     */
    virtual string convert(EDescriptor descriptor) = 0;
    
    /*!
     internal functions for conversion from Enum to string
     @param kind kind declared in sdConst.h
     */
    virtual string convert(EKind kind) = 0;
    
    /*!
     internal functions for conversion from Enum to string
     @param type type declared in sdConst.h
     */
    virtual string convert(EType type) = 0;
    
    /*!
     converts string to enums. The return value should be casted to the proper enum.
     @param enum
     */
    virtual int convert(string str) = 0;
    
public:
    /*!
     returns time as Float
     */
    float getTime(void);
    /*!
     returns time as string
     */
    string getTimeAsString(void);
    
    /*!
     returns descriptor as Enum
     */
    EDescriptor getDescriptor(void);
    
    /*!
     returns descriptor as string
     */
    virtual string getDescriptorAsString(void) = 0;
    
    /*!
     returns value through void pointer. It should be casted to propoer type manually.
     */
    void* getValue(void);
    
    /*!
     returns value as string
     */
    virtual string getValueAsString(void) = 0;
    
    /*!
     set al three parameters at once
     @param time time of event
     @param desciptor desciprot of event declared in sdConst.h
     @param value value of event
     */
    void set(float time, EDescriptor descriptor, void* value);
    
    /*!
     set time of the event
     @param time time of the event
     */
    void setTime(float time);
    
    /*!
     descriptor and value should be set at once. because this function allocates memory to void* value based on the descriptor
     */
    virtual void setValue(EDescriptor descriptor, void* value) = 0;
};

/*** inline implementation ***/
inline float sdEvent::getTime(void){
    return time;
}

inline string sdEvent::getTimeAsString(void){
    ostringstream timeStringStream;
    timeStringStream << time;
    return timeStringStream.str();
}

inline EDescriptor sdEvent::getDescriptor(void){
    return descriptor;
}

inline void* sdEvent::getValue(void){
    return value;
}

inline void sdEvent::set(float time, EDescriptor descriptor, void* value){
    setTime(time);
    setValue(descriptor, value);
}

inline void sdEvent::setTime(float time){
    sdEvent::time = time;
}

/*!
 a helper class for sorting events
 */
class sdEventCompare{
public:
    bool operator()(sdEvent* e1, sdEvent* e2){
        if(e1->getTime() < e2->getTime()) // sort by time
            return true;
        else
            return false;
    }
};


/*! sdEntity
 sdEntity is a pure abstract class. This class maintains and handles all events associated to relavant descriptors. This class is also responsible for answering query about it's relavant descriptors.
*/

class sdEntity{

protected:
    set<sdEvent*, sdEventCompare> eventSet; /*!< maintains all relevant events */

public:
    sdEntity(); /*!< constructor */
    ~sdEntity(); /*!< desctrictor */
    
    /*!
     query method. it simply returns a pointer to the buffer, where the designated data are stored. returns null if not found. This is a pure abstract function. The subclass must override.
     @param time specify time
     @param descriptor specify descriptor defined in sdConst.h
     */
    virtual void* getValue(float time, EDescriptor descriptor) = 0;
    
    /*!
     return all events related to the given descriptor
     @param descriptor the descriptor of the event declared in sdConst.h
     @param time the time of the event in second
     */
    virtual sdEvent* getEvent(float time, EDescriptor descriptor) = 0;

    /*!
     return a set of sdEvent pointers. sorted by time.
     */
    set<sdEvent*, sdEventCompare> getEventSet(void);
    
    /*!
     return number of registerd events in the eventSet
     */
    unsigned int getNumberOfEvents(void);
    
    /*!
     this function is the only way to instantiate sdEvent and this function is responsible for allocating the void pointer properly based on the provoded descriptor
     */
    virtual sdEvent* addEvent(float time, EDescriptor descriptor, void* value) = 0;
    virtual sdEvent* addEvent(string time, string descriptor, string value) = 0;

    /*<
     remove an event from the set
     @param event the event to be removed
     */
    void removeEvent(sdEvent *event);
    
    /*!
     remove an event at the specified time and descriptor
     @param descriptor the descriptor of sdEvent to be removed
     @param time the time of sdEvent to be removed. 
     */
    void removeEvent(EDescriptor descriptor, float time);
    
    /*!
     remove events in the specified period
     @param descriptor the descriptor of sdEvent to be removed
     @param start specifies the start point of the removal
     @param end specifies the end point of the removal
     */
    void removeEvent(EDescriptor descriptor, float start, float end);
    
    /*!
     remove all events in the eventSet
     */
    void removeAllEvents();
};

/*** inline implementation ***/

inline sdEntity::sdEntity(){
}

inline sdEntity::~sdEntity(){
}

inline set<sdEvent*, sdEventCompare> sdEntity::getEventSet(void){
    return eventSet;
}
    
inline unsigned int sdEntity::getNumberOfEvents(){
    return eventSet.size();
}

inline void sdEntity::removeAllEvents(){
    eventSet.clear();
}


#endif /* defined(____sdEntity__) */

