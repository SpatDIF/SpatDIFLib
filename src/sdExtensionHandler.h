//
//  sdExtensionHandler.h
//  libSpatDIFTest
//
//  Created by Chikashi Miyama on 23/09/16.
//  Copyright © 2016 ICST. All rights reserved.
//

#pragma once

#include "sdDataSetHandler.h"

class sdExtensionHandler : public sdDataSetHandler{
public:

    
    bool addExtension(EExtension extension){
        auto ret =  activatedExtensionSet.insert(extension).second;
        if(ret){
            auto descriptors = sdSpec::getDescriptorsOfExtension(extension);
            for(auto it = descriptors.begin(); descriptors.end() != it; it++) {
                validDescriptorSet.insert((*it).descriptor);
            }
            addDataSetCollection(extension);
        }
        return ret;
    }

    bool addExtension(std::string extension){
        auto ext = sdSpec::stringToExtension(extension);
        if(ext == EExtension::SD_EXTENSION_ERROR){
            std::cerr << extension << " no such extension" << std::endl;
            return false;
        }
        return addExtension(ext);
    }
    
    void addInitialDescriptors() {
        for(auto &item : sdSpec::getCoreSpec()){
            addExtension(item);
        }
    }
    
    std::unordered_set<std::string> getActivatedExtensionsAsStrings() const{
        std::unordered_set<std::string> set;
        for(auto it = activatedExtensionSet.begin(); it != activatedExtensionSet.end(); it++){
            set.insert(sdSpec::extensionToString(*it));
        }
        return std::move(set);
    }
    
    size_t getNumberOfActivatedExtensions() const{
        size_t size = activatedExtensionSet.size();
        size -= sdSpec::getNumberOfCoreSpecs();
        return size ; // because core is not an extension;
    }

    bool isExtensionActivated(EExtension extension) const{
        return activatedExtensionSet.find(extension) != activatedExtensionSet.end();
    }

    bool isExtensionActivated(std::string extension) const{
        auto ext = sdSpec::stringToExtension(extension);
        if(ext == EExtension::SD_EXTENSION_ERROR) return false;
        return isExtensionActivated(ext);
    }

    bool removeExtension(EExtension extension){
        if (extension == EExtension::SD_CORE) return false;
        auto descriptors = sdSpec::getDescriptorsOfExtension(extension);
        for(auto it = descriptors.begin(); it != descriptors.end(); it++) {
            validDescriptorSet.erase((*it).descriptor);
        }
        return activatedExtensionSet.erase(extension);
    }

    bool removeExtension(std::string extension){
        auto ext = sdSpec::stringToExtension(extension);
        if(ext == EExtension::SD_EXTENSION_ERROR) return false;
        return removeExtension(ext);
    }

    void removeAllExtensions() noexcept{
        activatedExtensionSet.clear();
        addInitialDescriptors();
    }
    
    size_t getNumberOfValidDescriptors() const{
        return validDescriptorSet.size();
    }
    
    bool isDescriptorValid(const EDescriptor &descriptor) const{
        return !(validDescriptorSet.find(descriptor) == validDescriptorSet.end());
    }
protected:
    std::set <EExtension> activatedExtensionSet; //!< a set of activated extension
    std::set <EDescriptor> validDescriptorSet; //!< a set of valid descriptor
};
