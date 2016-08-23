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
#include "sdDescriptors.h"
#include "sdLoader.h"
#include "tinyxml2.h"
#include "libjson.h"
#include "JSONNode.h"
#include "JSONOptions.h"

sdScene sdLoader::sceneFromXML(std::string xmlString){
    using namespace tinyxml2;
    XMLDocument xml;
    XMLError err = xml.Parse(xmlString.c_str());
    if(err != XML_SUCCESS){throw FileErrorException();}
    
    sdScene scene;
    sdInfo info;
    
    
    XMLElement* spatdif = xml.FirstChildElement("spatdif");
    XMLElement* meta = spatdif->FirstChildElement("meta");
    
    {
        XMLElement* information = meta->FirstChildElement("info");
        XMLElement* author = information->FirstChildElement("author");
        XMLElement* host = information->FirstChildElement("host");
        XMLElement* date = information->FirstChildElement("date");
        XMLElement* session = information->FirstChildElement("session");
        XMLElement* location = information->FirstChildElement("location");
        XMLElement* annotation = information->FirstChildElement("annotation");
        XMLElement* title = information->FirstChildElement("title");
        XMLElement* duration = information->FirstChildElement("duration");
        
        if(author){info.setAuthor(std::string(author->GetText()));}
        if(date){info.setDate(std::string(date->GetText()));}
        if(host){info.setHost(std::string(host->GetText()));}
        if(session){info.setSession(std::string(session->GetText()));}
        if(location){info.setLocation(std::string(location->GetText()));}
        if(annotation){info.setAnnotation(std::string(annotation->GetText()));}
        if(title){info.setTitle(std::string(title->GetText()));}
        if(duration){info.setDuration(std::stod(duration->GetText()));}
        scene.setInfo(info);
    }
    
    // activate extension
    XMLElement* extensions = meta->FirstChildElement("extensions");
    if(extensions){
        std::string extensionsString = std::string(extensions->GetText());
        std::istringstream iss(extensionsString);
        while (iss.good()) { // add extensions one by one
            std::string extensionString;
            iss >> extensionString;
            scene.addExtension(extensionString);
        }
    }
    
    // create sink entities
    {
        XMLElement* sink = meta->FirstChildElement("sink");
        while (sink) {
            XMLElement *name = sink->FirstChildElement("name");
            XMLElement *type = sink->FirstChildElement("type");
            XMLElement *position = sink->FirstChildElement("position");
            XMLElement *hoPhysicalChannel = sink->FirstChildElement("physical-channel");
            XMLElement *hoGain = sink->FirstChildElement("gain");
            auto entityName = std::string(name->GetText());
            sdEntity* targetEntity = scene.getEntity(entityName);
            if(!targetEntity){
                targetEntity = scene.addEntity(entityName, EKind::SD_SINK);
                if(type)targetEntity->addMeta<SD_TYPE>(sdDescriptor<SD_TYPE>::stringTo(type->GetText()));
                if(position){
                    std::string unit = position->Attribute("unit");
                    if(unit == "aed"){
                        auto aed = sdUtils::stringToArray<double, 3>(position->GetText());
                        targetEntity->addMeta<SD_POSITION>(sdUtils::aedToXyz(aed));
                    }else{
                        targetEntity->addMeta<SD_POSITION>(sdUtils::stringToArray<double, 3>(position->GetText()));
                    }
                }
                if(hoPhysicalChannel){
                    int channel = sdUtils::stringTo<int>(hoPhysicalChannel->GetText());
                    targetEntity->addMeta<SD_HARDWARE_OUT_PHYSICAL_CHANNEL>(channel);
                }
                if(hoGain) {
                    targetEntity->addMeta<SD_HARDWARE_OUT_GAIN>(sdUtils::stringTo<double>(hoGain->GetText()));
                }
                
                sink = sink->NextSiblingElement("sink");
            }
        }
    }

    //first time tag
    std::string timeString;
    XMLElement* element = meta->NextSiblingElement();

    while(element){
        
        std::string tagType = element->Name();
        if(tagType == "time"){
            timeString = std::string(element->GetText());
        }
        else if(tagType == "source" || tagType == "sink"){
            XMLElement *name = element->FirstChildElement("name");
            std::string entityName = std::string(name->GetText());
            sdEntity* targetEntity;
            
            // if target entity does not exist, create one
            targetEntity = scene.getEntity(entityName);
            if(!targetEntity){
                if (tagType == "source") {
                    targetEntity = scene.addEntity(entityName, EKind::SD_SOURCE);
                }else{
                    targetEntity = scene.addEntity(entityName, EKind::SD_SINK);
                }
            }
            
            // get descriptor or extension type
            XMLElement *tempTag = name->NextSiblingElement();
            while(tempTag) {
                XMLElement *childTag = tempTag->FirstChildElement();
                if(!childTag){ // tempTag is a descriptor
                    XMLElement *descriptor = tempTag;
                    //targetEntity->addEvent(timeString, std::string(descriptor->Name()), std::string(descriptor->GetText()));

                }else{ // tempTag is a exType
                    XMLElement *descriptor = childTag;
                    while (descriptor) {
                        //targetEntity->addEvent(timeString, std::string(descriptor->Name()), std::string(descriptor->GetText()));
                        descriptor = descriptor->NextSiblingElement();
                    }
                }
                tempTag = tempTag->NextSiblingElement();
            }
        }

        element = element->NextSiblingElement();
    }
    
    return std::move(scene);
}

