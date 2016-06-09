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

#ifndef ____sdScene__
#define ____sdScene__

#include <string>
#include <vector>
#include <map>
#include <unordered_set>
#include <set>
#include "sdConst.h"
#include "sdInfo.h"
#include "sdEntity.h"

/*! is responsible for 
 - adding, removing, and maintaining sdEntityCores.
 - attaching and detaching extensions to the exisiting and newly instantiated cores
 */
class sdScene{
    friend sdEntity;
    
protected:
    std::map <std::string, sdEntity> entities; //!< a map of sdEntities
    std::vector<std::pair<sdEntity*, std::shared_ptr<sdProtoMeta>>> allMetas; //!< an alias pointer to all metas
    std::vector<std::pair<sdEntity*, std::shared_ptr<sdProtoEvent>>> allEvents; //!< an alias pointer to all events
    std::set <EExtension> activatedExtensionSet; //!< a set of activated extension
    std::set <EDescriptor> validDescriptorSet; //!< a set of valid descriptor
    EOrdering ordering; //!< ordering flag
    sdInfo info; //!< contains "info" part of the meta section
    void addMetaAlias(sdEntity * const entity, const std::shared_ptr<sdProtoMeta>);
    void addEventAlias(sdEntity * const entity, const std::shared_ptr<sdProtoEvent>);
    bool removeMetaAlias(const sdEntity* const entity, const EDescriptor & descriptor);
    bool removeEventAlias(const sdEntity* const entity, const double &time, const EDescriptor & descriptor);

    void sortAllEvents();
    
public:
    
    /*! constructor with sdInfo and ordering*/
    sdScene(sdInfo info = sdInfo(), EOrdering ordering = EOrdering::SD_TIME ):info(info), ordering(ordering){
        addExtension(EExtension::SD_CORE); // validate core descriptors
        addExtension(EExtension::SD_MEDIA); // media is now part of core
    }
    
    /*!
     @}
     */

#pragma mark info
    /*! @name Meta data handing
     @{
     */
    
    /*! sets an instance of sdInfo, which contains the "info" part of the meta section 
     @param info a sdInfo
     */
    void setInfo(sdInfo info);
    
    /*! gets an instance of sdInfo, which contains the "info" part of the meta section
     @param info a sdInfo
     */
    const sdInfo &getInfo(void) const;

#pragma mark ordering

    /*! sets an enum of ordering
     @param ordering the ordering enum declared in sdConst.h
     */
    void setOrdering(const EOrdering &ordering);
    
    /*! sets an enum of ordering
     @param ordering the ordering enum declared in sdConst.h
     @returns returns true if the ordering successfully renewed
     */
    bool setOrdering(const std::string &string);
    
    /*!
     gets an enum ordering
     @returns returns enum parameter for ordering
     */
    const EOrdering &getOrdering(void) const;

    /*!
     gets ordering as String
     @returns returns ordering as std::string
     */
    std::string getOrderingAsString(void) const;
    

    /*!
     @}
     */
#pragma mark entity
    
    /*! @name Entity handling
     @{
     */
    
    /*! add a sdEntityCore to this scene. this function is the only way to generate an instance of sdEntityCore because the
     constructor of sdEntityCore is intentionally declared as a private function. Thus, the constcutor can be called only by
     its friend i.e. this class. This prevent users to create entities not attached to any sdScene.
     Thus, all sdSceneCore are always aware of the list of activated extensions.
     this function returns a pointer to the new instance of sdEntityCore.
     @param name the name of new sdEntityCore
     */
    sdEntity * const addEntity(std::string name, EKind kind = EKind::SD_SOURCE);

    
    /*! return the name of all entities in the scene as vector of string
    */
    std::vector<std::string> getEntityNames() const;

    /*! search an entity in the entity vector by its name and return the pointer. returns null if the entity can not be found.
     @param name the name of a designated entity
     */
    sdEntity *  const getEntity(const std::string &name);
    std::string getEntityName(const sdEntity * entity);
    
    /*! return the entity set */
    const std::map<std::string, sdEntity> &getEntities() const;
    
    /*! returns the number of entity in the entityVector*/
    size_t getNumberOfEntities(void) const;
    
    
    /*! remove the sdEntityCore specified by name from the entityVector]
     @param name the name of a sdEntityCore to be removed from the entityVector
     */
    bool removeEntity(const std::string &name);
    
