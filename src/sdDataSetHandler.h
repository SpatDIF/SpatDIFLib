//
//  sdDataSetHandler.h
//  libSpatDIFTest
//
//  Created by Chikashi Miyama on 30/08/16.
//  Copyright © 2016 ICST. All rights reserved.
//

#pragma once

class sdDataSetHandler{
public:
    using sdDataSetCollection = std::unordered_map<std::string, std::shared_ptr<sdProtoDataSet>>;
    
    template<EExtension Extension>
    void addDataSet(EExtension extension, std::string identifier, sdDataSet<Extension> set){
        auto targetCollection = descriptorSetCollections.find(extension);
        if(targetCollection == descriptorSetCollections.end()) throw InvalidExtensionException("extension not activated");
        std::shared_ptr<sdProtoDataSet> setPtr(new sdProtoDataSet(set));
        addDataSet(extension, identifier, setPtr);
    }
    
    void addDataSet(EExtension extension, std::string identifier, std::shared_ptr<sdProtoDataSet> set){
        auto targetCollection = descriptorSetCollections.find(extension);
        if(targetCollection == descriptorSetCollections.end()) throw InvalidExtensionException("extension not activated");
        (*targetCollection).second.emplace(identifier, set);
    }
    
    void removeDataSet(EExtension extension, std::string identifier){
        auto targetCollection = descriptorSetCollections.find(extension);
        if(targetCollection == descriptorSetCollections.end()) throw InvalidExtensionException("extension not activated");
        (*targetCollection).second.erase(identifier);
    }
    
    std::shared_ptr<sdProtoDataSet> getProtoDataSet(EExtension extension, std::string identifier) {
        auto targetCollection = descriptorSetCollections.find(extension);
        if(targetCollection == descriptorSetCollections.end()) throw InvalidExtensionException("extension not activated");
        auto dataset = (*targetCollection).second;
        return dataset.at(identifier);
    }
    
    void setData(std::string identifier, EDescriptor descriptor, std::string valueString){
        sdSpec::getAddDataFunc(descriptor)(this, identifier, valueString);
    }
    
protected:
    void addDataSetCollection(EExtension extension){
        descriptorSetCollections.emplace(extension, sdDataSetCollection());
    }
    
    void removeDataSetCollection(EExtension extension){
        descriptorSetCollections.erase(extension);
    }
    
    void copy(const sdDataSetHandler &origin){
        descriptorSetCollections = origin.descriptorSetCollections;
    }
    
    std::unordered_map<EExtension, sdDataSetCollection> descriptorSetCollections;
    
};

