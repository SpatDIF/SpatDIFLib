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

    std::string infoStrings[6];
    std::string elementNameStrings[6];
    
    infoStrings[0] = information.getAuthor();
    infoStrings[1] = information.getHost();
    infoStrings[2] = information.getDateAsString();
    infoStrings[3] = information.getLocation();
    infoStrings[4] = information.getSession();
    infoStrings[5] = information.getAnnotation();

    elementNameStrings[0] = "author";
    elementNameStrings[1] = "host";
    elementNameStrings[2] = "date";
    elementNameStrings[3] = "location";
    elementNameStrings[4] = "session";
    elementNameStrings[5] = "annotation";
    
    for(int i = 0; i< 6; i++){
        if(!infoStrings[i].empty()){
            XMLElement* elem  = xml.NewElement(elementNameStrings[i].c_str());
            XMLText* tex = xml.NewText(infoStrings[i].c_str());
            elem->InsertEndChild(tex);
            info->InsertEndChild(elem);
        }
    }
    return info;
}

XMLElement* sdSaver::XMLOrderingSection(XMLDocument &xml, sdScene *scene){
    XMLElement* ordering = xml.NewElement("ordering");
    XMLText* orderingText = xml.NewText(scene->getOrderingAsString().c_str());
    ordering->InsertEndChild(orderingText);
    return  ordering;
}


XMLElement* sdSaver::XMLMetaSection(XMLDocument &xml, sdScene *scene){

    XMLElement* meta = xml.NewElement("meta");

    // add info section to meta
    meta->InsertEndChild(sdSaver::XMLInfoSection(xml, scene));

    // add extensions to meta
    size_t num = scene->getNumberOfActivatedExtensions();
    if(num > 0){
        XMLElement* extensions = xml.NewElement("extensions");
        std::string extString;
        auto extensionNames = scene->getActivatedExtensionsAsStrings();
        std::for_each(extensionNames.begin(), extensionNames.end(),[&extString](std::string ext){extString += ext;});
        
        XMLText* extensionsText = xml.NewText(extString.c_str());
        extensions->InsertEndChild(extensionsText);
        meta->InsertEndChild(extensions);
    }
    
    // add ordering
    meta->InsertEndChild(XMLOrderingSection(xml, scene));


    return meta;
}

std::string sdSaver::XMLFromScene(sdScene *scene){
    
    XMLDocument xml;
 	XMLDeclaration* decl = xml.NewDeclaration();
	decl->SetValue("xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"");
	xml.InsertEndChild(decl);
    
    XMLElement* spatdif = xml.NewElement("spatdif");
	xml.InsertEndChild(spatdif);
    spatdif->SetAttribute("version", "0.3");

    spatdif->InsertEndChild(sdSaver::XMLMetaSection(xml, scene));

    
    /* ordered by time */
    if(scene->getOrdering() == EOrdering::SD_TIME){
        
        // 2. create string
        double previousTime = -1.0;
        std::string previousName;
        std::string previousExtension;
        sdEntity * previousEntity = nullptr;
        XMLElement* extension;
        XMLElement* kind;
        auto allEvents = scene->getAllEvents();
        
        std::for_each(allEvents.begin(), allEvents.end(), [&](std::pair<sdEntity*, std::shared_ptr<sdProtoEvent>> eventPair ) {
            auto entity = eventPair.first;
            auto event = eventPair.second;
            
            //time tag
            if(event->getTime() != previousTime){ // if not same as the previous time tag, make a new time tag
                XMLElement* time = xml.NewElement("time");
                XMLText* timeText = xml.NewText(event->getTimeAsString().c_str());
                time->InsertEndChild(timeText);
                spatdif->InsertEndChild(time);
            }
            
            //event
            if((event->getTime() != previousTime) || (entity != previousEntity)){
                // if name of entity or time tag changes we need to write many tags!
                
                kind = xml.NewElement(entity->getKindAsString().c_str());
                
                // the name of entity always comes first
                XMLElement* name = xml.NewElement("name"); // entity name
                auto entityName = scene->getEntityName(entity);
                XMLText* nameText = xml.NewText(entityName.c_str());
                name->InsertEndChild(nameText);
                kind->InsertEndChild(name);
            }
            
            // packaging - combining element and text (value)
            XMLElement* element = xml.NewElement(event->getDescriptorAsString().c_str());
            XMLText* text = xml.NewText(event->getValueAsString().c_str());
            element->InsertEndChild(text);
            
            std::string relevantExtension = sdExtension::extensionToString(sdExtension::getExtensionOfDescriptor(event->getDescriptor()));
            
            if(relevantExtension == "core"){
                kind->InsertEndChild(element);
                spatdif->InsertEndChild(kind);
            }else{
                
                if( (event->getTime() != previousTime) ||  (entity != previousEntity ) || (previousExtension != relevantExtension)){
                    // if different entity, time, or extension from the previous put extension tag
                    extension = xml.NewElement((relevantExtension).c_str());
                }
                extension->InsertEndChild(element);
                kind->InsertEndChild(extension);
                spatdif->InsertEndChild(kind);
            }
            
            previousEntity = entity; // store current name in order to avoid the dupplication.
            previousExtension = relevantExtension;
            previousTime = event->getTime();
        });

    }else if(scene->getOrdering() == EOrdering::SD_TRACK){
        // 1. Sort vector by name alphabetically
        
        auto entities = scene->getEntities();
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