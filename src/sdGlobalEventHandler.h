//
//  sdGlobalEventHandler.h
//  libSpatDIFTest
//
//  Created by Chikashi Miyama on 23/09/16.
//  Copyright © 2016 ICST. All rights reserved.
//

#pragma once


class sdEntity;
class sdGlobalEventHandler{
public:
    void sortAllEvents();
    bool removeEventAlias(const sdEntity* const entity, const double &time, const EDescriptor & descriptor );
    std::vector<std::pair<const sdEntity*, std::shared_ptr<sdProtoEvent>>> getEventsFromAllEntities(const double &time) const;
    std::vector<std::pair<const sdEntity*, std::shared_ptr<sdProtoEvent>>> getEventsFromAllEntities(const double &start, const double &end) const;
    
    std::vector<std::pair<const sdEntity* , std::shared_ptr<sdProtoEvent>>> getNextEventsFromAllEntities(const double &time) const;
    std::pair<double, bool> getPreviousEventTime(const double &time) const;
    std::pair<double, bool> getDeltaTimeFromPreviousEvent(const double &time) const;
    std::pair<double, bool> getNextEventTime(const double &time) const;
    std::pair<double, bool> getDeltaTimeToNextEvent(const double &time) const;
    const std::vector<std::pair<sdEntity*, std::shared_ptr<sdProtoEvent>>> &getAllEvents() const;
    std::vector<std::pair<const sdEntity* , std::shared_ptr<sdProtoEvent>>> getPreviousEventsFromAllEntities(const double &time) const;
    std::vector<std::pair<const sdEntity*, std::shared_ptr<sdProtoEvent>>> getEventsFromAllEntities() const;
    std::vector<std::pair<const sdEntity* , std::shared_ptr<sdProtoEvent>>>  getFirstEventsFromAllEntities() const;
    std::vector<std::pair<const sdEntity* , std::shared_ptr<sdProtoEvent>>> getLastEventsFromAllEntities() const;

    void addEventAlias(sdEntity * const entity, std::shared_ptr<sdProtoEvent> event, bool autosort = true);

protected:
    std::vector<std::pair<sdEntity*, std::shared_ptr<sdProtoEvent>>> allEvents; //!< an alias pointer to all events
};

inline void sdGlobalEventHandler::sortAllEvents(){
    std::sort(allEvents.begin(), allEvents.end(),
              [](std::pair<const sdEntity* , std::shared_ptr<sdProtoEvent>> eventA,
                 std::pair<const sdEntity* , std::shared_ptr<sdProtoEvent>> eventB)->bool{
                  return eventA.second->getTime() < eventB.second->getTime();
              });
}

inline bool sdGlobalEventHandler::removeEventAlias(const sdEntity* const entity, const double &time, const EDescriptor & descriptor ){
    auto it = std::find_if(allEvents.begin(), allEvents.end(), [&entity, &descriptor, &time]( std::pair<const sdEntity * ,std::shared_ptr<sdProtoEvent>> event){
        return sdUtils::almostEqual(event.second->getTime(), time) && (event.second->getDescriptor() == descriptor) && (event.first == entity);
    });
    if(it == allEvents.end()) return false;
    allEvents.erase(it);
    return true;
}

/*!   collect next event(s) from all entities and report them  */
inline std::vector<std::pair<const sdEntity*, std::shared_ptr<sdProtoEvent>>> sdGlobalEventHandler::getEventsFromAllEntities(const double &time) const{
    std::vector<std::pair<const sdEntity*, std::shared_ptr<sdProtoEvent>>> matchedEvents;
    for (auto it = allEvents.begin(); it != allEvents.end(); it++) {
        if(sdUtils::almostEqual((*it).second->getTime(), time)){matchedEvents.push_back(*it);}
    }
    
    return std::move(matchedEvents);
}

