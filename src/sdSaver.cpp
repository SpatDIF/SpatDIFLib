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

#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include "sdMain.h"
#include "tinyxml2.h"
#include "JSONNode.h"
#include "sdSaver.h"

using namespace tinyxml2;

XMLElement* sdSaver::XMLInfoSection(XMLDocument &xml, sdScene *scene){

    XMLElement* info = xml.NewElement("info");
    sdInfo information = scene->getInfo();

    std::vector<std::pair<std::string ,std::string>> infoStrings = {
        {"author",information.getAuthor()},
        {"host",information.getHost()},
        {"date",information.getDateAsString()},
        {"location",information.getLocation()},
        {"session",information.getSession()},
        {"annotation",information.getAnnotation()},
        {"title",information.getTitle()},
        {"duration",std::to_string(information.getDuration())}
    };
    
    std::for_each(infoStrings.begin(), infoStrings.end(), [&xml, &info](const std::pair<std::string ,std::string> &infoString){
        if(!infoString.second.empty()){
            XMLElement* elem  = xml.NewElement(infoString.first.c_str());
            XMLText* tex = xml.NewText(infoString.second.c_str());
            elem->InsertEndChild(tex);
            info->InsertEndChild(elem);
        }
    });

    return info;
}

XMLElement* sdSaver::XMLOrderingSection(XMLDocument &xml, sdScene *scene){
    XMLElement* ordering = xml.NewElement("ordering");
    XMLText* orderingText = xml.NewText(scene->getOrderingAsString().c_str());
    ordering->InsertEndChild(orderingText);
    return  ordering;
}

XMLElement* sdSaver::XMLMetaSection(XMLDocument &xml, sdScene *scene){

    XMLElement* metaSection = xml.NewElement("meta");

    // add info section to meta
    metaSection->InsertEndChild(sdSaver::XMLInfoSection(xml, scene));

    // add extensions to meta
    size_t num = scene->getNumberOfActivatedExtensions();
    if(num > 0){
        XMLElement* extensions = xml.NewElement("extensions");
        std::string extString;
        auto extensionNames = scene->getActivatedExtensionsAsStrings();
        std::for_each(extensionNames.begin(), extensionNames.end(),[&extString](std::string ext){
            if(!(ext == "media" || ext == "core")){
                extString += ext;
                extString += " ";
            }
        });
        extString.erase(extString.size()-1, extString.size()-1);
        XMLText* extensionsText = xml.NewText(extString.c_str());
        extensions->InsertEndChild(extensionsText);
        metaSection->InsertEndChild(extensions);
    }
    
    // add other meta infomation such as constant position or sound file definition
    auto allMetas = scene->getAllMetas();
    
    std::for_each(allMetas.begin(), allMetas.end(), [&](std::pair<sdProtoEntity*, std::shared_ptr<sdProtoMeta>> metaPair ) {
        auto entity = metaPair.first;
        auto meta = metaPair.second;
        std::string previousName;
        std::string previousExtension;
        sdProtoEntity * previousEntity = nullptr;
        XMLElement* kind;
        XMLElement* extension;

        //event
        //kind = xml.NewElement(entity->getKindAsString().c_str());
        kind = xml.NewElement(std::string("source").c_str());

        // the name of entity always comes first
        XMLElement* name = xml.NewElement("name"); // entity name
        auto entityName = scene->getEntityName(entity);
        XMLText* nameText = xml.NewText(entityName.c_str());
        name->InsertEndChild(nameText);
        kind->InsertEndChild(name);
    
        // packaging - combining element and text (value)
        XMLElement* element = xml.NewElement(meta->getDescriptorAsString().c_str());
        XMLText* text = xml.NewText(meta->getValueAsString().c_str());
        element->InsertEndChild(text);
        
        std::string relevantExtension = sdSpec::extensionToString(sdSpec::getExtensionOfDescriptor(meta->getDescriptor()));
        
        if(relevantExtension == "core"){
            kind->InsertEndChild(element);
            metaSection->InsertEndChild(kind);
        }else{
            
            if( (entity != previousEntity ) || (previousExtension != relevantExtension)){
                // if different entity, time, or extension from the previous put extension tag
                extension = xml.NewElement((relevantExtension).c_str());
            }
            extension->InsertEndChild(element);
            kind->InsertEndChild(extension);
            metaSection->InsertEndChild(kind);
        }
        
        previousEntity = entity; // store current name in order to avoid the dupplication.
        previousExtension = relevantExtension;
    });

    // add ordering
    metaSection->InsertEndChild(XMLOrderingSection(xml, scene));

    return metaSection;
}

