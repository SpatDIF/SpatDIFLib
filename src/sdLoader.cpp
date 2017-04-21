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

#include <iostream>
#include <string>
#include <sstream>
#include "sdDescriptorSpec.h"
#include "sdSpec.h"
#include "sdLoader.h"
#include "libjson.h"
#include "JSONNode.h"
#include "JSONOptions.h"
#include "rapidxml.hpp"

sdScene sdXMLLoader::sceneFromXML(std::string xmlString){
    using namespace rapidxml;
    xml_document<> doc;
    doc.parse<0>(const_cast<char *>(xmlString.c_str())); // rapidXML is in-situ so const cannot be passed but xmlString is local string
    
    sdScene scene;

    auto spatdif = doc.first_node("spatdif");
    auto meta = spatdif->first_node("meta");
    {
        std::vector<EExtension> enabledExtension = sdSpec::getDataSetEnabledExtensions();

        for(EExtension extension : enabledExtension){
            std::string extensionString = sdSpec::extensionToString(extension);
            auto datasetElement = meta->first_node(extensionString.c_str());
            if(!datasetElement) continue;
            
            auto descriptorSpecs = sdSpec::getDescriptorsOfExtension(extension);
            for(auto descriptorSpec : descriptorSpecs){
                auto metaElement = datasetElement->first_node(descriptorSpec.descriptorString.c_str());
                if(!metaElement) continue;
                if(metaElement->value_size() != 0){
                    std::string valueString = metaElement->value();
                    scene.setData(extensionString, descriptorSpec.descriptor, valueString);
                }
                metaElement = metaElement->next_sibling();
            }
        }
    }
    
    // activate extension
    auto extensions = meta->first_node("extensions");
    if(extensions){
        std::string extensionsString = std::string(extensions->value());
        std::istringstream iss(extensionsString);
        while (iss.good()) { // add extensions one by one
            std::string extensionString;
            iss >> extensionString;
            scene.addExtension(extensionString);
        }
    }
    
    // create sink entities
    {
        auto sink = meta->first_node("sink");
        while (sink) {
            auto *name = sink->first_node("name");
            auto *type = sink->first_node("type");
            auto *position = sink->first_node("position");
            auto *hoPhysicalChannel = sink->first_node("physical-channel");
            auto *hoGain = sink->first_node("gain");
            auto entityName = std::string(name->value());
            sdEntity* targetEntity = scene.getEntity(entityName);
            if(!targetEntity){
                targetEntity = scene.addEntity(entityName, ECATEGORY::SD_SINK);
                if(type)targetEntity->addMeta<SD_TYPE>(sdDescriptorSpec<SD_TYPE>::stringTo(type->value()));
                if(position){
                    std::string unit = position->first_attribute("unit")->value();
                    if(unit == "aed"){
                        auto aed = sdUtils::stringToArray<double, 3>(position->value());
                        targetEntity->addMeta<SD_POSITION>(sdUtils::aedToXyz(aed));
                    }else{
                        targetEntity->addMeta<SD_POSITION>(sdUtils::stringToArray<double, 3>(position->value()));
                    }
                }
                if(hoPhysicalChannel){
                    int channel = sdUtils::stringTo<int>(hoPhysicalChannel->value());
                    targetEntity->addMeta<SD_HARDWARE_OUT_PHYSICAL_CHANNEL>(channel);
                }
                if(hoGain) {
                    targetEntity->addMeta<SD_HARDWARE_OUT_GAIN>(sdUtils::stringTo<double>(hoGain->value()));
                }
                
                sink = sink->next_sibling("sink");
            }
        }
    }

    //first time tag
    std::string timeString;
    auto element = meta->next_sibling();

    while(element){
        std::string tagType = element->name();
        if(tagType == "time"){
            timeString = std::string(element->value());
            
        }else if(tagType == "source" || tagType == "sink"){
            auto name = element->first_node("name");
            std::string entityName = std::string(name->value());
            sdEntity* targetEntity;
            
            // if target entity does not exist, create one
            targetEntity = scene.getEntity(entityName);
            if(!targetEntity){
                if (tagType == "source") {
                    targetEntity = scene.addEntity(entityName, ECATEGORY::SD_SOURCE);
                }else{
                    targetEntity = scene.addEntity(entityName, ECATEGORY::SD_SINK);
                }
            }
            
            auto node = element->first_node();
            while(node){
                if(std::string(node->name()) == "name") {
                    node = node->next_sibling();
                    continue;
                }

                xml_node<> * childTag = node->first_node();
                if(childTag->type() == node_data){ // node is a descriptor
                    xml_node<> * descriptor = node;
                    targetEntity->addEvent(timeString, "core", std::string(descriptor->name()), std::string(descriptor->value()), false);
                }else if(childTag->type() == node_element){ // tempTag is a extension type
                    xml_node<> * descriptor = childTag;
                    while (descriptor) {
                        targetEntity->addEvent(timeString, node->name(), descriptor->name(), descriptor->value(), false); // no sort for performance gain
                        descriptor = descriptor->next_sibling();
                    }
                }
                node = node->next_sibling();
            }
        }
        element = element->next_sibling();
        
    }
    scene.sort();
    return std::move(scene);
}

