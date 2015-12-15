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
    std::vector<std::shared_ptr<sdProtoEvent>>::const_iterator findEvent(const double &time, const EDescriptor &descriptor) const;

    /*! iterate with lambda */
    template <typename L> void iterate(L lambda) const;
    
    /*! sort utility function */
    void sort();

public:
    
    /*! this function is the only way to instantiate sdEvent.*/
    template <EDescriptor D>
    sdEvent<D> * const addEvent(const double &time,  typename sdDescriptor<D>::type value);

    /*! @name Event Handling @{ */

    /*! return number of registerd events in the eventSet */
    size_t getNumberOfEvents(void) const;

    /*! @name Getting  Event(s) in the specified Range @{ */

    /*! return single event with specific time and descriptor*/
    template <EDescriptor D>
    const sdEvent< D > * const getEvent(const double &time) const;

    /*! returns a set of sdProtoEvents, depending on given filter arguments*/
    std::set<std::shared_ptr<sdProtoEvent> > getEventSet(const double &time) const;
    
    /*! returns a vector of sdProtoEvents between start and end time */
    std::vector<std::shared_ptr<sdProtoEvent>> getEvents(const double &start, const double &end) const;
    
    /*! return a vectir of sdProtoEvents with designated descriptor between start and end time */
    std::vector<std::shared_ptr<sdProtoEvent>> getEvents(const double &start, const double &end, const EDescriptor &descriptor) const;

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

    template <EDescriptor D>
    sdEvent<D>  * const getNextEvent(const double time) const;
    
    std::set<std::shared_ptr<sdProtoEvent>> getNextEventSet(const double time) const;

    /*!
     return the time tag of the next event
     */
    std::pair<double, bool>  getNextEventTime(const double time) const;

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

    template<EDescriptor D>
    sdEvent<D> * const getPreviousEvent(const double time) const;
    
    /*!
     return previous events from the given time index .
     @param time index
     */
    std::set<std::shared_ptr<sdProtoEvent>> getPreviousEventSet(const double time) const;
    
    /*!
     return the time tag of the next event
     */
    std::pair<double, bool> getPreviousEventTime(const double time) const;
    
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

    
    /*< remove an event from the events vector
     @param event the event to be removed
     */
    bool removeEvent(const std::shared_ptr<sdProtoEvent> &event);
    
    /*< remove an event from the
     @param a pointer to a raw proto event to be removed.
     */
    bool removeEvent(const sdProtoEvent * const event);

    /*! remove an event at the specified time and descriptor
     @param time the time of sdEvent to be removed.
     @param descriptor the descriptor of sdEvent to be removed */
    bool removeEvent(const double &time, const EDescriptor &descriptor);
    //bool removeEvent(std::string time, std::string descriptor);
    
    /*! remove all events in the eventSet */
    void removeAllEvents();

    /*!
     @}
     */

    /*! @name Getting value directly
     @{
     */

    /*!
     this function looks for an event at specified time and descriptor and returns a pointer to the data.
     @param time time of the event
     @param descriptor specify descriptor defined in sdConst.h
     */
    template <EDescriptor D>
    const typename sdDescriptor<D>::type * const getValue(const double &time) const;
    
    
    /*!
     this function search for an event at specified time and descriptor and return the data as a string.
     @param time time of the event
     @param descriptor specify descriptor defined in sdConst.h
     */
    template <EDescriptor D>
    const std::string getValueAsString(const double &time) const;

    /*!
     this function looks for a next event from the specified time with the designated descriptor and returns a pointer to the data.
     @param time 
     @param descriptor specify descriptor defined in sdConst.h
     */
    template <EDescriptor D>
    const typename sdDescriptor<D>::type * const getNextValue(const double &time) const;

    /*!
     this function looks for a next event from the specified time with the designated descriptor and returns a pointer to the data.
     @param time 
     @param descriptor specify descriptor defined in sdConst.h
     */
    template <EDescriptor D>
    const typename sdDescriptor<D>::type * const getPreviousValue(const double &time) const;
   /*!
     @} 
    */
};

std::vector<std::shared_ptr<sdProtoEvent>>::const_iterator sdEntity::findEvent(const double &time, const EDescriptor &descriptor) const{
    for (auto it = events.begin(); it != events.end(); it++) {
        if( (almostEqual(time, (*it)->getTime())) && (descriptor == (*it)->getDescriptor()) ){ return it; }
        if( (*it)->getTime() > time ){return events.end();}
    }
    return events.end();
}

template <typename L>
inline void sdEntity::iterate(L lambda) const{
    for_each(events.begin(), events.end(), lambda);
}

template <EDescriptor D>
inline sdEvent<D> * const sdEntity::addEvent(const double &time,  typename sdDescriptor<D>::type value){
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
    return dynamic_cast<sdEvent<D>*>(event.get());
}