    /*! remove all entities from the entityVector*/
    void removeAllEntities(void);
    
    /*!
     @}
     */
    
#pragma mark event
    /*! @name global event handling
     @{
     */

    /*! add event by specifying name of the target entity, time, and values.
     @param name name of target eneity
     @param time time of the newly added event
     @param values values for the event
     */
    
    template<EDescriptor D>
    inline const sdEvent<D> * const addEvent(std::string name, const double &time, const typename sdDescriptor<D>::type &values);

    /*! query timed data by specifying name of the target entity and time.
     @param name  name of target eneity
     @param time time of the event
     */
    template<EDescriptor D>
    const typename sdDescriptor<D>::type *  const getValue(std::string name, double time);
    
    /*!   collect next event(s) from all entities and report them  */
    std::vector<std::pair<const sdEntity* , std::shared_ptr<sdProtoEvent>>> getNextEventsFromAllEntities(const double &time) const;
    std::vector<std::pair<const sdEntity* , std::shared_ptr<sdProtoEvent>>> getPreviousEventsFromAllEntities(const double &time) const;
    
    /*!   collect first event(s) from all entities and report them  */
    std::vector<std::pair<const sdEntity* , std::shared_ptr<sdProtoEvent>>>  getFirstEventsFromAllEntities() const;
    std::vector<std::pair<const sdEntity* , std::shared_ptr<sdProtoEvent>>>  getLastEventsFromAllEntities() const;
    
    /*!   collect  event(s) of all entities at a specific time */
    std::vector<std::pair<const sdEntity*, std::shared_ptr<sdProtoEvent>>> getEventsFromAllEntities(const double &time) const;
    std::vector<std::pair<const sdEntity*, std::shared_ptr<sdProtoEvent>>> getEventsFromAllEntities(const double &start, const double &end) const;
    
    /*!
     asks all existing entities in the scene the time of next event and returns the soonest event from @ptime
     @param time time of index. The soonest event from this time point will be searched
     @return time of next event
     */
    std::pair<double, bool> getPreviousEventTime(const double &time);
    std::pair<double, bool> getDeltaTimeFromPreviousEvent(const double &time);
    std::pair<double, bool> getNextEventTime(const double &time);
    std::pair<double, bool> getDeltaTimeToNextEvent(const double &time);

    const std::vector<std::pair<sdEntity*, std::shared_ptr<sdProtoEvent>>> &getAllEvents();
    const std::vector<std::pair<sdEntity*, std::shared_ptr<sdProtoMeta>>> &getAllMetas();

    /*!
     @}
     */

#pragma mark extension
    /*! @name Extension handling
     @{
     */
    /*! activate an extension specified by enum EExtension. This function instantiates instances of the
     designated extension (i.e. a subclass of sdEntityExtension ) and attached them to all existing sdEntityCores
     in the scene. The added extensions will be also attached to all newly instantiated sdEntityCores.
     @param extension enum EExtension of extension to be added
     */
    bool addExtension(EExtension extension);
    bool addExtension(std::string extension);

    /*!
     check if the descriptor is vaild
     */


    /*! returns const reference of enum set */
    const std::set<EExtension> &getActivatedExtensions() const;

    /*! returns names of  activated extensions as a set*/
    std::unordered_set<std::string> getActivatedExtensionsAsStrings() const;

    /*! returns number of activated Extensions*/
    size_t getNumberOfActivatedExtensions(void) const;

    /*! check if the specified extension is activated
     @param extension enum EExtension of extension to be checked
     */
    bool isExtensionActivated(EExtension extension) const;
    bool isExtensionActivated(std::string extension) const;
    bool isDescriptorValid(const EDescriptor &descriptor) const;

    
    const std::set<EDescriptor> &getValidDescriptorSet() const;

    /*! deactivate an extension specified by enum EExtension. This function removes instances of the designated extension (i.e. a subclass of sdEntityExtension )  attached  to all existing sdEntityCores in the scene.
        Thus, all events data stored in the extension will be lost.
     @param extension enum EExtension of extension to be removed
     */
    bool removeExtension(EExtension extension);
    bool removeExtension(std::string extension);

