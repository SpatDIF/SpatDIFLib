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

#include <vector>
#include <string>
#include <memory>
#include "sdConst.h"
#include "sdEvent.h"

/*! sdEntity
 sdEntity is a pure abstract class. This class maintains and handles all events associated to relevant descriptors. This class is also responsible for answering queries about it's relevant descriptors.
*/
class sdScene;
class sdEntity{
    friend sdScene;
    
protected:
    
    std::vector< std::shared_ptr<sdProtoEvent> > events; /*!< maintains pointers to all relevant sdEvents */
    const sdScene * const parent;
    void sort();
    
public:
    sdEntity(const sdScene * const parent):parent(parent){};
    
    template <EDescriptor D>
    const std::shared_ptr<sdProtoEvent> addEvent(const double &time, typename sdDescriptor<D>::type value){
        auto event = std::shared_ptr<sdProtoEvent>(
                                                   new sdEvent<typename sdDescriptor<D>::type>(time,sdDescriptor<D>::type,this,value));
        events.push_back(event);
        

        return event;
    }
    
//    template <typename L>
//    void iterate(L lambda) const{
//        for_each(events.begin(), events.end(), lambda);
//    }
//    
//    template <typename T>
//    vector<T> getValueVector(shared_ptr<_Event> &event) const{
//        const DescriptorFormat *  format = fetchDescriptorFormat(event->getDescriptor());
//        if (format->type != typeid(T)) {
//            return {}; // empty
//        }
//        return dynamic_cast<Event<T>*>(event.get())->getValues(); // downcast
//    };
//    
//    
//    /*! desctructor. this function erases all contents of eventSet.
//     Subsequently, all allocated buffers stored in the eventSet will be destroyed*/
//    virtual ~sdEntity();
//    
//    /*! @name Event Handling @{ */
//
//    /*! return number of registerd events in the eventSet */
//    virtual int getNumberOfEvents(void) const;
//    
//    /*! @name Getting  Event(s) in the specified Range @{ */
//
//    /*! return single event with specific time and descriptor*/
//    virtual sdEvent* getEvent(const double time, const EDescriptor descriptor) const;
//
//    /*! return a multiset of sdEvent pointers, depending on given filter arguments*/
//    virtual std::multiset<sdEvent*, sdEventCompare> getEventSet(void) const;
//    virtual std::multiset<sdEvent*, sdEventCompare> getEventSet(const double time) const;
//    virtual std::multiset<sdEvent*, sdEventCompare> getEventSet(const double start, const double end) const;
//    virtual std::multiset<sdEvent*, sdEventCompare> getEventSet(const double start, const double end, const EDescriptor descriptor) const;
//
//    /*! @} */
//    
//
//    /*! @name Next Event(s)
//     @{
//     */
//    
//    /**
//     * @brief return a multiset of chronologically next event from the provided time 
//     * 
//     * @param time index time
//     * @return a multiset of next events
//     */
//    std::multiset<sdEvent*, sdEventCompare> getNextEventSet(const double time) const;
//
//      
//    sdEvent* getNextEvent(const double time, const EDescriptor descriptor) const;
//
//
//    /*!
//     return the time tag of the next event
//     */
//    double getNextEventTime(const double time) const;
//
//    /*!
//     @}
//     */
//    
//    /*! @name Previous Event(s)
//     @{
//     */
//    
//    /*!
//     return previous event from the given time index that holds the specified descriptor.
//     @param time index
//     @param descriptor the descriptor of the event declared in sdConst.h
//     */
//    
//    sdEvent* getPreviousEvent(const double time, const EDescriptor descriptor) const;
//    
//    /*!
//     return previous events from the given time index .
//     @param time index
//     */
//    std::multiset<sdEvent*, sdEventCompare> getPreviousEventSet(const double time) const;
//    
//    /*!
//     return the time tag of the next event
//     */
//    double getPreviousEventTime(const double time) const;
//    
//    /*!
//     @}
//     */
//
//    /*! @name Getting frist Event(s)
//     @{
//     */
//    
//    /*!
//     return the very first event with the specified descriptor.
//     @param descriptor the descriptor of the event declared in sdConst.h
//     */
//    sdEvent* getFirstEvent(const EDescriptor descriptor) const;
//    
//    /*!
//     return the very first events regardless of descriptors.
//     */
//    std::multiset<sdEvent*, sdEventCompare> getFirstEventSet() const;
//
//    /*!
//     return the time tag of the first event
//     */
//    double getFirstEventTime() const;
//    
//    /*!
//     @}
//     */
//    
//    /*! @name Last Event(s)
//     @{
//     */
//    
//    /*!
//     return the timeTag of the very last event with the specified descriptor.
//     @param descriptor the descriptor of the event declared in sdConst.h
//     */
//    sdEvent* getLastEvent(const EDescriptor descriptor) const;
//
//    /*!
//     return the very last events regardless of descriptors.
//     */
//    std::multiset<sdEvent*, sdEventCompare> getLastEventSet() const;
//    
//    /*!
//     return the time tag of the last event
//     */
//    double getLastEventTime() const;
//
//    /*!
//    }
//     */
//    
//     
//    /*!
//     this function is the only way to instantiate sdEvent.
//     */
//    virtual sdEvent* addEvent(double time, EDescriptor descriptor, void* value) = 0;
//    virtual sdEvent* addEvent(std::string time, std::string descriptor, std::string value) = 0;
//    
//    /*<
//     remove an event from the set
//     @param event the event to be removed
//     */
//    void removeEvent(sdEvent *event);
//    
//    /*!
//     remove an event at the specified time and descriptor
//     @param time the time of sdEvent to be removed.
//     @param descriptor the descriptor of sdEvent to be removed
//     */
//    void removeEvent(double time, EDescriptor descriptor);
//    virtual void removeEvent(std::string time, std::string descriptor) = 0;
//    
//    /*!
//     remove all events in the eventSet
//     */
//    void removeAllEvents();
//    
//    /*!
//     @}
//     */
//    
//    /*! @name Getting value directly
//     @{
//     */
//
//    /*!
//     this function looks for an event at specified time and descriptor and returns a pointer to the data.
//     @param time time of the event
//     @param descriptor specify descriptor defined in sdConst.h
//     */
//    void* getValue(double time, EDescriptor descriptor);
//    
//    
//    /*!
//     this function search for an event at specified time and descriptor and return the data as a string.
//     @param time time of the event
//     @param descriptor specify descriptor defined in sdConst.h
//     */
//    virtual std::string getValueAsString(double time, EDescriptor descriptor) = 0;
//    
//    /*!
//     this function looks for a next event from the specified time with the designated descriptor and returns a pointer to the data.
//     @param time 
//     @param descriptor specify descriptor defined in sdConst.h
//     */
//    void* getNextValue(double time, EDescriptor descriptor);
//
//    /*!
//     this function looks for a next event from the specified time with the designated descriptor and returns a pointer to the data.
//     @param time 
//     @param descriptor specify descriptor defined in sdConst.h
//     */
//    void* getPreviousValue(double time, EDescriptor descriptor);
//   /*!
//     @} 
//    */
};

void sdEntity::sort(){
    std::sort(events.begin(), events.end(),
         [](std::shared_ptr<sdProtoEvent> eventA, std::shared_ptr<sdProtoEvent> eventB)->bool{
             return eventA->getTime() < eventB->getTime();});
}

#endif