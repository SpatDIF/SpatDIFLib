
#ifndef ____sdEntity__
#define ____sdEntity__

#include <set>
#include <string>
#include <iostream>
#include "sdConst.h"

using namespace std;

class sdEntity;

/*!
 maintains a set of event data. The constructor is intentionally declared as a protected function. It must be invoked through its subclasses of the friend class sdEntity. This process ensures that all existing instances of sdEvents belong to a sdEntity. the sdEntity is also responsible for the allocation of void* value.
*/
class sdEvent{
    /*! only sdEntity can instantitate this class */
    friend class sdEntity;
public:
    float time;/*!< time of the event */
    EDescriptor descriptor;/*!<descriptor of the event*/
    void *value;/*!<value of the event. sdEntity allocates the void pointer automatically*/
protected:
    /*construcor is intentionally declared as protected. this class must be instantiated through sdEntity::addEvent*/
    sdEvent(){};
};

/*!
 a helper class for sorting events
*/
class sdEventCompare{
public:
    bool operator()(sdEvent* e1, sdEvent* e2){
        if(e1->time < e2->time) // sort by time
            return true;
        else
            return false;
    }
};

/*!
 a helper class for sdSaver. It should not be instantiated directly.
 */
class sdGlobalEvent : public sdEvent{
public:
    string entityName; /*!< name of attached entity */
    EKind kind; /*< kind of attached entity */
};

class sdGlobalEventCompare{
public:
    bool operator()(sdGlobalEvent e1, sdGlobalEvent e2){
        if(e1.time < e2.time){
            return true;
        }else if(e1.time == e2.time){
            if(e1.kind == SD_SOURCE){ // source first then sink
                return true;
            }else{
                return false;
            }
        }
        else{
            return false;
        }
    }
};

/*! sdEntity
 sdEntity is a pure abstract class.  This class maintains and handles all events associated to relavant descriptors. This class is responsible for answering query about it's relavant descriptors.
*/

class sdEntity{

protected:
    std::set<sdEvent*, sdEventCompare> eventSet; /*!< maintains all relevant events */

public:
    sdEntity(); /*!< constructor */
    ~sdEntity(); /*!< desctrictor */
    
    /*!
     query method. it is client's responsibility to provided properly allocated void pointer as an argument value
     @param descriptor specify descriptor defined in sdConst.h
     @param time specify time 
     @param value allocated buffer
     */
    virtual void* getValue(EDescriptor descriptor, float time, void* value) = 0;
    
    /*!
     return all events related to the given descriptor
     @param descriptor the descriptor of the event declared in sdConst.h
     @param time the time of the event in second
     */
    sdEvent* getEvent(float time, EDescriptor descriptor);

    /*!
     return a std::set of sdEvent pointers. sorted by time.
     */
    std::set<sdEvent*, sdEventCompare> getEventSet(void);
    
    /*!
     return number of registerd events in the eventSet
     */
    unsigned int getNumberOfEvents(void);
    
    /*!
     this function is the only way to instantiate sdEvent and this function is responsible for allocating the void pointer properly based on the provoded descriptor
     */
    sdEvent* addEvent(float time, EDescriptor descriptor, void* value);
    
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

inline std::set<sdEvent*, sdEventCompare> sdEntity::getEventSet(void){
    return eventSet;
}

#endif /* defined(____sdEntity__) */

