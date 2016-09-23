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

#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include "sdDescriptor.h"
#include "sdEntityHandler.h"
#include "sdExtensionHandler.h"
#include "sdOrderingHandler.h"
#include "sdGlobalMetaHandler.h"
#include "sdGlobalEventHandler.h"

/*! is responsible for 
 - adding, removing, and maintaining sdEntityCores.
 - attaching and detaching extensions to the exisiting and newly instantiated cores
 */
class sdScene : public sdGlobalEventHandler, public sdGlobalMetaHandler, public sdEntityHandler, public sdExtensionHandler, public sdOrderingHandler{
    friend sdEntity;
    friend sdProtoEntity;
    
private:
    void copy(const sdScene& origin){
        entities = origin.entities;
        allMetas = origin.allMetas;
        allEvents = origin.allEvents;
        activatedExtensionSet = origin.activatedExtensionSet;
        validDescriptorSet = origin.validDescriptorSet;
        setOrdering(origin.ordering);
        sdDataSetHandler::copy(origin);
        
        for(auto &entity:entities){ // parent is now me
            entity.second.parent = this;
        }
        // point new entities
        for(auto &meta: allMetas){
            std::string name = meta.first->getName();
            meta.first = getEntity(name);
        }
        for(auto &event: allEvents){
            std::string name = event.first->getName();
            event.first = getEntity(name);
        }
    }
public:
    
    /*! constructor with sdInfo and ordering*/
    sdScene(sdDataSet<EExtension::SD_INFO> info = sdDataSet<EExtension::SD_INFO>(), EOrdering ordering = EOrdering::SD_TIME ):sdOrderingHandler(ordering){
        addInitialDescriptors();
        addDataSet(EExtension::SD_INFO, info);
    }
    
    sdScene (const sdScene& origin){
        copy(origin);
    }
    sdScene operator=(const sdScene& origin){
        copy(origin);
        return (*this);
    }
    sdScene(sdScene&& origin){
        copy(origin);
    }
    sdScene& operator=(const sdScene&& origin){
        copy(origin);
        return (*this);
    }

#pragma mark entity handler override
    sdEntity * const addEntity(std::string name, EKind kind = EKind::SD_SOURCE) override;
    
    template<EDescriptor D>
    const sdEvent<D> * const addEvent(std::string name, const double &time, const typename sdDescriptor<D>::type &values);
    
    template<EDescriptor D>
    const typename sdDescriptor<D>::type * const getValue(std::string name, double time);

    
    std::string dump(bool consoleOut = true);
};


inline sdEntity * const sdScene::addEntity(std::string name, EKind kind){
    auto ret = entities.insert(std::pair<std::string, sdEntity>(name ,sdEntity(this, kind)));
    if(!ret.second) return nullptr;
    return &ret.first->second;
}


template<EDescriptor D>
inline const sdEvent<D> * const sdScene::addEvent(std::string name, const double &time, const typename sdDescriptor<D>::type &values){
    auto entity = getEntity(name);
    if(!entity) return nullptr;
    return entity->addEvent<D>(time, values);
}

template<EDescriptor D>
inline const typename sdDescriptor<D>::type * const sdScene::getValue(std::string name, double time) {
    auto entity = getEntity(name);
    if(!entity) return nullptr;
    return entity->getValue<D>(time);
}


inline std::string sdScene::dump(bool consoleOut){
    return std::string();
}


