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

#include "sdConst.h"
#include "sdInfo.h"
#include "sdEntity.h"

//#include "sdTrajectoryHandler.h"
/*!
    a structure for associating an entity and multiple events
*/
//class sdReport {
//public:
//    sdEntityCore* entity;
//    std::multiset<sdEvent*, sdEventCompare> eventSet;
//};


/*! is responsible for 
 - adding, removing, and maintaining sdEntityCores.
 - attaching and detaching extensions to the exisiting and newly instantiated cores
 */
class sdScene{
    friend sdEntity;
    
private:
    std::map <std::string, sdEntity> entities; //!< a map of sdEntities
    std::vector<std::shared_ptr<sdProtoEvent>> allEvents; //!< an alias pointer to all events
    std::unordered_set<int> activatedExtensionSet; //!< a vector of activated
    EOrdering ordering; //!< ordering flag
    sdInfo info; //!< contains "info" part of the meta section

public:
    
    /*! @name Constructors
     @{
     */
    
    /*! constructor */
    sdScene(void);
    
    /*! constructor with sdInfo  */
    sdScene(sdInfo info);
    
    /*! constructor with sdInfo and ordering*/
    sdScene(sdInfo info, EOrdering ordering);
    
    /*!
     @}
     */

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
    sdInfo getInfo(void);

    /*!
     gets an enum  ordering
     */
    EOrdering getOrdering(void);

    /*!
     gets ordering as String
     */
    std::string getOrderingAsString(void);
    
    /*! sets an enum of ordering
     @param ordering the ordering enum declared in sdConst.h
     */
    void setOrdering(EOrdering ordering);
    
    /*! sets an enum of ordering
     @param ordering the ordering enum declared in sdConst.h
     */
    void setOrdering(std::string string);

    /*!
     @}
     */
    
//    /*! @name Entity handling
//     @{
//     */
//    
//    /*! return name of the entity at the specified index
//     @param index
//     */
//    std::string getEntityName(int index);
//
    /*! return the name of all entities in the scene as vector of string
    */
    std::vector<std::string> getEntityNames();

    /*! search an entity in the entity vector by its name and return the pointer. returns null if the entity can not be found.
     @param name the name of a designated entity
     */
//    sdEntityCore* getEntity(std::string name);
    
//    /*! returns a pointer to the entityVector */
//    std::vector <sdEntityCore*> getEntityVector(void);
//    
    /*! returns the number of entity in the entityVector*/
    size_t getNumberOfEntities(void);
    
    /*! add a sdEntityCore to this scene. this function is the only way to generate an instance of sdEntityCore because the
     constructor of sdEntityCore is intentionally declared as a private function. Thus, the constcutor can be called only by
      its friend i.e. this class. This prevent users to create entities not attached to any sdScene. 
      Thus, all sdSceneCore are always aware of the list of activated extensions.
      this function returns a pointer to the new instance of sdEntityCore.
        @param name the name of new sdEntityCore
     */
    sdEntity * const addEntity(std::string name, EKind kind = SD_SOURCE);
    
    /*! remove the sdEntityCore specified by name from the entityVector]
     @param name the name of a sdEntityCore to be removed from the entityVector
     */
    void removeEntity(std::string name);
    
    /*! remove all entities from the entityVector*/
    void removeAllEntities(void);
    