    /*!
     remove all exntesions from the extensionVector and sdEntityCores in the scene.
     The data stored in the instances of sdEntityExtension will be lost.
     */
    
    void removeAllExtensions(void);


    /*!
     @}
     */
#pragma debug
    
    /*! @name debug
     @{
     */
    
    /**
     * @brief varifies the content of scene by sending all containing data (meta and timed) on the standard output. 
     *  It is useful for debugging the scene
     * @param consoleOut if false, does not send dump to standard output only returns a string
     * @return dump string
     */
      std::string dump(bool consoleOut = true);
    
    /*!
     @}
     */
};

#pragma mark protected functions
inline void sdScene::sortAllEvents(){
    std::sort(allEvents.begin(), allEvents.end(),
              [](std::pair<const sdEntity* , std::shared_ptr<sdProtoEvent>> eventA,
                 std::pair<const sdEntity* , std::shared_ptr<sdProtoEvent>> eventB)->bool{
                  return eventA.second->getTime() < eventB.second->getTime();
              });
}



#pragma mark Info

inline void sdScene::setInfo(sdInfo info){
    sdScene::info = info;
}

inline const sdInfo &sdScene::getInfo(void) const{
    return info;
}

#pragma mark Ordering handling

inline const EOrdering &sdScene::getOrdering(void) const{
    return ordering;
}

inline std::string sdScene::getOrderingAsString(void) const{
    return ordering == EOrdering::SD_TIME ? std::string("time") : std::string("track"); // invoke move semantics bacause of rvalue
}

inline void sdScene::setOrdering(const EOrdering &ordering ){
    sdScene::ordering = ordering;
}

inline const std::set<EDescriptor> &sdScene::getValidDescriptorSet() const{
    return validDescriptorSet;
}

inline bool sdScene::setOrdering(const std::string &ordering){
    if(ordering == "time"){
        sdScene::ordering = EOrdering::SD_TIME;
        return true;
    }else if(ordering == "track"){
        sdScene::ordering = EOrdering::SD_TRACK;
        return true;
    }
    return false;
}

#pragma mark Entity Handling

inline sdEntity * const sdScene::addEntity(std::string name, EKind kind){
    
    auto ret = entities.insert(std::pair<std::string, sdEntity>(name ,sdEntity(this, kind)));
    if(!ret.second) return nullptr;
    return &ret.first->second;
}

inline bool sdScene::removeEntity(const std::string &name){
    return entities.erase(name) != 0;
}

inline void sdScene::removeAllEntities(){
    entities.clear();
}

inline bool sdScene::isDescriptorValid(const EDescriptor &descriptor) const{
    return !(validDescriptorSet.find(descriptor) == validDescriptorSet.end());
}

inline void sdScene::addMetaAlias(sdEntity * const entity, std::shared_ptr<sdProtoMeta> event){
    removeMetaAlias(entity, event->getDescriptor());
    allMetas.push_back(std::make_pair(entity, event));
}

inline void sdScene::addEventAlias(sdEntity * const entity, std::shared_ptr<sdProtoEvent> event){
    removeEventAlias(entity, event->getTime(), event->getDescriptor());
    allEvents.push_back(std::make_pair(entity, event));
    sortAllEvents();
}

inline bool sdScene::removeMetaAlias(const sdEntity* const entity, const EDescriptor & descriptor ){
    auto it = std::find_if(allMetas.begin(), allMetas.end(), [&entity, &descriptor]( std::pair<const sdEntity * ,std::shared_ptr<sdProtoMeta>> meta){
        return (meta.second->getDescriptor() == descriptor) && (meta.first == entity);
    });
    if(it == allMetas.end()) return false;
    allMetas.erase(it);
    return true;
}

inline bool sdScene::removeEventAlias(const sdEntity* const entity, const double &time, const EDescriptor & descriptor ){
    auto it = std::find_if(allEvents.begin(), allEvents.end(), [&entity, &descriptor, &time]( std::pair<const sdEntity * ,std::shared_ptr<sdProtoEvent>> event){
        return almostEqual(event.second->getTime(), time) && (event.second->getDescriptor() == descriptor) && (event.first == entity);
    });
    if(it == allEvents.end()) return false;
    allEvents.erase(it);
    return true;
}

