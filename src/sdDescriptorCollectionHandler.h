//
//  sdDescriptorCollectionHandler.h
//  libSpatDIFTest
//
//  Created by Chikashi Miyama on 30/08/16.
//  Copyright © 2016 ICST. All rights reserved.
//

#pragma once

class sdDescriptorCollectionHandler{
public:
    using sdDataSetCollection = std::unordered_map<std::string, std::shared_ptr<sdProtoDataSet>>;
    
    template<EExtension Extension>
    void addDescriptorSet(EExtension extension, std::string identifier, sdDataSet<Extension> set){
        auto targetCollection = descriptorSetCollections.find(extension);
        if(targetCollection == descriptorSetCollections.end()) throw InvalidExtensionException("extension not activated");
        std::shared_ptr<sdProtoDataSet> setPtr(new sdProtoDataSet(set));
        addDescriptorSet(extension, identifier, setPtr);
    }
    
    void addDescriptorSet(EExtension extension, std::string identifier, std::shared_ptr<sdProtoDataSet> set){
        auto targetCollection = descriptorSetCollections.find(extension);
        if(targetCollection == descriptorSetCollections.end()) throw InvalidExtensionException("extension not activated");
        (*targetCollection).second.emplace(identifier, set);
    }
    
    void removeDescriptorSet(EExtension extension, std::string identifier){
        auto targetCollection = descriptorSetCollections.find(extension);
        if(targetCollection == descriptorSetCollections.end()) throw InvalidExtensionException("extension not activated");
        (*targetCollection).second.erase(identifier);
    }
    
    std::shared_ptr<sdProtoDataSet> getProtoDescriptorSet(EExtension extension, std::string identifier){
        auto targetCollection = descriptorSetCollections.find(extension);
        if(targetCollection == descriptorSetCollections.end()) throw InvalidExtensionException("extension not activated");
        return (*targetCollection).second.at(identifier);
    }
    
protected:
    void addDescriptorSetCollection(EExtension extension){
        descriptorSetCollections.emplace(extension, sdDataSetCollection());
    }
    
    void removeDescriptorSetCollection(EExtension extension){
        descriptorSetCollections.erase(extension);
    }
    
    void copy(const sdDescriptorCollectionHandler &origin){
        descriptorSetCollections = origin.descriptorSetCollections;
    }
    
    std::unordered_map<EExtension, sdDataSetCollection> descriptorSetCollections;
    
};

