//
//  sdDescriptorSetHandler.h
//  libSpatDIFTest
//
//  Created by Chikashi Miyama on 26/08/16.
//  Copyright © 2016 ICST. All rights reserved.
//

#pragma once
#include <unordered_map>
#include "sdDescriptors.h"
#include "sdExtension.h"

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
    using sdDescriptorSet = std::unordered_map<std::string, std::shared_ptr<sdProtoDescriptorSet>> ;
    
    
protected:
    /*!
     add unordered map for specified extension. called when extension is activated.
     */
    void addDescriptorSetCollectionForExtension(const EExtension &extension){
        if(descriptorSetCollections.find(extension) != descriptorSetCollections.end()) return;
        descriptorSetCollections.insert({extension, sdDescriptorSet()});
    }
    
    /*!
     remove unordered map of the specified extension. called when extension is deactivated.
     */
    void removeDescriptorSetCollectionForExtension(const EExtension &extension){
        descriptorSetCollections.erase(extension);
    }

    /*!
     add descriptor set for specifid map.
     */
    void addDescriptorSet(const EExtension &extension, std::pair<std::string, std::shared_ptr<sdProtoDescriptorSet>> &descriptorSet){
        auto itr = descriptorSetCollections.find(extension);
        if(itr == descriptorSetCollections.end()){
            std::string name = sdExtension::extensionToString(extension);
            std::string error = std::string("Extension ")  + name + "not activated";
            throw InvalidExtensionException(error);
        }
        (*itr).second.insert(descriptorSet);
    }
    
    /*!
     remove descriptor set from speicified collection
     */
    void removeDescriptorSet(const EExtension &extension, std::string key){
        auto itr = descriptorSetCollections.find(extension);
        if(itr == descriptorSetCollections.end()){
            std::string name = sdExtension::extensionToString(extension);
            std::string error = std::string("Extension ")  + name + "not activated";
            throw InvalidExtensionException(error);
        }
        (*itr).second.erase(key);
    }
    
private:
    std::unordered_map<EExtension, sdDescriptorSet> descriptorSetCollections; // only accessible via member functions by the subclass

};


