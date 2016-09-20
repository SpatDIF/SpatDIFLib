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

/*!
 This class is responsible for answering questions about specifications defined in sdDescriptor.h and .cpp from other classes.

 */

class sdDataSetHandler;
class sdProtoEntity;
class sdProtoEvent;
class sdProtoMeta;
class sdProtoHolder;
class sdSpec {
public:
    
    // a holder of descriptor in enum, string, utility func to make pair, meta, event
    struct sdDescriptorSpec{
        sdDescriptorSpec(EDescriptor descriptor,
                std::string descriptorString,
                std::function< void(sdDataSetHandler*, std::string, std::string)> addDataFromStringFunc,
                std::function< std::shared_ptr<sdProtoMeta>(sdProtoEntity*, std::string)> addMetaFromStringFunc,
                std::function< std::shared_ptr<sdProtoEvent>(sdProtoEntity*, double, std::string)> addEventFromStringFunc,
                std::function< std::string(std::shared_ptr<sdProtoHolder>)> getDataAsStringFunc ):
        descriptor(descriptor),
        descriptorString(descriptorString),
        addDataFromStringFunc(addDataFromStringFunc),
        addEventFromStringFunc(addEventFromStringFunc),
        addMetaFromStringFunc(addMetaFromStringFunc),
        getDataAsStringFunc(getDataAsStringFunc){};

        EDescriptor descriptor;
        std::string descriptorString;
        
        
        std::function< void(sdDataSetHandler*, std::string, std::string)> addDataFromStringFunc;
        std::function< std::shared_ptr<sdProtoMeta>(sdProtoEntity * entity, std::string)> addMetaFromStringFunc;
        std::function< std::shared_ptr<sdProtoEvent>(sdProtoEntity * entity, double, std::string)> addEventFromStringFunc;
        std::function< std::string(std::shared_ptr<sdProtoHolder>)> getDataAsStringFunc;

    };
    
    struct sdExtensionSpec{
        sdExtensionSpec(EExtension extension, std::string extensionString, std::vector<sdDescriptorSpec> descriptorSpecs, EDescriptor idDescriptor = SD_ERROR):
        extension(extension),
        extensionString(extensionString),
        descriptorSpecs(descriptorSpecs),
        idDescriptor(idDescriptor){};
        
        EExtension extension;
        std::string extensionString;
        std::vector<sdDescriptorSpec> descriptorSpecs;
        EDescriptor idDescriptor;
    };
    
    const static std::vector<sdExtensionSpec> spatDIFSpec;
    
    
#pragma mark extension
    static EExtension getExtensionOfDescriptor(EDescriptor descriptor){
        EExtension extension = EExtension::SD_EXTENSION_ERROR;
        auto it = std::find_if(spatDIFSpec.begin(), spatDIFSpec.end(), [&descriptor](const sdExtensionSpec &eSpec){
            auto itr = std::find_if(eSpec.descriptorSpecs.begin(), eSpec.descriptorSpecs.end(), [&descriptor](sdDescriptorSpec dSpec){
                return dSpec.descriptor == descriptor;
            });
            return itr != eSpec.descriptorSpecs.end();
        });
        if(it != spatDIFSpec.end()) extension = (*it).extension;
        if(extension == EExtension::SD_EXTENSION_ERROR){
            std::string message = std::string("cannot find extension for descriptor ") + sdSpec::descriptorToString(descriptor);
            throw InvalidExtensionException(message);
        }
        return extension;
    }
    
    static EDescriptor getIDDescriptorOfExtension(EExtension extension){
        auto it = std::find_if(spatDIFSpec.begin(), spatDIFSpec.end(), [&extension](const sdExtensionSpec &eSpec){
            return eSpec.extension == extension;
        });
        
        if(it == spatDIFSpec.end()) throw InvalidExtensionException("No such extension");
        if(it->idDescriptor == SD_ERROR) throw InvalidDescriptorException("Extension has no ID descriptor");
        
        return it->idDescriptor;
    }
    
    static EDescriptor getIDDescriptorFromMemberDescriptorInExtension(EDescriptor descriptor){
        EExtension extension = getExtensionOfDescriptor(descriptor);
        return getIDDescriptorOfExtension(extension);
    }
    