/*!   collect next event(s) from all entities and report them  */
inline std::vector<std::pair<const sdEntity*, std::shared_ptr<sdProtoEvent>>> sdScene::getEventsFromAllEntities(const double &time) const{
    std::vector<std::pair<const sdEntity*, std::shared_ptr<sdProtoEvent>>> matchedEvents;
    for (auto it = allEvents.begin(); it != allEvents.end(); it++) {
        if(almostEqual((*it).second->getTime(), time)){matchedEvents.push_back(*it);}
    }
    
    return std::move(matchedEvents);
}


inline std::vector<std::pair<const sdEntity*, std::shared_ptr<sdProtoEvent>>> sdScene::getEventsFromAllEntities(const double &start, const double &end) const{
    std::vector<std::pair<const sdEntity*, std::shared_ptr<sdProtoEvent>>> vector;
    for_each(allEvents.begin(), allEvents.end(), [&vector, &start, &end]( std::pair<const sdEntity*, std::shared_ptr<sdProtoEvent>> event ){
        if( start <= event.second->getTime() && event.second->getTime() <= end){ vector.push_back(event);}
    });
    return std::move(vector);
}


inline std::vector<std::pair<const sdEntity* , std::shared_ptr<sdProtoEvent>>> sdScene::getNextEventsFromAllEntities(const double &time) const{
    auto result = std::find_if(allEvents.begin(), allEvents.end(), [&time](std::pair<const sdEntity* , std::shared_ptr<sdProtoEvent>> event){
        if(event.second->getTime() > time){return true;}
        else return false;
    });
    
    if(result == allEvents.end()){return std::vector<std::pair<const sdEntity*, std::shared_ptr<sdProtoEvent>>>();}
    return getEventsFromAllEntities((*result).second->getTime());
}


inline std::pair<double, bool> sdScene::getPreviousEventTime(const double &time){
    auto set = getPreviousEventsFromAllEntities(time);
    if(set.empty()) return std::make_pair(0.0, false);
    return std::make_pair((*set.begin()).second->getTime(), true);
}

inline std::pair<double, bool> sdScene::getDeltaTimeFromPreviousEvent(const double &time){
    auto previousEventTime = getPreviousEventTime(time);
    if(!previousEventTime.second)return std::make_pair(0.0, false);
    return std::make_pair(time-previousEventTime.first, true);
}

inline std::pair<double, bool> sdScene::getNextEventTime(const double &time){
    auto set = getNextEventsFromAllEntities(time);
    if(set.empty()) return std::make_pair(0.0, false);
    return std::make_pair((*set.begin()).second->getTime(), true);
}

inline std::pair<double, bool> sdScene::getDeltaTimeToNextEvent(const double &time){
    auto nextEventTime = getNextEventTime(time);
    if(!nextEventTime.second)return std::make_pair(0.0, false);
    return std::make_pair(nextEventTime.first - time, true);
}

inline const std::vector<std::pair<sdEntity*, std::shared_ptr<sdProtoEvent>>> &sdScene::getAllEvents(){
    return allEvents;
}

inline const std::vector<std::pair<sdEntity*, std::shared_ptr<sdProtoMeta>>> &sdScene::getAllMetas(){
    return allMetas;
}

inline std::vector<std::pair<const sdEntity* , std::shared_ptr<sdProtoEvent>>> sdScene::getPreviousEventsFromAllEntities(const double &time) const{
    auto result = std::find_if(allEvents.rbegin(), allEvents.rend(), [&time](std::pair<const sdEntity* , std::shared_ptr<sdProtoEvent>> event){
        if(event.second->getTime() < time){return true;}
        else return false;
    });
    
    if(result == allEvents.rend()){return std::vector<std::pair<const sdEntity*, std::shared_ptr<sdProtoEvent>>>();}
    return getEventsFromAllEntities((*result).second->getTime());
 
}

/*!   collect first event(s) from all entities and report them  */
inline std::vector<std::pair<const sdEntity* , std::shared_ptr<sdProtoEvent>>>  sdScene::getFirstEventsFromAllEntities() const{
    if(allEvents.empty()) return std::vector<std::pair<const sdEntity* , std::shared_ptr<sdProtoEvent>>> ();
    return getEventsFromAllEntities(allEvents.front().second->getTime());
}

