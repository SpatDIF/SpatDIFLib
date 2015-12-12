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
 * @copyright Copyright (C) 2013 - 2016 Zurich University of the Arts, Institute for Computer Music and Sound Technology @n
 * This code is licensed under the terms of the "New BSD License" @n
 * http://creativecommons.org/licenses/BSD/
 */

#ifndef ____sdEntity__
#define ____sdEntity__

#include <vector>
#include <string>
#include <memory>
#include "sdException.h"
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
    
    /*!sdEntity can be invoked only by sdScene*/
    sdEntity(const sdScene * const parent):parent(parent){};

    /* find event with the descriptor at time */
    std::vector<std::shared_ptr<sdProtoEvent>>::const_iterator findEvent(const double time, EDescriptor descriptor) const;

    /*! iterate with lambda */
    template <typename L> void iterate(L lambda) const;
    
    /*! sort utility function */
    void sort();

public:
    
    /*! this function is the only way to instantiate sdEvent.*/
    template <EDescriptor D>
    const std::shared_ptr<sdProtoEvent> addEvent(const double &time,  typename sdDescriptor<D>::type value);
    
//    template <typename T>
//    vector<T> getValueVector(shared_ptr<_Event> &event) const{
//        const DescriptorFormat *  format = fetchDescriptorFormat(event->getDescriptor());
//        if (format->type != typeid(T)) {
//            return {}; // empty
//        }
//        return dynamic_cast<Event<T>*>(event.get())->getValues(); // downcast
//    };


    /*! @name Event Handling @{ */

    /*! return number of registerd events in the eventSet */
    size_t getNumberOfEvents(void) const;

    /*! @name Getting  Event(s) in the specified Range @{ */

    /*! return single event with specific time and descriptor*/
    template <EDescriptor D>
    const sdEvent< D > * const getEvent(const double time) const;

    /*! returns a set of sdProtoEvents, depending on given filter arguments*/
    std::set<std::shared_ptr<sdProtoEvent> > getEventSet(const double time) const;
    
    /*! returns a vector of sdProtoEvents between start and end time */
    std::vector<std::shared_ptr<sdProtoEvent>> getEvents(const double start, const double end) const;
    
    /*! return a vectir of sdProtoEvents with designated descriptor between start and end time */
    std::vector<std::shared_ptr<sdProtoEvent>> getEvents(const double start, const double end, const EDescriptor descriptor) const;

    /*! @} */

    /*! @name Next Event(s)
     @{
     */
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
    /*! @name Getting frist Event(s)
     @{
     */

    /*!
     return the very first event with the specified descriptor.
     @param descriptor the descriptor of the event declared in sdConst.h
     */
    
    template <EDescriptor D>
    const sdEvent< D > * const getFirstEvent() const;

    /*!
     return the very first events regardless of descriptors. 
     @return the first events in the entity. if no events are assigned return nullptr.
     */
    const std::set< std::shared_ptr<sdProtoEvent> > getFirstEventSet() const;

    /*!
     return the time tag of the first event
     @return pair of double and bool, if no events are registered in the entity the second element of the pair is false
     */
    std::pair<double, bool> getFirstEventTime() const;

    /*!
     @}
     */

    /*! @name Last Event(s)
     @{
     */
    
    /*!
     return the timeTag of the very last event with the specified descriptor.
     @param descriptor the descriptor of the event declared in sdConst.h
     @return a const pointer to an event
     */
    template <EDescriptor D>
    const sdEvent< D > * const getLastEvent() const;

    /*!
     return the very last events regardless of descriptors.
     @return the last events in the entity. if no events are assigned return nullptr.
     */
    const std::set< std::shared_ptr<sdProtoEvent> > getLastEventSet() const;

    /*!
     return the time tag of the last event
     */
    std::pair<double, bool>  getLastEventTime() const;

    /*!
    }
     */

    
    /*<
     remove an event from the set
     @param event the event to be removed
     */
    bool removeEvent(const std::shared_ptr<sdProtoEvent> &event);
    
    /*! remove an event at the specified time and descriptor
     @param time the time of sdEvent to be removed.
     @param descriptor the descriptor of sdEvent to be removed */
    bool removeEvent(double time, EDescriptor descriptor);
    bool removeEvent(std::string time, std::string descriptor);
    
    /*! remove all events in the eventSet */
    void removeAllEvents();

    /*!
     @}
     */

    /*! @name Getting value directly
     @{
     */
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