inline std::vector<std::pair<const sdEntity*, std::shared_ptr<sdProtoEvent>>> sdGlobalEventHandler::getEventsFromAllEntities(const double &start, const double &end) const{
    std::vector<std::pair<const sdEntity*, std::shared_ptr<sdProtoEvent>>> vector;
    for_each(allEvents.begin(), allEvents.end(), [&vector, &start, &end]( std::pair<const sdEntity*, std::shared_ptr<sdProtoEvent>> event ){
        if( start <= event.second->getTime() && event.second->getTime() <= end){ vector.push_back(event);}
    });
    return std::move(vector);
}

inline std::vector<std::pair<const sdEntity* , std::shared_ptr<sdProtoEvent>>> sdGlobalEventHandler::getNextEventsFromAllEntities(const double &time) const{
    auto result = std::find_if(allEvents.begin(), allEvents.end(), [&time](std::pair<const sdEntity* , std::shared_ptr<sdProtoEvent>> event){
        if(event.second->getTime() > time){return true;}
        else return false;
    });
    
    if(result == allEvents.end()){return std::vector<std::pair<const sdEntity*, std::shared_ptr<sdProtoEvent>>>();}
    return getEventsFromAllEntities((*result).second->getTime());
}

inline std::pair<double, bool> sdGlobalEventHandler::getPreviousEventTime(const double &time) const{
    auto set = getPreviousEventsFromAllEntities(time);
    if(set.empty()) return std::make_pair(0.0, false);
    return std::make_pair((*set.begin()).second->getTime(), true);
}

inline std::pair<double, bool> sdGlobalEventHandler::getDeltaTimeFromPreviousEvent(const double &time) const{
    auto previousEventTime = getPreviousEventTime(time);
    if(!previousEventTime.second)return std::make_pair(0.0, false);
    return std::make_pair(time-previousEventTime.first, true);
}

inline std::pair<double, bool> sdGlobalEventHandler::getNextEventTime(const double &time) const{
    auto set = getNextEventsFromAllEntities(time);
    if(set.empty()) return std::make_pair(0.0, false);
    return std::make_pair((*set.begin()).second->getTime(), true);
}

inline std::pair<double, bool> sdGlobalEventHandler::getDeltaTimeToNextEvent(const double &time) const{
    auto nextEventTime = getNextEventTime(time);
    if(!nextEventTime.second)return std::make_pair(0.0, false);
    return std::make_pair(nextEventTime.first - time, true);
}

inline const std::vector<std::pair<sdEntity*, std::shared_ptr<sdProtoEvent>>> &sdGlobalEventHandler::getAllEvents() const{
    return allEvents;
}

inline std::vector<std::pair<const sdEntity* , std::shared_ptr<sdProtoEvent>>> sdGlobalEventHandler::getPreviousEventsFromAllEntities(const double &time) const{
    auto result = std::find_if(allEvents.rbegin(), allEvents.rend(), [&time](std::pair<const sdEntity* , std::shared_ptr<sdProtoEvent>> event){
        if(event.second->getTime() < time){return true;}
        else return false;
    });
    
    if(result == allEvents.rend()){return std::vector<std::pair<const sdEntity*, std::shared_ptr<sdProtoEvent>>>();}
    return getEventsFromAllEntities((*result).second->getTime());
}

inline std::vector<std::pair<const sdEntity* , std::shared_ptr<sdProtoEvent>>>  sdGlobalEventHandler::getFirstEventsFromAllEntities() const{
    if(allEvents.empty()) return std::vector<std::pair<const sdEntity* , std::shared_ptr<sdProtoEvent>>> ();
    return getEventsFromAllEntities(allEvents.front().second->getTime());
}

inline std::vector<std::pair<const sdEntity* , std::shared_ptr<sdProtoEvent>>> sdGlobalEventHandler::getLastEventsFromAllEntities() const{
    if(allEvents.empty()) return std::vector<std::pair<const sdEntity* , std::shared_ptr<sdProtoEvent>>> ();
    return getEventsFromAllEntities(allEvents.back().second->getTime());
}

inline void sdGlobalEventHandler::addEventAlias(sdEntity * const entity, std::shared_ptr<sdProtoEvent> event, bool autosort){
    if(autosort) removeEventAlias(entity, event->getTime(), event->getDescriptor());
    allEvents.push_back(std::make_pair(entity, event));
    if(autosort) sortAllEvents();
}