    /*!
     @}
     */
    
//    /*! @name Value handling
//     @{
//     */
//    
//    /*! set timed data by specifying name of the target entity, descriptor, and time.
//     sdScene forwards the request to a proper sdEntityCore. 
//     a properly allocated void pointer should be provided as the fourth argument
//     @param name name of target eneity
//     @param time time of the newly added event
//     @param descriptor enum of target descriptor
//     @param value allocated void pointer containing value(s)
//     */
//    void setValue(std::string name, double time, EDescriptor descriptor,  void* value);
//    
//    /*! query timed data by specifying name of the target entity, descriptor, and time.
//     sdScene forwards the query to a proper sdEntityCore and return the answer.
//     a properly allocated void pointer should be provided as the fourth argument
//     @param name  name of target eneity
//     @param descriptor enum of target descriptor
//     @param value allocated void pointer containing value(s)
//     */
//    void* getValue(std::string name, double time, EDescriptor descriptor);
//    
//    /*!
//     @}
//     */
//    
//    /*! @name Extension handling
//     @{
//     */
//    
//    /*! return an enum EExtension at the specified index */
//    EExtension getActivatedExtension(int index);
//    
//    /*! returns number of activated Extensions*/
//    int getNumberOfActivatedExtensions(void);
//     
//    /*! returns enum EExtension as an Vector */
//    std::vector<EExtension> getActivatedExtensions();
//
//    /*! returns names of  activated extensions as a vector*/
//    std::vector<std::string> getActivatedExtensionsAsStrings();
//
//    /*! activate an extension specified by enum EExtension. This function instantiates instances of the
//     designated extension (i.e. a subclass of sdEntityExtension ) and attached them to all existing sdEntityCores
//    in the scene. The added extensions will be also attached to all newly instantiated sdEntityCores.
//     @param extension enum EExtension of extension to be added
//     */
//    void addExtension(EExtension extension);
//    
//    /*! activate an extension specified by a string. 
//     @param extension string of extension to be added
//     */
//    void addExtension(std::string extension);
//
//    /*! check if the specified extension is activated
//     @param extension enum EExtension of extension to be checked
//     */
//    bool isExtensionActivated(EExtension extension);
//    bool isExtensionActivated(std::string extensionString);
//
//    /*! deactivate an extension specified by enum EExtension. This function removes instances of the designated extension (i.e. a subclass of sdEntityExtension )  attached  to all existing sdEntityCores in the scene.
//        Thus, all events data stored in the extension will be lost.
//     @param extension enum EExtension of extension to be removed
//     */
//    void removeExtension(EExtension extension);
//    
//    /*! deactivate an extension specified by a string
//     @param extension string of extension to be removed
//     */
//    void removeExtension(std::string extension);
//
//    /*! 
//     remove all exntesions from the extensionVector and sdEntityCores in the scene.
//     The data stored in the instances of sdEntityExtension will be lost.
//     */
//    
//    void removeAllExtensions(void);
//
//    /*!
//     @}
//     */
//    
//    /*! @name Utility function
//     @{
//     */
//    
//    /*!   collect next event(s) from all entities and report them  */
//    std::vector<sdReport> getNextEventSetsFromAllEntities(double time);
//    std::vector<sdReport> getPreviousEventSetsFromAllEntities(double time);
//
//    /*!   collect first event(s) from all entities and report them  */
//    std::vector<sdReport> getFirstEventSetsFromAllEntities();
//    std::vector<sdReport> getLastEventSetsFromAllEntities();
//
//    /*!   collect  event(s) from all entities in range and report them */
//    std::vector<sdReport> getEventSetsFromAllEntities(double time);
//    std::vector<sdReport> getEventSetsFromAllEntities(double start, double end);
//
//    /*!
//     collect first event(s) from all entities, compare the time tag, and return the very first event(s).
//     */
//    std::vector<sdReport> getFirstEventSets();
//    
//    /*!
//     collect next events from all entities, compare the time tag, and return the soonest events
//     */
//    std::vector<sdReport> getNextEventSets(double time);
//
//    
//    /*!
//     asks all existing entities in the scene the time of next event and returns the soonest event from @ptime
//	 @param time time of index. The soonest event from this time point will be searched
//     @return time of next event
//     */
//    double getNextEventTime(double time);
//    double getDeltaTimeToNextEvent(double time);
//
//    /*!
//     @}
//     */
//    
//    
//    /*! @name Varification
//     @{
//     */
//    
//    /**
//     * @brief varifies the content of scene by sending all containing data (meta and timed) on the standard output. 
//     *  It is useful for debugging the scene
//     * @param consoleOut if false, does not send dump to standard output only returns a string
//     * @return dump string
//     */
//      std::string dump(bool consoleOut = true);
//    
//    /*!
//     @}
//     */
};

#pragma inline implementations

inline sdScene::sdScene(void){
    sdInfo info("unknown", "unknown", sdDate(), "unknown", "unknown", "unknown");
    ordering = SD_TIME;
}

inline sdScene::sdScene(sdInfo info){
    setInfo(info);
    ordering = SD_TIME;
}

inline sdScene::sdScene(sdInfo info, EOrdering ordering){
    setInfo(info);
    sdScene::ordering = ordering;
}

inline void sdScene::setInfo(sdInfo info){
    sdScene::info = info;
}

inline sdInfo sdScene::getInfo(void){
    return info;
}

inline EOrdering sdScene::getOrdering(void){
    return ordering;
}

inline std::string sdScene::getOrderingAsString(void){
    return ordering == SD_TIME ? std::string("time") : std::string("track");
}

inline void sdScene::setOrdering(EOrdering ordering ){
    sdScene::ordering = ordering;
}

inline void sdScene::setOrdering(std::string ordering){
    if(ordering == "time"){
        sdScene::ordering = SD_TIME;
    }else if(ordering == "track"){
        sdScene::ordering = SD_TRACK;
    }else{
        std::cout << "sdScene Error: Unknown Ordering" << std::endl;
    }
}

sdEntity * const sdScene::addEntity(std::string name, EKind kind){
    auto ret = entities.insert(std::pair<std::string, sdEntity>(name ,sdEntity(this)));
    if(!ret.second) return nullptr;
    return &ret.first->second;
}

size_t sdScene::getNumberOfEntities(){
    return entities.size();
}
#endif