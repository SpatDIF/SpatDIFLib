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


#ifndef ____sdExtension__
#define ____sdExtension__


class sdEntity;
class sdProtoEvent;
class sdProtoMeta;
class sdExtension {
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
        sdESpec(EExtension extension, std::string extensionString, std::vector<sdDSpec> descriptorSpecs):extension(extension), extensionString(extensionString), descriptorSpecs(descriptorSpecs){};
        EExtension extension;
        std::string extensionString;
        std::vector<sdDSpec> descriptorSpecs;
    };
    
    
    const static std::vector<sdESpec> spatDIFSpec;
    
public:
    
#pragma mark extension
    static EExtension getExtensionOfDescriptor(EDescriptor descriptor){
        EExtension extension = EExtension::SD_EXTENSION_ERROR;
        auto it = std::find_if(spatDIFSpec.begin(), spatDIFSpec.end(), [&descriptor](sdESpec eSpec){
            auto itr = std::find_if(eSpec.descriptorSpecs.begin(), eSpec.descriptorSpecs.end(), [&descriptor](sdDSpec dSpec){
                return dSpec.descriptor == descriptor;
            });
            return itr != eSpec.descriptorSpecs.end();
        });
        if(it != spatDIFSpec.end()) extension = (*it).extension;
        return extension;
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
        if(itr == spatDIFSpec.end()){throw InvalidDescriptorException(); }
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
        if(it == spatDIFSpec.end()) return SD_ERROR;
        auto dSpecs = (*it).descriptorSpecs;
        auto iit = std::find_if(dSpecs.begin(), dSpecs.end(), [&string](sdDSpec dSpec){return dSpec.descriptorString == string;});
        if( iit == dSpecs.end()) return SD_ERROR;
        return (*iit).descriptor;
    }
    
    static EDescriptor stringToDescriptor(std::string extension, std::string string){
        
        auto itr = std::find_if(spatDIFSpec.begin(), spatDIFSpec.end(), [&extension](sdESpec eSpec)->bool{return eSpec.extensionString == extension;});
        if(itr == spatDIFSpec.end()) return SD_ERROR;
        const std::vector<sdDSpec> &descroptorVector = itr->descriptorSpecs;
        auto ditr = std::find_if(descroptorVector.begin(), descroptorVector.end(), [&string](sdDSpec dSpec)->bool{return dSpec.descriptorString == string;});
        if(ditr == descroptorVector.end()) return SD_ERROR;
        return ditr->descriptor;
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

#endif