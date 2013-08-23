
#ifndef ____sdEntity__
#define ____sdEntity__

#include <set>
#include <string>
#include <iostream>
#include "sdEvent.h"
#include "sdConst.h"

using namespace std;

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
    sdEvent* addEvent(string time, string descriptor, string value);

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