    static const std::vector<sdDescriptorSpec> &getDescriptorsOfExtension(EExtension extension){
        auto itr = std::find_if(spatDIFSpec.begin(), spatDIFSpec.end(), [&extension](sdExtensionSpec eSpec){return eSpec.extension == extension;});
        if(itr == spatDIFSpec.end()){throw InvalidDescriptorException("no matching string for EExtension"); }
        return (*itr).descriptorSpecs;
    }
    
#pragma mark string conversion
    static std::string extensionToString(const EExtension &extension){
        auto it = std::find_if(spatDIFSpec.begin(), spatDIFSpec.end(), [&extension](sdExtensionSpec eSpec){return eSpec.extension == extension;});
        if (it == spatDIFSpec.end()) return std::string();
        return (*it).extensionString;
    }
    
    static EExtension stringToExtension(std::string string){
        auto it = std::find_if(spatDIFSpec.begin(), spatDIFSpec.end(),[&string](sdExtensionSpec eSpec){return eSpec.extensionString == string;});
        if(it != spatDIFSpec.end()) return (*it).extension;
        return EExtension::SD_EXTENSION_ERROR;
    }

    static std::string descriptorToString(const EDescriptor &descriptor) {
        std::string str;
        std::find_if(spatDIFSpec.begin(), spatDIFSpec.end(), [&descriptor, &str](sdExtensionSpec eSpec){
            auto itr = std::find_if(eSpec.descriptorSpecs.begin(), eSpec.descriptorSpecs.end(), [&descriptor, &str](sdDescriptorSpec dSpec){
                if(dSpec.descriptor == descriptor){
                    str = dSpec.descriptorString;
                    return true;
                }else return false;
            });
            return itr != eSpec.descriptorSpecs.end();
        });
        return str;
    }
    
    static EDescriptor stringToDescriptor(EExtension extensionString, std::string descriptorString){
        auto it = std::find_if(spatDIFSpec.begin(), spatDIFSpec.end(),[&extensionString](sdExtensionSpec eSpec){return eSpec.extension == extensionString;});
        auto dSpecs = (*it).descriptorSpecs;
        auto iit = std::find_if(dSpecs.begin(), dSpecs.end(), [&descriptorString](sdDescriptorSpec dSpec){return dSpec.descriptorString == descriptorString;});
        if( iit == dSpecs.end()){throw InvalidDescriptorException(std::string("no matching descriptor for string:") + descriptorString );}
        return (*iit).descriptor;
    }
    
    static EDescriptor stringToDescriptor(std::string extensionString, std::string descriptorString){
        if(extensionString.empty()) extensionString = "core"; // assuming core
        auto itr = std::find_if(spatDIFSpec.begin(), spatDIFSpec.end(), [&extensionString](sdExtensionSpec eSpec)->bool{return eSpec.extensionString == extensionString;});
        if(itr == spatDIFSpec.end()) {
            throw InvalidExtensionException(std::string("no matching EEXtension for string: ") + extensionString);
        }
        
        EExtension extension = itr->extension;
        return stringToDescriptor(extension, descriptorString);
    }
    
#pragma mark ID utility
    static std::vector<EExtension> getDataSetEnabledExtensions(){
        std::vector<EExtension> extensionList;
        for(auto &spec : spatDIFSpec){
            if(spec.idDescriptor != SD_ERROR){ extensionList.push_back(spec.extension); }
        }
        return extensionList;
    }
    
#pragma getters for functor for addting data meta and event
    
    static std::function<void(sdDataSetHandler*, std::string, std::string)> getAddDataFunc(EDescriptor descriptor){
        return getDescriptorSpec(descriptor).addDataFromStringFunc;
    }
    
    static std::function<std::shared_ptr<sdProtoMeta>(sdProtoEntity* entity, std::string)> getAddMetaFunc(EDescriptor descriptor){
        return getDescriptorSpec(descriptor).addMetaFromStringFunc;
    }
    
    static std::function<std::shared_ptr<sdProtoEvent>(sdProtoEntity* entity, double,std::string)> getAddEventFunc(EDescriptor descriptor){
        return getDescriptorSpec(descriptor).addEventFromStringFunc;
    }
    
    static std::function< std::string(std::shared_ptr<sdProtoHolder>)> getDataAsStringFunc(EDescriptor desctriptor){
        return getDescriptorSpec(desctriptor).getDataAsStringFunc;
    }
    
    static sdDescriptorSpec &getDescriptorSpec(EDescriptor descriptor){
        auto ext = getExtensionOfDescriptor(descriptor);
        auto descriptors = getDescriptorsOfExtension(ext);
        auto it = std::find_if(descriptors.begin(), descriptors.end(), [&descriptor](sdDescriptorSpec dSpec){ return dSpec.descriptor == descriptor;});
        if(it == descriptors.end()){
            std::string msg = "invalid descriptor: ";
            msg += descriptorToString(descriptor);
            throw InvalidDescriptorException(msg);
        }
        return *it;
    }
    
};

