//
//  sdGlobalMetaHandler.h
//  libSpatDIFTest
//
//  Created by Chikashi Miyama on 23/09/16.
//  Copyright © 2016 ICST. All rights reserved.
//

#pragma once

class sdProtoEntity;
class sdGlobalMetaHandler{
public:
    bool removeMetaAlias(const sdProtoEntity* const entity, const EDescriptor & descriptor );
    std::vector<std::pair<sdProtoEntity*, std::shared_ptr<sdProtoMeta>>> &getAllMetas();
    void addMetaAlias(sdProtoEntity * const entity, std::shared_ptr<sdProtoMeta> event);
    
protected:
    std::vector<std::pair<sdProtoEntity*, std::shared_ptr<sdProtoMeta>>> allMetas; //!< an alias pointer to all metas
};

inline bool sdGlobalMetaHandler::removeMetaAlias(const sdProtoEntity* const entity, const EDescriptor & descriptor ){
    auto it = std::find_if(allMetas.begin(), allMetas.end(), [&entity, &descriptor]( std::pair<const sdProtoEntity * ,std::shared_ptr<sdProtoMeta>> meta){
        return (meta.second->getDescriptor() == descriptor) && (meta.first == entity);
    });
    if(it == allMetas.end()) return false;
    allMetas.erase(it);
    return true;
}

inline std::vector<std::pair<sdProtoEntity*, std::shared_ptr<sdProtoMeta>>> &sdGlobalMetaHandler::getAllMetas(){
    return allMetas;
}

inline void sdGlobalMetaHandler::addMetaAlias(sdProtoEntity * const entity, std::shared_ptr<sdProtoMeta> event){
    removeMetaAlias(entity, event->getDescriptor());
    allMetas.push_back(std::make_pair(entity, event));
}

