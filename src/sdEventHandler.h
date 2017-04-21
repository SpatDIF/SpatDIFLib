/** @file
 *
 * @ingroup spatdiflib
 *
 * @brief an abstract class of spat DIF event and its template subclass
 *
 * @details
 *
 * @authors Chikashi Miyama, Trond Lossius
 *
 * @copyright Copyright (C) 2013 - 2014 Zurich University of the Arts, Institute for Computer Music and Sound Technology @n
 * This code is licensed under the terms of the "New BSD License" @n
 * http://creativecommons.org/licenses/BSD/
 */


#pragma once

#include "sdUtils.h"

class sdEventHandler{
    /*! @name Event Handling @{ */
protected:
    std::vector< std::shared_ptr<sdProtoEvent> > events; /*!< maintains pointers to all relevant sdEvents */
    
    /*! sort utility function */
    void sort();
    
    /*! add a pointer an event to the global event vector */
    virtual void addGlobalEventAlias(std::shared_ptr<sdProtoEvent> event, bool autosort = true) = 0;

public:
    
    /*! @name Adding  Event(s) @{ */

    /*! this function is the only way to instantiate sdEvent.*/
    template <EDescriptor D>
    std::shared_ptr<sdProtoEvent> addProtoEvent(const double &time, typename sdDescriptorSpec<D>::type value, sdEntity * entity, bool autosort = true);
    
    /*! return value cast to a specific subclass event.*/
    template <EDescriptor D>
    sdEvent<D> * const addEvent(const double &time, typename sdDescriptorSpec<D>::type value, sdEntity * entity);

    /*! @} */
    
    /*! return number of registerd events in the events */
    size_t getNumberOfEvents(void) const;
    
    /*! @name Getting  Event(s) in the specified Range @{ */
    
    /*! return single event with specific time and descriptor*/
    template <EDescriptor D>
    const sdEvent< D > * const getEvent(const double &time) const;
    
    /*! returns a vector of sdProtoEvents, depending on given filter arguments*/
    std::vector<std::shared_ptr<sdProtoEvent> > getEvents(const double &time) const;
    
    /*! return a mapped vector of sdProtoEvents categorized by extension name */
    std::map<EExtension, std::vector<std::shared_ptr<sdProtoEvent>>> getEventsCategorizedByExtension(const double &time) const;

    /*! returns all events as a vector of sdProtoEvents */
    const std::vector<std::shared_ptr<sdProtoEvent>> &getEvents() const;
    
    /*! returns a vector of sdProtoEvents between start and end time */
    std::vector<std::shared_ptr<sdProtoEvent>> getEvents(const double &start, const double &end) const;
    
    /*! return a vector of sdProtoEvents with designated descriptor between start and end time */
    std::vector<std::shared_ptr<sdProtoEvent>> getEvents(const double &start, const double &end, const EDescriptor &descriptor) const;
    

    /*! @} */
    
    /*! @name Next Event(s)
     @{
     */
    
    /**
     * @brief return a next event from the provided time
     *
     * @param time index time
     * @return a next event
     */
    
    template <EDescriptor D>
    sdEvent<D>  * const getNextEvent(const double time) const;
    
    std::vector<std::shared_ptr<sdProtoEvent>> getNextEvents(const double time) const;
    
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
    std::vector<std::shared_ptr<sdProtoEvent>> getPreviousEvents(const double time) const;
    
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
    const std::vector< std::shared_ptr<sdProtoEvent> > getFirstEvents() const;
    
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
    const std::vector< std::shared_ptr<sdProtoEvent> > getLastEvents() const;
    
    /*!
     return the time tag of the last event
     */
    std::pair<double, bool>  getLastEventTime() const;
    
    /*!
     }
     */
    
    
    /*! find event with the descriptor at time */
    std::vector<std::shared_ptr<sdProtoEvent>>::const_iterator findEvent(const double &time, const EDescriptor &descriptor) const;
    
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
    
    /*! remove all events in the events */
    void removeAllEvents();
    

};

