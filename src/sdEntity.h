
#ifndef ____sdEntity__
#define ____sdEntity__

#include <set>
#include <string>
#include <iostream>
#include <sstream>
#include "sdConst.h"

using namespace std;

/*!
 a pure virtual class of event.
 it declares functions that all subclasses of this class should override.
 */
class sdEvent{
    
protected:
    double time;/*!< time of the event */
    EDescriptor descriptor;/*!<descriptor of the event*/
    void *value;/*!<value of the event. the subclasses must allocates this void pointer */
    
    /*!
     this construcor is intentionally declared as protected. this class must be instantiated through sdEntity::addEvent
     */
    sdEvent(void){};
    
public:
    /*! @name Setter/Getter
     @{
     */
    
    /*!
     set all three parameters at once
     @param time time of event
     @param desciptor desciprot of event declared in sdConst.h
     @param value value of event
     */
    void set(double time, EDescriptor descriptor, void* value);
    
    /*!
     set time of the event
     @param time time of the event
     */
    void setTime(double time);
    void setTime(string timeString);
    
    /*!returns time as double*/
    double getTime(void);
    
    /*!returns time as string*/
    string getTimeAsString(void);
    
    /*!returns descriptor as Enum*/
    EDescriptor getDescriptor(void);
    
    /*!returns value through void pointer. It should be casted to propoer type manually.*/
    void* getValue(void);

    /*!
     @}
     */
    
    /*! @name Virtual functions
    @{
    */
    
    /*!descriptor and value should be set at once. because this function allocates memory to void* value based on the descriptor*/
    virtual void setValue(EDescriptor descriptor, void* value) = 0;
    
    /*!returns descriptor as string. a routine for conversion should be implemented in derived classes*/
    virtual string getDescriptorAsString(void) = 0;
    
    /*!returns value as string. a routine for conversion should be implemented in derived classes */
    virtual string getValueAsString(void) = 0;
    
   /*!
     @}
     */
};

/*** inline implementation ***/
inline double sdEvent::getTime(void){
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

inline void sdEvent::set(double time, EDescriptor descriptor, void* value){
    setTime(time);
    setValue(descriptor, value);
}

inline void sdEvent::setTime(double time){
    sdEvent::time = time;
}

inline void sdEvent::setTime(string timeString){
    istringstream is;
    is.str(timeString);
    is >> sdEvent::time;
};

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
 sdEntity is a pure abstract class. This class maintains and handles all events associated to relevant descriptors. This class is also responsible for answering query about it's relevant descriptors.
*/
class sdEntity{

protected:
    set<sdEvent*, sdEventCompare> eventSet; /*!< maintains all relevant events */

public:
    sdEntity(); /*!< constructor */
    ~sdEntity(); /*!< desctrictor */
    
    /*! @name Setter/Getter
     @{
     */
    
    /*! return a set of sdEvent pointers. sorted by time.*/
    set<sdEvent*, sdEventCompare> getEventSet(void);
    set<sdEvent*, sdEventCompare> getRangedEventSet(double start, double end);
    set<sdEvent*, sdEventCompare> getFilteredEventSet(double start, double end, EDescriptor descriptor);

    /*! return number of registerd events in the eventSet*/
    int getNumberOfEvents(void);

    /*!
     @}
     */
    
    /*! @name Event Handling
     @{
     */
    
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
    void removeEvent(EDescriptor descriptor, double time);
    
    /*!
     remove events in the specified period
     @param descriptor the descriptor of sdEvent to be removed
     @param start specifies the start point of the removal
     @param end specifies the end point of the removal
     */
    void removeEvent(EDescriptor descriptor, double start, double end);
    
    /*!
     remove all events in the eventSet
     */
    void removeAllEvents();
    
    /*!
     @}
     */
    
    /*! @name Virtual functions
     @{
     */
    
    /*!
     query method. it simply returns a pointer to the buffer, where the designated data are stored. returns null if not found. This is a pure abstract function. The subclass must override.
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
    
    
    /*!
     this function is the only way to instantiate sdEvent and this function is responsible for allocating the void pointer properly based on the provoded descriptor
     */
    virtual sdEvent* addEvent(double time, EDescriptor descriptor, void* value) = 0;
    virtual sdEvent* addEvent(string time, string descriptor, string value) = 0;
    
    virtual void removeEvent(double time, EDescriptor descriptor) = 0;

    /*!
     @}
     */

    virtual bool isDescriptorRelevant(EDescriptor descriptor) = 0;

};

/*** inline implementation ***/

inline sdEntity::sdEntity(){
}

inline sdEntity::~sdEntity(){
    // delete all allocated values of attached events
    eventSet.clear();
}

inline set<sdEvent*, sdEventCompare> sdEntity::getEventSet(void){
    return eventSet;
}
    
inline int sdEntity::getNumberOfEvents(){
    return static_cast<int>(eventSet.size());
}

inline void sdEntity::removeAllEvents(){
    eventSet.clear();
}

inline sdEvent* sdEntity::getEvent(double time, EDescriptor descriptor){
    set<sdEvent*, sdEventCompare>::iterator it = eventSet.begin();
    while(it != eventSet.end()){
        sdEvent* event = *it;
        if((event->getTime() == time) && (event->getDescriptor() == descriptor)){
            return event;
        }
        ++it;
    }
    return NULL;
}

inline void* sdEntity::getValue(double time, EDescriptor descriptor){
    
    sdEvent *evt = getEvent(time, descriptor);
    if(evt){
        return evt->getValue();
    }
    return NULL;
}

inline set <sdEvent*, sdEventCompare>sdEntity::getRangedEventSet(double start, double end){
    set <sdEvent*, sdEventCompare>rangedSet;
    set <sdEvent*, sdEventCompare>::iterator it = eventSet.begin();
    while(it != eventSet.end()){
        sdEvent* event = *it;
        if ( (event->getTime() >= start) && (event->getTime() <= end)) {
            rangedSet.insert(*it);
        }
        ++it;
    }
    return rangedSet;
}

inline set <sdEvent*, sdEventCompare>sdEntity::getFilteredEventSet(double start, double end, EDescriptor descriptor){
    
    set <sdEvent*, sdEventCompare>rangedSet;
    set <sdEvent*, sdEventCompare>::iterator it = eventSet.begin();
    while(it != eventSet.end()){
        sdEvent* event = *it;
        if ( (event->getTime() >= start) && (event->getTime() <= end)) {
            if(event->getDescriptor() == descriptor){
                rangedSet.insert(*it);
            }
        }
        ++it;
    }
    return rangedSet;
}

#endif /* defined(____sdEntity__) */