sdScene sdLoader::sceneFromJSON(std::string jsonString){
    JSONNode json;
    sdScene scene;
    sdInfo info;
    
    json = libjson::parse(jsonString);
    JSONNode::iterator it = json.find("spatdif");
    if(it != json.end()){
        JSONNode spatdif = *it;
        JSONNode::iterator iit = spatdif.find("meta");
        std::string timeString;

        while(iit != spatdif.end()){
            if(iit->name() ==  "meta"){
                JSONNode metaNode = *iit;
                JSONNode::iterator iiit = metaNode.begin();
                while(iiit != metaNode.end()){
                    if(iiit->name() == "info"){
                        JSONNode infoNode = *iiit;
                        JSONNode::iterator iiiit = infoNode.begin();
                        while(iiiit != infoNode.end()){
                            if(iiiit->name() == "author"){
                                info.setAuthor(iiiit->as_string());
                            }else if(iiiit->name() == "host"){
                                info.setHost(iiiit->as_string());
                            }else if(iiiit->name() == "date"){
                                info.setDate(iiiit->as_string());
                            }else if(iiiit->name() == "session"){
                                info.setSession(iiiit->as_string());
                            }else if(iiiit->name() == "location"){
                                info.setLocation(iiiit->as_string());
                            }else if(iiiit->name() == "annotation"){
                                info.setAnnotation(iiiit->as_string());
                            }
                            iiiit++;
                        }
                    }
                    else if(iiit->name() == "extensions"){
                        JSONNode extensionsNode = *iiit;
                        std::string extensionsString = iiit->as_string();
                        std::istringstream iss(extensionsString);
                        while (iss.good()) { // add extensions one by one
                            std::string extensionString;
                            iss >> extensionString;
                            scene.addExtension(extensionString);
                        }
                    }
                    iiit++;
                }
                scene.setInfo(info);
            }else if(iit->name() == "time"){
                JSONNode timeNode = *iit;
                timeString = timeNode.as_string();
            }else if(iit->name() == "source" || iit->name() == "sink" ){
                JSONNode source = *iit;
                JSONNode::iterator iiit = source.begin();
                sdEntity* targetEntity = nullptr;
                
                while(iiit != source.end()){
                    if(iiit ->name() == "name"){
                        targetEntity = scene.getEntity(iiit->as_string());
                        if(!targetEntity){
                            targetEntity = scene.addEntity(iiit->as_string());
                        }
                    }
                    else{
                        switch(iiit -> type()){
                            case JSON_STRING:{
                                //targetEntity->addEvent(timeString, iiit->name(), iiit->as_string());
                                break;
                            }
                            case JSON_ARRAY:{
                                JSONNode array = iiit->as_array();
                                JSONNode::iterator ait = array.begin();
                                std::ostringstream os;
                                while (ait != array.end()) {
                                    JSONNode num = *ait;
                                    os << num.as_string() << ' ';
                                    ait++;
                                }
                                
                                //targetEntity->addEvent(timeString, iiit->name(),os.str());
                                break;
                            }
                            case JSON_BOOL:{
                                std::string bl = sdUtils::toString(iiit->as_bool());
                                //targetEntity->addEvent(timeString, iiit->name(), bl);
                                break;
                            }
                            case JSON_NUMBER:{
                                
                                break;
                            }
                        }

                    }
                    iiit++;
                }
            }
            iit++;
        }
    }
    return scene;
}

sdScene sdLoader::sceneFromYAML(std::string yamlString){
    sdScene scene;
    return scene;
}