sdScene sdXMLLoader::sceneFromJSON(std::string jsonString){
    sdScene scene;

//    JSONNode json;
//    sdInfo info;
//    
//    json = libjson::parse(jsonString);
//    JSONNode::iterator it = json.find("spatdif");
//    if(it != json.end()){
//        JSONNode spatdif = *it;
//        JSONNode::iterator iit = spatdif.find("meta");
//        std::string timeString;
//
//        while(iit != spatdif.end()){
//            if(iit->name() ==  "meta"){
//                JSONNode metaNode = *iit;
//                JSONNode::iterator iiit = metaNode.begin();
//                while(iiit != metaNode.end()){
//                    if(iiit->name() == "info"){
//                        JSONNode infoNode = *iiit;
//                        JSONNode::iterator iiiit = infoNode.begin();
//                        while(iiiit != infoNode.end()){
//                            if(iiiit->name() == "author"){
//                                info.setAuthor(iiiit->as_string());
//                            }else if(iiiit->name() == "host"){
//                                info.setHost(iiiit->as_string());
//                            }else if(iiiit->name() == "date"){
//                                info.setDate(iiiit->as_string());
//                            }else if(iiiit->name() == "session"){
//                                info.setSession(iiiit->as_string());
//                            }else if(iiiit->name() == "location"){
//                                info.setLocation(iiiit->as_string());
//                            }else if(iiiit->name() == "annotation"){
//                                info.setAnnotation(iiiit->as_string());
//                            }
//                            iiiit++;
//                        }
//                    }
//                    else if(iiit->name() == "extensions"){
//                        JSONNode extensionsNode = *iiit;
//                        std::string extensionsString = iiit->as_string();
//                        std::istringstream iss(extensionsString);
//                        while (iss.good()) { // add extensions one by one
//                            std::string extensionString;
//                            iss >> extensionString;
//                            scene.addExtension(extensionString);
//                        }
//                    }
//                    iiit++;
//                }
//                scene.setInfo(info);
//            }else if(iit->name() == "time"){
//                JSONNode timeNode = *iit;
//                timeString = timeNode.as_string();
//            }else if(iit->name() == "source" || iit->name() == "sink" ){
//                JSONNode source = *iit;
//                JSONNode::iterator iiit = source.begin();
//                sdEntity* targetEntity = nullptr;
//                
//                while(iiit != source.end()){
//                    if(iiit ->name() == "name"){
//                        targetEntity = scene.getEntity(iiit->as_string());
//                        if(!targetEntity){
//                            targetEntity = scene.addEntity(iiit->as_string());
//                        }
//                    }
//                    else{
//                        switch(iiit -> type()){
//                            case JSON_STRING:{
//                                //targetEntity->addEvent(timeString, iiit->name(), iiit->as_string());
//                                break;
//                            }
//                            case JSON_ARRAY:{
//                                JSONNode array = iiit->as_array();
//                                JSONNode::iterator ait = array.begin();
//                                std::ostringstream os;
//                                while (ait != array.end()) {
//                                    JSONNode num = *ait;
//                                    os << num.as_string() << ' ';
//                                    ait++;
//                                }
//                                
//                                //targetEntity->addEvent(timeString, iiit->name(),os.str());
//                                break;
//                            }
//                            case JSON_BOOL:{
//                                std::string bl = sdUtils::toString(iiit->as_bool());
//                                //targetEntity->addEvent(timeString, iiit->name(), bl);
//                                break;
//                            }
//                            case JSON_NUMBER:{
//                                
//                                break;
//                            }
//                        }
//
//                    }
//                    iiit++;
//                }
//            }
//            iit++;
//        }
//    }
    return scene;
}

sdScene sdXMLLoader::sceneFromYAML(std::string yamlString){
    sdScene scene;
    return scene;
}




