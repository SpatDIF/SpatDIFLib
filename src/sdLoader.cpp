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

void sdXMLLoader::activateExtensions(sdScene &scene, const std::string &extensionsString){
    // activate extension
    std::istringstream iss(extensionsString);
    while (iss.good()) { // add extensions one by one
        std::string extensionString;
        iss >> extensionString;
        scene.addExtension(extensionString);
    }
}

sdScene sdXMLLoader::sceneFromXML(std::string xmlString){
    using namespace rapidxml;
    xml_document<> doc;
    doc.parse<0>(const_cast<char *>(xmlString.c_str())); // rapidXML is in-situ so const cannot be passed but xmlString is local string
    
    sdScene scene;

    auto spatdif = doc.first_node("spatdif");
    auto meta = spatdif->first_node("meta");
    {
        
        std::vector<EExtension> enabledExtension = sdSpec::getDataSetEnabledExtensions();

        /* check the dataset definition in the meta section and add to scene */
        
        for(auto metaNode = meta->first_node(); metaNode; metaNode = metaNode->next_sibling()){
            std::string nodeName = metaNode->name();
            if(nodeName == "extensions"){
                activateExtensions(scene, metaNode->value());
            }else if(nodeName == "source" || nodeName == "sink"){
                auto *name = metaNode->first_node("name");
                auto entityName = std::string(name->value());
                sdEntity* targetEntity = scene.getEntity(entityName);
                if(!targetEntity){
                    ECATEGORY cat = nodeName == "source" ? ECATEGORY::SD_SOURCE : ECATEGORY::SD_SINK;
                    targetEntity = scene.addEntity(entityName, cat);
                }
                //setup property
                auto *type = metaNode->first_node("type");
                if(type)targetEntity->addMeta<SD_TYPE>(sdDescriptorSpec<SD_TYPE>::stringTo(type->value()));

                auto *position = metaNode->first_node("position");
                if(position){
                    xml_attribute<> * attribute = position->first_attribute("unit");
                    std::string unit;
                    if(attribute){
                        unit = attribute->value();
                    }
                    
                    if(unit == "aed"){
                        auto aed = sdUtils::stringToArray<double, 3>(position->value());
                        targetEntity->addMeta<SD_POSITION>(sdUtils::aedToXyz(aed));
                    }else{
                        targetEntity->addMeta<SD_POSITION>(sdUtils::stringToArray<double, 3>(position->value()));
                    }
                }
                auto *mediaNode = metaNode->first_node("media");
                if(mediaNode){
                    auto *idNode = mediaNode->first_node("id");
                    if(idNode){
                        targetEntity->addMeta<SD_MEDIA_ID>(idNode->value());
                    }
                }
                
                if(nodeName == "sink"){
                    auto *physicalChannel = metaNode->first_node("physical-channel");
                    if(physicalChannel){
                        int channel = sdUtils::stringTo<int>(physicalChannel->value());
                        targetEntity->addMeta<SD_HARDWARE_OUT_PHYSICAL_CHANNEL>(channel);
                    }
                }
                
                auto *gain = metaNode->first_node("gain");
                if(gain) {
                    targetEntity->addMeta<SD_HARDWARE_OUT_GAIN>(sdUtils::stringTo<double>(gain->value()));
                }
            }else if(nodeName == "media"){
                // need to rework!!

                // dataset ... minimal implementation now
                auto *id = metaNode->first_node("id");
                auto *type = metaNode->first_node("type");
                auto *location = metaNode->first_node("location");
                auto *channel = metaNode->first_node("channel");
                auto *time_offset = metaNode->first_node("time-offset");
                auto *gain = metaNode->first_node("gain");
                if(!id)continue;
                if(!type)continue;
                
                std::string idVal = id->value();
                std::string typeVal = type->value();
                std::string locationVal = "unkonwn";
                int channelVal = 1;
                double time_offsetVal = 0.0;
                double gainVal = 1.0;
                
                if(location) locationVal = location->value();
                if(channel) channelVal = sdUtils::stringTo<int>(channel->value());
                if(time_offset) time_offsetVal = sdUtils::stringTo<double>(time_offset->value());
                if(gain) gainVal = sdUtils::stringTo<double>(gain->value());

                sdDataSet<EExtension::SD_MEDIA> dataset = sdDataSet<EExtension::SD_MEDIA>(
                                                                                             idVal,sdDescriptorSpec<SD_MEDIA_TYPE>::SD_FILE,
                                                                                          locationVal, channelVal, time_offsetVal, gainVal );

                scene.addDataSet(EExtension::SD_MEDIA, dataset );
                
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

//sdScene sdXMLLoader::sceneFromJSON(std::string jsonString){
//    sdScene scene;

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
//    return scene;
//}
//
//sdScene sdXMLLoader::sceneFromYAML(std::string yamlString){
//    sdScene scene;
//    return scene;
//}