inline std::vector<std::pair<const sdEntity* , std::shared_ptr<sdProtoEvent>>> sdScene::getLastEventsFromAllEntities() const{
    if(allEvents.empty()) return std::vector<std::pair<const sdEntity* , std::shared_ptr<sdProtoEvent>>> ();
    return getEventsFromAllEntities(allEvents.back().second->getTime());
}

inline std::string sdScene::getEntityName(const sdEntity* entity){
    for(auto it = entities.begin(); it != entities.end(); it++) {
        if(&((*it).second) == entity){
            return (*it).first;
        }
    }
    return std::string();
}

inline const std::map<std::string, sdEntity> &sdScene::getEntities() const{
    return entities;
}

inline std::vector<std::string> sdScene::getEntityNames() const{
    std::vector<std::string> returnVector;
    for_each(entities.begin(), entities.end(),[&returnVector](std::pair<std::string, sdEntity> pair){
        returnVector.push_back(pair.first);
    });
    return std::move(returnVector);
}

inline sdEntity * const sdScene::getEntity(const std::string &name){
    std::map<std::string, sdEntity>::iterator it = entities.find(name);
    if(it == entities.end()){ return nullptr; }
    return &((*it).second);
}



inline size_t sdScene::getNumberOfEntities() const{
    return entities.size();
}

#pragma global event handling

template<EDescriptor D>
inline const sdEvent<D> * const sdScene::addEvent(std::string name, const double &time, const typename sdDescriptor<D>::type &values){
    auto entity = getEntity(name);
    if(!entity) return nullptr;
    return entity->addEvent<D>(time, values);
}

template<EDescriptor D>
inline const typename sdDescriptor<D>::type * const sdScene::getValue(std::string name, double time){
    auto entity = getEntity(name);
    if(!entity) return nullptr;
    return entity->getValue<D>(time);
}



#pragma mark extension

inline bool sdScene::addExtension(EExtension extension){
    auto ret =  activatedExtensionSet.insert(extension).second;
    if(ret){
        auto descriptors = sdExtension::getDescriptorsForExtension(extension);
        for(auto it = descriptors.begin(); descriptors.end() != it; it++) {
            validDescriptorSet.insert((*it).descriptor);
        }
    }
    return ret;
}

inline bool sdScene::addExtension(std::string extension){
    auto ext = sdExtension::stringToExtension(extension);
    if(ext == EExtension::SD_EXTENSION_ERROR){
        std::cerr << extension << " no such extension" << std::endl;
        return false;
    }
    return addExtension(ext);
}


inline std::unordered_set<std::string> sdScene::getActivatedExtensionsAsStrings() const{
    std::unordered_set<std::string> set;
    for(auto it = activatedExtensionSet.begin(); it != activatedExtensionSet.end(); it++){
        set.insert(sdExtension::extensionToString(*it));
    }
    return std::move(set);
}

inline size_t sdScene::getNumberOfActivatedExtensions() const{
    return activatedExtensionSet.size() - 2; // because core is not a extension;
}

inline bool sdScene::isExtensionActivated(EExtension extension) const{
    return activatedExtensionSet.find(extension) != activatedExtensionSet.end();
}

inline bool sdScene::isExtensionActivated(std::string extension) const{
    auto ext = sdExtension::stringToExtension(extension);
    if(ext == EExtension::SD_EXTENSION_ERROR) return false;
    return isExtensionActivated(ext);
}

inline bool sdScene::removeExtension(EExtension extension){
    if (extension == EExtension::SD_CORE) return false;
    auto descriptors = sdExtension::getDescriptorsForExtension(extension);
    for(auto it = descriptors.begin(); it != descriptors.end(); it++) {
        validDescriptorSet.erase((*it).descriptor);
    }
    return activatedExtensionSet.erase(extension);
}

inline bool sdScene::removeExtension(std::string extension){
    auto ext = sdExtension::stringToExtension(extension);
    if(ext == EExtension::SD_EXTENSION_ERROR) return false;
    return removeExtension(ext);
}

inline void sdScene::removeAllExtensions(){
    activatedExtensionSet.clear();
    validDescriptorSet.clear();
    addExtension(EExtension::SD_CORE);
}


inline std::string sdScene::dump(bool consoleOut){
    
    return std::string();
}

#endif