std::vector<std::shared_ptr<sdProtoEvent>>::const_iterator sdEntity::findEvent(const double time, EDescriptor descriptor) const{
    std::vector< std::shared_ptr<sdProtoEvent> > ::const_iterator it = events.begin();
    while (it != events.end()) {
        if( (almostEqual(time, (*it)->getTime())) && (descriptor == (*it)->getDescriptor()) ){ return it; }
        it++;
    }
    return it;
}

template <typename L>
inline void sdEntity::iterate(L lambda) const{
    for_each(events.begin(), events.end(), lambda);
}

template <EDescriptor D>
inline const std::shared_ptr<sdProtoEvent> sdEntity::addEvent(const double &time,  typename sdDescriptor<D>::type value){
    if(time < 0.0){ throw  InvalidTimeException(time);}
    // extension activated?
    
    // remove if already exist
    removeEvent(time, D);
    
    // add
    auto event = std::shared_ptr<sdProtoEvent>(new sdEvent<D>(time, this, value));
    events.push_back(event);
    
    // sort
    std::sort(events.begin(), events.end(),
              [](std::shared_ptr<sdProtoEvent> eventA, std::shared_ptr<sdProtoEvent> eventB)->bool{
             return eventA->getTime() < eventB->getTime(); });
    return event;
}

template <EDescriptor D>
inline const sdEvent< D > * const sdEntity::getEvent(const double time) const{
    auto it = findEvent(time, D);
    if(it == events.end()){return nullptr;}
    return dynamic_cast<const sdEvent<D> *>((*it).get());
}

template <EDescriptor D>
inline const sdEvent< D > * const sdEntity::getFirstEvent() const{
    auto it = events.begin();
    while (it != events.end()) {
        if((*it)->getDescriptor() == D)
            return dynamic_cast< sdEvent<D> *>((*it).get());
        it++;
    }
    return nullptr;
}

inline const std::set< std::shared_ptr<sdProtoEvent> > sdEntity::getFirstEventSet() const{
    std::set< std::shared_ptr<sdProtoEvent> > firstEventSet;
    if(events.empty()) return firstEventSet; // no event in entity
    
    auto it = events.begin();
    double time = (*it)->getTime();
    firstEventSet.insert(*it);
    while (it != events.end()){
        if( almostEqual(time, (*it)->getTime())){ firstEventSet.insert(*it);
        }else{ break; }
        it++;
    }
    return firstEventSet;
}

inline std::pair<double, bool> sdEntity::getFirstEventTime() const{
    auto set = getFirstEventSet();
    if(set.empty()) return std::pair<double, bool>{0.0,false};
    return std::move( std::pair<double, bool>{(*(set.begin()))->getTime(), true});
}

template <EDescriptor D>
inline const sdEvent< D > * const sdEntity::getLastEvent() const{
    auto it = events.rbegin();
    while (it != events.rend()) {
        if((*it)->getDescriptor() == D)
            return dynamic_cast< sdEvent<D> *>((*it).get());
        it++;
    }
    return nullptr;
}

inline const std::set< std::shared_ptr<sdProtoEvent> > sdEntity::getLastEventSet() const{
    std::set< std::shared_ptr<sdProtoEvent> > lastEventSet;
    if(events.empty()) return lastEventSet; // no event in entity
    
    auto it = events.rbegin();
    double time = (*it)->getTime();
    lastEventSet.insert(*it);
    while (it != events.rend()){
        if( almostEqual(time, (*it)->getTime())){ lastEventSet.insert(*it);
        }else{ break; }
        it++;
    }
    return lastEventSet;
}

inline std::pair<double, bool> sdEntity::getLastEventTime() const{
    auto set = getLastEventSet();
    if(set.empty()) return std::pair<double, bool>{0.0,false};
    return std::move( std::pair<double, bool>{(*(set.rbegin()))->getTime(), true});
}

inline void sdEntity::sort(){
    std::sort(events.begin(), events.end(),
         [](std::shared_ptr<sdProtoEvent> eventA, std::shared_ptr<sdProtoEvent> eventB)->bool{
             return eventA->getTime() < eventB->getTime();
         });
}

inline bool sdEntity::removeEvent(const std::shared_ptr<sdProtoEvent> &event){
    for (auto it = events.begin(); it != events.end(); it++) {
        if(*it == event){
            events.erase(it);
            return true;
        }
    }
    return false;
}

inline bool sdEntity::removeEvent(double time, EDescriptor descriptor){
    auto it = findEvent(time, descriptor);
    if(it == events.end()) return false;
    events.erase(it);
    return true;
}

inline bool sdEntity::removeEvent(std::string time, std::string descriptor){
    return false;
}

inline void sdEntity::removeAllEvents(){
    events.clear();
}

inline size_t sdEntity::getNumberOfEvents() const{
    return events.size();
}

#endif