template <EDescriptor D>
inline std::shared_ptr<sdProtoEvent> sdEventHandler::addProtoEvent(const double &time,  typename sdDescriptorSpec<D>::type value, sdEntity * entity, bool autosort){
    
    if(time < 0.0){ throw  InvalidTimeException(time);}
    sdDescriptorSpec<D>::validateValue(value);

    // remove if already exist
    removeEvent(time, D);
    
    // add
    auto event = std::shared_ptr<sdProtoEvent>(new sdEvent<D>(time, entity, value));
    events.push_back(event);
    addGlobalEventAlias(event, autosort);
    
    // sort
    if(autosort){
        std::sort(events.begin(), events.end(),
              [](std::shared_ptr<sdProtoEvent> eventA, std::shared_ptr<sdProtoEvent> eventB)->bool{
                  return eventA->getTime() < eventB->getTime(); });
    }
    return event;
}

template <EDescriptor D>
inline sdEvent<D> * const sdEventHandler::addEvent(const double &time, typename sdDescriptorSpec<D>::type value, sdEntity * entity){
    return dynamic_cast<sdEvent<D>*>(addProtoEvent<D>(time, value, entity).get());
}

template <EDescriptor D>
inline const sdEvent< D > * const sdEventHandler::getEvent(const double &time) const{
    auto it = findEvent(time, D);
    if(it == events.end()){return nullptr;}
    return dynamic_cast<const sdEvent<D> *>((*it).get());
}

inline std::vector<std::shared_ptr<sdProtoEvent>> sdEventHandler::getEvents(const double &time) const{
    std::vector<std::shared_ptr<sdProtoEvent>> vector;
    for(auto event : events){
        if(sdUtils::almostEqual(event->getTime(), time)){vector.push_back(event);}
        if(event->getTime() > time )break;
    }
    return std::move(vector);
}

inline std::map<EExtension, std::vector<std::shared_ptr<sdProtoEvent>>> sdEventHandler::getEventsCategorizedByExtension(const double &time) const{
    std::vector<std::shared_ptr<sdProtoEvent>> events = getEvents(time);
    std::map<EExtension, std::vector<std::shared_ptr<sdProtoEvent>>> categorizedEvents;
    for(auto event : events){
        auto extension = sdSpec::getExtensionOfDescriptor(event->getDescriptor());
        if(categorizedEvents.find(extension) == categorizedEvents.end()){ // previously unused extension
            std::vector<std::shared_ptr<sdProtoEvent>> emptyVector;
            categorizedEvents.insert(std::pair<EExtension, std::vector<std::shared_ptr<sdProtoEvent>>>(extension,emptyVector));
        }
        categorizedEvents[extension].push_back(event);
    }
    return std::move(categorizedEvents);
}

inline const std::vector<std::shared_ptr<sdProtoEvent>> &sdEventHandler::getEvents() const{
    return events;
}

inline std::vector<std::shared_ptr<sdProtoEvent>> sdEventHandler::getEvents(const double &start, const double &end) const{
    std::vector<std::shared_ptr<sdProtoEvent>> returnVect;
    for(auto it = events.begin(); it != events.end(); it++){
        double time = (*it)->getTime();
        if(start <= time)returnVect.push_back(*it);
        if(time > end) return std::move(returnVect);
    }
    return std::move(returnVect);
}

inline std::vector<std::shared_ptr<sdProtoEvent>> sdEventHandler::getEvents(const double &start, const double &end, const EDescriptor &descriptor)const{
    std::vector<std::shared_ptr<sdProtoEvent>> returnVect;
    for(auto it = events.begin(); it != events.end(); it++){
        double time = (*it)->getTime();
        if(start <= time && (*it)->getDescriptor() == descriptor)returnVect.push_back(*it);
        if(time > end) return std::move(returnVect);
    }
    return std::move(returnVect);
}

template <EDescriptor D>
inline const sdEvent< D > * const sdEventHandler::getFirstEvent() const{
    auto it = events.begin();
    while (it != events.end()) {
        if((*it)->getDescriptor() == D)
            return dynamic_cast< sdEvent<D> *>((*it).get());
        it++;
    }
    return nullptr;
}

inline const std::vector< std::shared_ptr<sdProtoEvent> > sdEventHandler::getFirstEvents() const{
    if(events.empty()) return std::vector<std::shared_ptr<sdProtoEvent>>(); // no event in entity
    return getEvents(events.front()->getTime());
}


inline std::pair<double, bool> sdEventHandler::getFirstEventTime() const{
    auto vector = getFirstEvents();
    if(vector.empty()) return std::pair<double, bool>{0.0,false};
    return std::move( std::pair<double, bool>{(*(vector.begin()))->getTime(), true});
}

