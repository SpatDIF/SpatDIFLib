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

#ifndef ____sdEntity__
#define ____sdEntity__

#include <set>
#include <vector>
#include <string>
#include <iostream>
#include "sdConst.h"

using namespace std;

/**
 * @brief  A pure virtual class of event. This class maintains data of time, descriptor, and value and responsible for converting incoming and outgoing data.
 */

class sdEvent{
    
protected:
    double time;/*!< time of the event */
    EDescriptor descriptor;/*!<descriptor of the event*/
    void *value;/*!<value of the event. the subclasses must allocates this void pointer */
    
public:
    /*! @name Setter
     @{ */
    
    /*!
     set all three parameters at once
     @param time time of event
     @param desciptor desciprot of event declared in sdConst.h
     @param value value of event
     */
    void set(double time, EDescriptor descriptor, void* value);
    
    /*! @} */

    /*! @name Time handling
     @{ */
    
    /*! set time of the event
     @param time time of the event */
    void setTime(double time);
    
    /*! set time of the event
     @param time time of the event as String */
    void setTime(string timeString);
    
    /*!returns time as double*/
    double getTime(void);
    
    /*!returns time as string*/
    string getTimeAsString(void);
    
    /*! @} */

    /*! @name Value handling
     @{ */
    
    /*! descriptor and value should be set at once. because this function allocates memory to void* value based on the descriptor*/
    virtual bool setValue(EDescriptor descriptor, void* value) = 0;
    
    /*!overloaded function*/
    virtual bool setValue(string descriptor, string value) = 0;
    
    /*!returns descriptor as Enum*/
    EDescriptor getDescriptor(void);
    
    /*!returns descriptor as string. a routine for conversion should be implemented in derived classes*/
    virtual string getDescriptorAsString(void) = 0;
    
    /*!returns value through void pointer. It should be casted to propoer type manually.*/
    void* getValue(void);

    /*!returns value as string. a routine for conversion should be implemented in derived classes */
    virtual string getValueAsString(void) = 0;

    /*! @} */
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
 sdEntity is a pure abstract class. This class maintains and handles all events associated to relevant descriptors. This class is also responsible for answering queries about it's relevant descriptors.
*/
class sdEntity{
    typedef enum{
        SD_ENTITY_PREVIOUS,
        SD_ENTITY_NEXT,
        SD_ENTITY_CURRENT
    } EMode;
    
protected:
    multiset<sdEvent*, sdEventCompare> eventSet; /*!< maintains pointers to all relevant sdEvents */

public:
    /*!< desctructor. this function erases all contents of eventSet. 
     Subsequently, all allocated buffers stored in the eventSet will be destroyed*/
    ~sdEntity();
    
    /*! @name Event Handling @{ */

    /*! return number of registerd events in the eventSet */
    virtual int getNumberOfEvents(void);
    
    /*! @name Getting  Event(s) in the specified Range @{ */

    /*! return single event with specific time and descriptor*/
    virtual sdEvent* getEvent(double time, EDescriptor descriptor);

    /*! return a multiset of sdEvent pointers, depending on given filter arguments*/
    virtual multiset<sdEvent*, sdEventCompare> getEventSet(void);
    virtual multiset<sdEvent*, sdEventCompare> getEventSet(double time);
    virtual multiset<sdEvent*, sdEventCompare> getEventSet(double start, double end);
    virtual multiset<sdEvent*, sdEventCompare> getEventSet(double start, double end, EDescriptor descriptor);

    /*! @} */
    

    /*! @name Next Event(s)
     @{
     */
    

    
    /**
     * @brief return a multiset of chronologically next event from the provided time 
     * 
     * @param time index time
     * @return a multiset of next events
     */
    multiset<sdEvent*, sdEventCompare> getNextEventSet(double time);

      
    sdEvent* getNextEvent(double time, EDescriptor descriptor);


    /*!
     return the time tag of the next event
     */
    double getNextEventTime(double time);

    /*!
     @}
     */
    
    /*! @name Previous Event(s)
     @{
     */
    
    /*!
     return previous event from the given time index that holds the specified descriptor.
     @param time index
     @param descriptor the descriptor of the event declared in sdConst.h
     */
    
    sdEvent* getPreviousEvent(double time, EDescriptor descriptor);
    
    /*!
     return previous events from the given time index .
     @param time index
     */
    multiset<sdEvent*, sdEventCompare> getPreviousEventSet(double time);
    
    /*!
     return the time tag of the next event
     */
    double getPreviousEventTime(double time);
    
    /*!
     @}
     */

    /*! @name Getting frist Event(s)
     @{
     */
    
    /*!
     return the very first event with the specified descriptor.
     @param descriptor the descriptor of the event declared in sdConst.h
     */
    sdEvent* getFirstEvent(EDescriptor descriptor);
    
    /*!
     return the very first events regardless of descriptors.
     */
    multiset<sdEvent*, sdEventCompare> getFirstEventSet();

    /*!
     return the time tag of the first event
     */
    double getFirstEventTime();
    
    /*!
     @}
     */
    
    /*! @name Last Event(s)
     @{
     */
    
    /*!
     return the timeTag of the very last event with the specified descriptor.
     @param descriptor the descriptor of the event declared in sdConst.h
     */
    sdEvent* getLastEvent(EDescriptor descriptor);

    /*!
     return the very last events regardless of descriptors.
     */
    multiset<sdEvent*, sdEventCompare> getLastEventSet();
    
    /*!
     return the time tag of the last event
     */
    double getLastEventTime();

    /*!
    }
     */
    
     
    /*!
     this function is the only way to instantiate sdEvent.
     */
    virtual sdEvent* addEvent(double time, EDescriptor descriptor, void* value) = 0;
    virtual sdEvent* addEvent(string time, string descriptor, string value) = 0;
    
    /*<
     remove an event from the set
     @param event the event to be removed
     */
    void removeEvent(sdEvent *event);
    
    /*!
     remove an event at the specified time and descriptor
     @param time the time of sdEvent to be removed.
     @param descriptor the descriptor of sdEvent to be removed
     */
    void removeEvent(double time, EDescriptor descriptor);
    virtual void removeEvent(string time, string descriptor) = 0;
    
    /*!
     remove all events in the eventSet
     */
    void removeAllEvents();
    
    /*!
     @}
     */
    
    /*! @name Getting value directly
     @{
     */

    /*!
     this function looks for an event at specified time and descriptor and returns a pointer to the data.
     @param time 
     @param descriptor specify descriptor defined in sdConst.h
     */
    void* getValue(double time, EDescriptor descriptor);

    /*!
     this function looks for a next event from the specified time with the designated descriptor and returns a pointer to the data.
     @param time 
     @param descriptor specify descriptor defined in sdConst.h
     */
    void* getNextValue(double time, EDescriptor descriptor);

    /*!
     this function looks for a next event from the specified time with the designated descriptor and returns a pointer to the data.
     @param time 
     @param descriptor specify descriptor defined in sdConst.h
     */
    void* getPreviousValue(double time, EDescriptor descriptor);

    /*!
     @} 
     */
    
private:
    
    multiset<sdEvent*, sdEventCompare> createEventSet(double time,  EMode mode );

};


#endif /* defined(____sdEntity__) */