std::string sdSaver::XMLFromScene(sdScene *scene){
    
    XMLDocument xml;
 	XMLDeclaration* decl = xml.NewDeclaration();
	decl->SetValue("xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"");
	xml.InsertEndChild(decl);
    
    XMLElement* spatdif = xml.NewElement("spatdif");
	xml.InsertEndChild(spatdif);
    spatdif->SetAttribute("version", "0.4");

    
    // META Section
    spatdif->InsertEndChild(sdSaver::XMLMetaSection(xml, scene));

    std::unordered_map<std::string, sdEntity> entities = scene->getEntities();
    
    // TIME Section
    /* ordered by time */
    if(scene->getOrdering() == EOrdering::SD_TIME){
        auto eventTime = scene->getNextEventTime(-1.0);
        while(eventTime.second){
            
            // add <time>
            XMLElement* timeElement = xml.NewElement("time");
            timeElement->InsertEndChild(xml.NewText(std::to_string(eventTime.first).c_str()));
            spatdif->InsertEndChild(timeElement);
            
            for(auto entity : entities){
                auto entityEventsAtTime = entity.second.getEvents(eventTime.first);
                if(entityEventsAtTime.empty()) continue;
                
                XMLElement * kindElement = xml.NewElement(entity.second.getKindAsString().c_str());
                XMLElement * nameElement = xml.NewElement("name"); // entity name

                XMLText* nameText = xml.NewText(entity.first.c_str());
                nameElement->InsertEndChild(nameText);
                kindElement->InsertEndChild(nameElement);

                XMLElement* extensionElement = nullptr;
                std::string previousExtension = "";
                for(auto entityEvent : entityEventsAtTime){
                    XMLElement* eventElement = xml.NewElement(entityEvent->getDescriptorAsString().c_str());
                    eventElement->InsertEndChild(xml.NewText(entityEvent->getValueAsString().c_str()));
                    std::string relevantExtension = sdSpec::extensionToString(sdSpec::getExtensionOfDescriptor(entityEvent->getDescriptor()));
        
                    if(relevantExtension == "core"){
                        kindElement->InsertEndChild(eventElement);
                    }else{// extension
                        if(previousExtension == relevantExtension){
                            extensionElement->InsertEndChild(eventElement);
                        }else{
                            extensionElement = xml.NewElement((relevantExtension).c_str());
                            extensionElement->InsertEndChild(eventElement);
                            kindElement->InsertEndChild(extensionElement);
                        }
                    }
                    previousExtension = relevantExtension;
                }
                spatdif->InsertEndChild(kindElement);
            }
            eventTime = scene->getNextEventTime(eventTime.first);
        }
 
    }else if(scene->getOrdering() == EOrdering::SD_TRACK){
        // 1. Sort vector by name alphabetically
        
        for(auto it = entities.begin(); it != entities.end(); it++ ){
            auto namedEntity = *it;
            auto entityName = (*it).first;
            auto entity = (*it).second;
            auto events = entity.getEvents();

            for(auto iit = events.begin(); iit != events.end(); iit++){
                std::shared_ptr<sdProtoEvent> event = *iit;
                XMLElement* time = xml.NewElement("time");
                XMLText* timeText = xml.NewText(event->getTimeAsString().c_str());
                time->InsertEndChild(timeText);
                spatdif->InsertEndChild(time);
                
                XMLElement* kind;
                kind = xml.NewElement(entity.getKindAsString().c_str());
                XMLElement* name = xml.NewElement("name");
                XMLText* nameText = xml.NewText(entityName.c_str());
                name->InsertEndChild(nameText);
                kind->InsertEndChild(name);
                
                XMLElement* element = xml.NewElement(event->getDescriptorAsString().c_str());
                XMLText* text = xml.NewText(event->getValueAsString().c_str());
                element->InsertEndChild(text);
                kind->InsertEndChild(element);
                
                spatdif->InsertEndChild(kind);
            }
        }
    }

    XMLPrinter printer;
    xml.Print(&printer);
    return std::string(printer.CStr());
    
}

std::string sdSaver::JSONFromScene( sdScene *sdScene){
    return NULL;
    
    
}

std::string sdSaver::YAMLFromScene( sdScene *sdScene){
    return NULL;
}