template <EDescriptor D>
inline const sdEvent< D > * const sdEntity::getEvent(const double &time) const{
    auto it = findEvent(time, D);
    if(it == events.end()){return nullptr;}
    return dynamic_cast<const sdEvent<D> *>((*it).get());
}

inline std::set<std::shared_ptr<sdProtoEvent>> sdEntity::getEventSet(const double &time) const{
    std::set<std::shared_ptr<sdProtoEvent>> set;;
    for(auto it = events.begin(); it != events.end(); it++){
        if(almostEqual((*it)->getTime(), time)){set.insert(*it);}
        if((*it)->getTime() > time )break;
    }
    return std::move(set);
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
    if(events.empty()) return std::set<std::shared_ptr<sdProtoEvent>>(); // no event in entity
    return getEventSet(events.front()->getTime());
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
    if(events.empty()) return std::set<std::shared_ptr<sdProtoEvent>>(); // no event in entity
    return getEventSet(events.back()->getTime());
}

inline std::pair<double, bool> sdEntity::getLastEventTime() const{
    auto set = getLastEventSet();
    if(set.empty()) return std::pair<double, bool>{0.0,false};
    return std::move( std::pair<double, bool>{(*(set.rbegin()))->getTime(), true});
}

template <EDescriptor D>
inline sdEvent<D> * const sdEntity::getNextEvent(const double time) const{
    for(auto it = events.begin(); it != events.end(); it++){
        if(((*it)->getTime() > time) && ((*it)->getDescriptor() == D))
            return dynamic_cast<sdEvent<D> *>((*it).get());
    }
    return nullptr;
}

std::set<std::shared_ptr<sdProtoEvent>> sdEntity::getNextEventSet(const double time) const{
    for(auto it = events.begin(); it != events.end(); it++){
        if((*it)->getTime() > time){
            return std::move(getEventSet((*it)->getTime()));
        }
    }
    return std::set<std::shared_ptr<sdProtoEvent>>(); // empty set
}

std::pair<double, bool> sdEntity::getNextEventTime(const double time) const{
    auto set = getNextEventSet(time);
    if(set.empty()) return std::make_pair(0.0, false);
    return std::make_pair( (*(set.begin()))->getTime(), true);
}


template <EDescriptor D>
inline sdEvent<D> * const sdEntity::getPreviousEvent(const double time) const{
    for(auto it = events.rbegin(); it != events.rend(); it++){
        if(((*it)->getTime() < time) && ((*it)->getDescriptor() == D))
            return dynamic_cast<sdEvent<D> *>((*it).get());
    }
    return nullptr;
}

std::set<std::shared_ptr<sdProtoEvent>> sdEntity::getPreviousEventSet(const double time) const{
    for(auto it = events.rbegin(); it != events.rend(); it++){
        if((*it)->getTime() < time){
            return std::move(getEventSet((*it)->getTime()));
        }
    }
    return std::move(std::set<std::shared_ptr<sdProtoEvent>>()); // empty set
}

std::pair<double, bool> sdEntity::getPreviousEventTime(const double time) const{
    auto set = getPreviousEventSet(time);
    if(set.empty()) return std::make_pair(0.0, false);
    return std::make_pair( (*(set.begin()))->getTime(), true);
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

inline bool sdEntity::removeEvent(const sdProtoEvent * const event){
    for (auto it = events.begin(); it != events.end(); it++) {
        if((*it).get() == event){
            events.erase(it);
            return true;
        }
    }
    return false;
}

inline bool sdEntity::removeEvent(const double &time, const EDescriptor &descriptor){
    auto it = findEvent(time, descriptor);
    if(it == events.end()) return false;
    events.erase(it);
    return true;
}

//inline bool sdEntity::removeEvent(std::string time, std::string descriptor){
//    return false;
//}


inline void sdEntity::removeAllEvents(){
    events.clear();
}

inline size_t sdEntity::getNumberOfEvents() const{
    return events.size();
}

template <EDescriptor D>
inline const typename sdDescriptor<D>::type * const sdEntity::getValue(const double &time) const{
    auto event = getEvent<D>(time);
    if(!event) return  nullptr;
    return &(event->getValue());
}

template <EDescriptor D>
const std::string sdEntity::getValueAsString(const double &time) const{
    auto event = getEvent<D>(time);
    if(!event) return  std::string();
    return getEvent<D>(time)->getValueAsString();
}


template <EDescriptor D>
const typename sdDescriptor<D>::type * const sdEntity::getNextValue(const double &time) const{
    auto nextEvent = getNextEvent<D>(time);
    if(!nextEvent) return nullptr;
    return &nextEvent->getValue();
}

template <EDescriptor D>
const typename sdDescriptor<D>::type * const sdEntity::getPreviousValue(const double &time) const{
    auto previousEvent = getPreviousEvent<D>(time);
    if(!previousEvent) return nullptr;
    return &previousEvent ->getValue();
}


#endif