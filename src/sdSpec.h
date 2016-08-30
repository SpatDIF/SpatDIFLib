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

class sdEntity;
class sdProtoEvent;
class sdProtoMeta;
class sdSpec {
protected:
    struct sdDSpec{
        sdDSpec(EDescriptor descriptor,
                std::string descriptorString,
                std::function< std::shared_ptr<sdProtoEvent>(sdEntity*, double, std::string)> addEventFromStringFunc,
                std::function< std::shared_ptr<sdProtoMeta>(sdEntity*, std::string)> addMetaFromStringFunc):
        descriptor(descriptor),
        descriptorString(descriptorString),
        addEventFromStringFunc(addEventFromStringFunc),
        addMetaFromStringFunc(addMetaFromStringFunc){};

        EDescriptor descriptor;
        std::string descriptorString;
        std::function< std::shared_ptr<sdProtoEvent>(sdEntity * entity, double, std::string)> addEventFromStringFunc;
        std::function< std::shared_ptr<sdProtoMeta>(sdEntity * entity, std::string)> addMetaFromStringFunc;
    };
    
    struct sdESpec{
        sdESpec(EExtension extension, std::string extensionString, std::vector<sdDSpec> descriptorSpecs, EDescriptor idDescriptor = SD_ERROR):
        extension(extension),
        extensionString(extensionString),
        descriptorSpecs(descriptorSpecs),
        idDescriptor(idDescriptor){};
        
        EExtension extension;
        EDescriptor idDescriptor;
        std::string extensionString;
        std::vector<sdDSpec> descriptorSpecs;
    };
    
    const static std::vector<sdESpec> spatDIFSpec;
    
public:
    
#pragma mark extension
    static EExtension getExtensionOfDescriptor(EDescriptor descriptor){
        EExtension extension = EExtension::SD_EXTENSION_ERROR;
        auto it = std::find_if(spatDIFSpec.begin(), spatDIFSpec.end(), [&descriptor](const sdESpec &eSpec){
            auto itr = std::find_if(eSpec.descriptorSpecs.begin(), eSpec.descriptorSpecs.end(), [&descriptor](sdDSpec dSpec){
                return dSpec.descriptor == descriptor;
            });
            return itr != eSpec.descriptorSpecs.end();
        });
        if(it != spatDIFSpec.end()) extension = (*it).extension;
        return extension;
    }
    
    static EDescriptor getIDDescriptorOfExtension(EExtension extension){
        auto it = std::find_if(spatDIFSpec.begin(), spatDIFSpec.end(), [&extension](const sdESpec &eSpec){
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
    
    static std::string extensionToString(const EExtension &extension){
        auto it = std::find_if(spatDIFSpec.begin(), spatDIFSpec.end(), [&extension](sdESpec eSpec){return eSpec.extension == extension;});
        if (it == spatDIFSpec.end()) return std::string();
        return (*it).extensionString;
    }
    
    static EExtension stringToExtension(std::string string){
        auto it = std::find_if(spatDIFSpec.begin(), spatDIFSpec.end(),[&string](sdESpec eSpec){return eSpec.extensionString == string;});
        if(it != spatDIFSpec.end()) return (*it).extension;
        return EExtension::SD_EXTENSION_ERROR;
    }
    
#pragma mark descriptor
    
    static const std::vector<sdDSpec> &getDescriptorsForExtension(EExtension extension){
        auto itr = std::find_if(spatDIFSpec.begin(), spatDIFSpec.end(), [&extension](sdESpec eSpec){return eSpec.extension == extension;});
        if(itr == spatDIFSpec.end()){throw InvalidDescriptorException("no matching string for EExtension"); }
        return (*itr).descriptorSpecs;
    }
    
    static std::string descriptorToString(const EDescriptor &descriptor) {
        std::string str;
        std::find_if(spatDIFSpec.begin(), spatDIFSpec.end(), [&descriptor, &str](sdESpec eSpec){
            auto itr = std::find_if(eSpec.descriptorSpecs.begin(), eSpec.descriptorSpecs.end(), [&descriptor, &str](sdDSpec dSpec){
                if(dSpec.descriptor == descriptor){
                    str = dSpec.descriptorString;
                    return true;
                }else return false;
            });
            return itr != eSpec.descriptorSpecs.end();
        });
        return str;
    }
    
    
    static EDescriptor stringToDescriptor(EExtension extension, std::string string){
        auto it = std::find_if(spatDIFSpec.begin(), spatDIFSpec.end(),[&extension](sdESpec eSpec){return eSpec.extension == extension;});
        auto dSpecs = (*it).descriptorSpecs;
        auto iit = std::find_if(dSpecs.begin(), dSpecs.end(), [&string](sdDSpec dSpec){return dSpec.descriptorString == string;});
        if( iit == dSpecs.end()){throw InvalidDescriptorException(std::string("no matching descriptor for string:") + string );}
        return (*iit).descriptor;
    }
    
    static EDescriptor stringToDescriptor(std::string extensionString, std::string descriptorString){
        if(extensionString.empty()) extensionString = "core"; // assuming core
        auto itr = std::find_if(spatDIFSpec.begin(), spatDIFSpec.end(), [&extensionString](sdESpec eSpec)->bool{return eSpec.extensionString == extensionString;});
        if(itr == spatDIFSpec.end()) {throw InvalidExtensionException(std::string("no matching EEXtension for string: ") + extensionString);}
        
        EExtension extension = itr->extension;
        return stringToDescriptor(extension, descriptorString);
    }
    
    static std::function<std::shared_ptr<sdProtoEvent>(sdEntity* entity, double,std::string)> getAddEventFunc(EDescriptor descriptor){
        auto ext = getExtensionOfDescriptor(descriptor);
        auto descriptors = getDescriptorsForExtension(ext);
        auto it = std::find_if(descriptors.begin(), descriptors.end(), [&descriptor](sdDSpec dSpec){ return dSpec.descriptor == descriptor;});
        if(it == descriptors.end())return nullptr;
        return (*it).addEventFromStringFunc;
    }
    
    static std::function<std::shared_ptr<sdProtoMeta>(sdEntity* entity, std::string)> getAddMetaFunc(EDescriptor descriptor){
        auto ext = getExtensionOfDescriptor(descriptor);
        auto descriptors = getDescriptorsForExtension(ext);
        auto it = std::find_if(descriptors.begin(), descriptors.end(), [&descriptor](sdDSpec dSpec){ return dSpec.descriptor == descriptor;});
        if(it == descriptors.end())return nullptr;
        return (*it).addMetaFromStringFunc;
    }
};

