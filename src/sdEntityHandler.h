//
//  sdEntityHandler.h
//  libSpatDIFTest
//
//  Created by Chikashi Miyama on 23/09/16.
//  Copyright © 2016 ICST. All rights reserved.
//


#pragma once
#include "sdEntity.h"
#include "sdExtensionHandler.h"

class sdEntityHandler {
    
public:
    virtual sdEntity * const addEntity(std::string name, EKind kind) = 0;

    /*! return the name of all entities in the scene as vector of string
     */
    std::unordered_set<std::string> getEntityNames() const noexcept;

    /*! search an entity in the entity vector by its name and return the pointer. returns null if the entity can not be found.
     @param name the name of a designated entity
     */
    sdEntity *  const getEntity(const std::string &name);
    std::string getEntityName(const sdProtoEntity * entity);

    /*! return the entity set */
    const std::unordered_map<std::string, sdEntity> &getEntities() const;

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
    
protected:
    std::unordered_map <std::string, sdEntity> entities; //!< a map of sdEntities

};

inline bool sdEntityHandler::removeEntity(const std::string &name){
    return entities.erase(name) != 0;
}

inline void sdEntityHandler::removeAllEntities(){
    entities.clear();
}

inline std::string sdEntityHandler::getEntityName(const sdProtoEntity* entity){
    for(auto it = entities.begin(); it != entities.end(); it++) {
        if(&((*it).second) == entity){
            return (*it).first;
        }
    }
    return std::string();
}

inline const std::unordered_map<std::string, sdEntity> &sdEntityHandler::getEntities() const{
    return entities;
}

inline std::unordered_set<std::string> sdEntityHandler::getEntityNames() const noexcept{
    std::unordered_set<std::string> returnSet;
    for(auto &entity:entities){ returnSet.emplace(entity.first);}
    return std::move(returnSet);
}

inline sdEntity * const sdEntityHandler::getEntity(const std::string &name){
    std::unordered_map<std::string, sdEntity>::iterator it = entities.find(name);
    if(it == entities.end()){ return nullptr; }
    return &((*it).second);
}

inline size_t sdEntityHandler::getNumberOfEntities() const{
    return entities.size();
}