template <EDescriptor D>
inline const sdEvent< D > * const sdEventHandler::getLastEvent() const{
    auto it = events.rbegin();
    while (it != events.rend()) {
        if((*it)->getDescriptor() == D)
            return dynamic_cast< sdEvent<D> *>((*it).get());
        it++;
    }
    return nullptr;
}

inline const std::vector< std::shared_ptr<sdProtoEvent> > sdEventHandler::getLastEvents() const{
    if(events.empty()) return std::vector<std::shared_ptr<sdProtoEvent>>(); // no event in entity
    return getEvents(events.back()->getTime());
}

inline std::pair<double, bool> sdEventHandler::getLastEventTime() const{
    auto vector = getLastEvents();
    if(vector.empty()) return std::pair<double, bool>{0.0,false};
    return std::move( std::pair<double, bool>{(*(vector.rbegin()))->getTime(), true});
}

template <EDescriptor D>
inline sdEvent<D> * const sdEventHandler::getNextEvent(const double time) const{
    for(auto it = events.begin(); it != events.end(); it++){
        if(((*it)->getTime() > time) && ((*it)->getDescriptor() == D))
            return dynamic_cast<sdEvent<D> *>((*it).get());
    }
    return nullptr;
}

inline std::vector<std::shared_ptr<sdProtoEvent>> sdEventHandler::getNextEvents(const double time) const{
    for(auto it = events.begin(); it != events.end(); it++){
        if((*it)->getTime() > time){
            return std::move(getEvents((*it)->getTime()));
        }
    }
    return std::vector<std::shared_ptr<sdProtoEvent>>(); // empty vector
}

inline std::pair<double, bool> sdEventHandler::getNextEventTime(const double time) const{
    auto vector = getNextEvents(time);
    if(vector.empty()) return std::make_pair(0.0, false);
    return std::make_pair( (*(vector.begin()))->getTime(), true);
}


template <EDescriptor D>
inline sdEvent<D> * const sdEventHandler::getPreviousEvent(const double time) const{
    for(auto it = events.rbegin(); it != events.rend(); it++){
        if(((*it)->getTime() < time) && ((*it)->getDescriptor() == D))
            return dynamic_cast<sdEvent<D> *>((*it).get());
    }
    return nullptr;
}

inline std::vector<std::shared_ptr<sdProtoEvent>> sdEventHandler::getPreviousEvents(const double time) const{
    for(auto it = events.rbegin(); it != events.rend(); it++){
        if((*it)->getTime() < time){
            return std::move(getEvents((*it)->getTime()));
        }
    }
    return std::move(std::vector<std::shared_ptr<sdProtoEvent>>()); // empty vector
}

inline std::pair<double, bool> sdEventHandler::getPreviousEventTime(const double time) const{
    auto vector = getPreviousEvents(time);
    if(vector.empty()) return std::make_pair(0.0, false);
    return std::make_pair( (*(vector.begin()))->getTime(), true);
}


#pragma mark removeEvent
inline bool sdEventHandler::removeEvent(const sdProtoEvent * const event){
    for (auto it = events.begin(); it != events.end(); it++) {
        if((*it).get() == event){
            events.erase(it);
            return true;
        }
    }
    return false;
}

inline bool sdEventHandler::removeEvent(const std::shared_ptr<sdProtoEvent> &event){
    return removeEvent(dynamic_cast<sdProtoEvent*>(event.get()));
}

inline bool sdEventHandler::removeEvent(const double &time, const EDescriptor &descriptor){
    auto it = findEvent(time, descriptor);
    if(it == events.end()) return false;
    events.erase(it);
    return true;
}

inline void sdEventHandler::removeAllEvents(){
    events.clear();
}

inline size_t sdEventHandler::getNumberOfEvents() const{
    return events.size();
}

inline std::vector<std::shared_ptr<sdProtoEvent>>::const_iterator sdEventHandler::findEvent(const double &time, const EDescriptor &descriptor) const{
    for (auto it = events.begin(); it != events.end(); it++) {
        if( (sdUtils::almostEqual(time, (*it)->getTime())) && (descriptor == (*it)->getDescriptor()) ){ return it; }
        if( (*it)->getTime() > time ){return events.end();}
    }
    return events.end();
}

inline void sdEventHandler::sort(){
    std::sort(events.begin(), events.end(),
              [](std::shared_ptr<sdProtoEvent> eventA, std::shared_ptr<sdProtoEvent> eventB)->bool{
                  return eventA->getTime() < eventB->getTime();
              });
}

