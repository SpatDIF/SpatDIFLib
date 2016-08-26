//
//  sdDescriptorSetHandler.h
//  libSpatDIFTest
//
//  Created by Chikashi Miyama on 26/08/16.
//  Copyright © 2016 ICST. All rights reserved.
//

#pragma once
#include <map>
#include "sdDescriptors.h"

namespace std
{
    
    // explicit instantiation for EExtension, cast to size_t
    template<>
    struct hash<EExtension> {
        size_t operator()(const EExtension &ext) const {
            return static_cast<size_t>(ext);
        }
    };
}
/*!
 this class handles sets of pairs of descriptors and value independent from entities.
 a new  unordered_map is dynamically added to map when an extension is activated.
 */

class sdDescriptorSetHandler{
    using sdDescriptorMap = std::unordered_map<std::string, std::shared_ptr<sdProtoDescriptorSet>> ;
    
    
protected:
    /*!
     add unordered map for specified extension. called when extension is activated.
     */
    void addDescriptorSetForExtension(const EExtension &extension){
        if(descriptorMaps.find(extension) != descriptorMaps.end()) return;
        descriptorMaps.insert({extension, sdDescriptorMap()});
    }
    
    /*!
     remove unordered map of the specified extension. called when extension is deactivated.
     */
    void removeDescriptorSetForExtension(const EExtension &extension){
        descriptorMaps.erase(extension);
    }

private:
    std::unordered_map<EExtension, sdDescriptorMap> descriptorMaps;

};


