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
    
    template<EExtension E>
    sdDataSet<E> getDataSet(std::string identifier){
        std::shared_ptr<sdProtoDataSet>  protoDataSet = getProtoDataSet(E, identifier);
        return *(static_cast<sdDataSet<E> *>(protoDataSet.get()));
    }
    
    template<EExtension E>
    void addDataSet(EExtension extension, sdDataSet<E> set){
        auto targetCollection = descriptorSetCollections.find(extension);
        if(targetCollection == descriptorSetCollections.end()) throw InvalidExtensionException("extension not activated");
        std::shared_ptr<sdProtoDataSet> setPtr(new sdProtoDataSet(set));
        addDataSet(extension, setPtr);
    }
    
    void addDataSet(EExtension extension, std::shared_ptr<sdProtoDataSet> set){
        auto it = descriptorSetCollections.find(extension);
        if(it == descriptorSetCollections.end()) throw InvalidExtensionException("extension not activated");
        EDescriptor idDescriptor = sdSpec::getIDDescriptorOfExtension(extension);
        std::string identifier = (extension == EExtension::SD_INFO) ? "info": set->getValueAsString(idDescriptor);
        if(identifier.empty())throw InvalidDatasetIDException();
        it->second.erase(identifier);
        it->second.emplace(identifier, set);
    }
    
    void removeDataSet(EExtension extension, std::string identifier){
        auto targetCollection = descriptorSetCollections.find(extension);
        if(targetCollection == descriptorSetCollections.end()) throw InvalidExtensionException("extension not activated");
        (*targetCollection).second.erase(identifier);
    }
    
    std::shared_ptr<sdProtoDataSet> getProtoDataSet(EExtension extension, std::string identifier) const{
        auto targetCollection = descriptorSetCollections.find(extension);
        if(targetCollection == descriptorSetCollections.end()) throw InvalidExtensionException("extension not activated");
        auto dataset = (*targetCollection).second;
        return dataset.at(identifier);
    }
    
    void setData(std::string identifier, EDescriptor descriptor, std::string valueString){
        auto addDataFunc = sdSpec::getAddDataFunc(descriptor);
        addDataFunc(this, identifier, valueString);
    }
    
    std::unordered_set<std::string> getIdentifiers(const EExtension &extension) const{
        auto collection = descriptorSetCollections.find(extension);
        std::unordered_set<std::string> identifierSet;

        if(collection == descriptorSetCollections.end()){return identifierSet;}
        for(auto &item : collection->second){
            identifierSet.emplace(item.first);
        }
        return identifierSet;
    }
    
    void setInfo(sdInfo &info){
        addDataSet(EExtension::SD_INFO, info);
    }
    
    const sdInfo &getInfo(void) const{
        return *(static_cast<sdDataSet<EExtension::SD_INFO>*>(getProtoDataSet(EExtension::SD_INFO, "info").get()));
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

