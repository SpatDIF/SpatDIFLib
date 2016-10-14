//
//  sdGlobalMetaHandler.h
//  libSpatDIFTest
//
//  Created by Chikashi Miyama on 23/09/16.
//  Copyright © 2016 ICST. All rights reserved.
//

#pragma once

class sdEntity;
class sdGlobalMetaHandler{
public:
    bool removeMetaAlias(const sdEntity* const entity, const EDescriptor & descriptor );
    const std::vector<std::pair<sdEntity*, std::shared_ptr<sdProtoMeta>>> &getAllMetas() const;
    void addMetaAlias(sdEntity * const entity, std::shared_ptr<sdProtoMeta> event);
    
protected:
    std::vector<std::pair<sdEntity*, std::shared_ptr<sdProtoMeta>>> allMetas; //!< an alias pointer to all metas
};

inline bool sdGlobalMetaHandler::removeMetaAlias(const sdEntity* const entity, const EDescriptor & descriptor ){
    auto it = std::find_if(allMetas.begin(), allMetas.end(), [&entity, &descriptor]( std::pair<const sdEntity * ,std::shared_ptr<sdProtoMeta>> meta){
        return (meta.second->getDescriptor() == descriptor) && (meta.first == entity);
    });
    if(it == allMetas.end()) return false;
    allMetas.erase(it);
    return true;
}

inline const std::vector<std::pair<sdEntity*, std::shared_ptr<sdProtoMeta>>> &sdGlobalMetaHandler::getAllMetas() const{
    return allMetas;
}

inline void sdGlobalMetaHandler::addMetaAlias(sdEntity * const entity, std::shared_ptr<sdProtoMeta> event){
    removeMetaAlias(entity, event->getDescriptor());
    allMetas.push_back(std::make_pair(entity, event));
}

