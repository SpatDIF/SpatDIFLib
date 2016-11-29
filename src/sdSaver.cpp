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
#include <unordered_set>
#include "sdMain.h"
#include "tinyxml2.h"
#include "JSONNode.h"
#include "sdSaver.h"

using namespace tinyxml2;

XMLDeclaration* sdXMLSaver::XMLDeclarationSection(XMLDocument &xml){
    XMLDeclaration* decl = xml.NewDeclaration();
    decl->SetValue("xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"");
    return decl;
}

XMLElement* sdXMLSaver::XMLOrderingSection(XMLDocument &xml, const sdScene &scene){
    XMLElement* ordering = xml.NewElement("ordering");
    XMLText* orderingText = xml.NewText(scene.getOrderingAsString().c_str());
    ordering->InsertEndChild(orderingText);
    return  ordering;
}

XMLElement* sdXMLSaver::XMLMetaSection(XMLDocument &xml, const sdScene &scene){

    XMLElement* metaSection = xml.NewElement("meta");

    // add extensions to meta
    size_t num = scene.getNumberOfActivatedExtensions();
    if(num > 0){
        XMLElement* extensions = xml.NewElement("extensions");
        std::string extString;
        auto extensionNames = scene.getActivatedExtensionsAsStrings();
        std::for_each(extensionNames.begin(), extensionNames.end(),[&extString](std::string ext){
            EExtension extension = sdSpec::stringToExtension(ext);
            if(!sdSpec::isCoreSpec(extension)){
                extString = ext;
                extString += " ";
            }
        });
        extString.erase(extString.size()-1, extString.size()-1);
        XMLText* extensionsText = xml.NewText(extString.c_str());
        extensions->InsertEndChild(extensionsText);
        metaSection->InsertEndChild(extensions);
    }
    
    // add info section to meta
    std::vector<EExtension> extensions = sdSpec::getDataSetEnabledExtensions();
    for(auto &extension : extensions){
        std::unordered_set<std::string> identifiers = scene.getIdentifiers(extension);
        if(identifiers.empty()) continue;
        for(auto &identifier : identifiers){
            std::shared_ptr<sdProtoDataSet> protoDataSet = scene.getProtoDataSet(extension, identifier);
            std::unordered_set<EDescriptor> keys = protoDataSet->getKeys();
            XMLElement * extensionElement = xml.NewElement(sdSpec::extensionToString(extension).c_str());
            
            for(auto &key : keys){
                if(key == SD_POINTSET_POINT_OR_HANDLE){
                    sdDataSet<EExtension::SD_POINTSET> *pointset = static_cast<sdDataSet<EExtension::SD_POINTSET> *>(protoDataSet.get());
                    auto pointHandles = pointset->getValue<SD_POINTSET_POINT_OR_HANDLE>();
                    for(auto pointHandle : pointHandles){
                        EDescriptor descriptor = pointHandle.first;
                        std::string descriptorString = sdSpec::descriptorToString(descriptor);
                        std::array<double, 3> value = pointHandle.second;
                        XMLElement * datasetElement = xml.NewElement(descriptorString.c_str());
                        XMLText * text = xml.NewText(sdUtils::toString(value).c_str());
                        datasetElement->InsertEndChild(text);
                        extensionElement->InsertEndChild(datasetElement);
                    }
                }else{
                    std::string descriptorString = sdSpec::descriptorToString(key);
                    std::string value = protoDataSet->getValueAsString(key);
                    XMLElement * datasetElement = xml.NewElement(descriptorString.c_str());
                    if(!datasetElement) continue;
                    XMLText * text = xml.NewText(value.c_str());
                    if(!text) continue;

                    datasetElement->InsertEndChild(text);
                    extensionElement->InsertEndChild(datasetElement);
                }
            }
            metaSection->InsertEndChild(extensionElement);
        }
    }
    

    
    // add other meta infomation such as constant position or sound file definition
    auto allMetas = scene.getAllMetas();
    
    std::for_each(allMetas.begin(), allMetas.end(), [&](std::pair<sdEntity*, std::shared_ptr<sdProtoMeta>> metaPair ) {
        auto entity = metaPair.first;
        auto meta = metaPair.second;
        std::string previousName;
        std::string previousExtension;
        sdEntity * previousEntity = nullptr;
        XMLElement* category;
        XMLElement* extension;

        //event
        //category = xml.NewElement(entity->getCategoryAsString().c_str());
        category = xml.NewElement(std::string("source").c_str());

        // the name of entity always comes first
        XMLElement* name = xml.NewElement("name"); // entity name
        auto entityName = scene.getEntityName(entity);
        XMLText* nameText = xml.NewText(entityName.c_str());
        name->InsertEndChild(nameText);
        category->InsertEndChild(name);
    
        // packaging - combining element and text (value)
        XMLElement* element = xml.NewElement(meta->getDescriptorAsString().c_str());
        XMLText* text = xml.NewText(meta->getValueAsString().c_str());
        element->InsertEndChild(text);
        
        std::string relevantExtension = sdSpec::extensionToString(sdSpec::getExtensionOfDescriptor(meta->getDescriptor()));
        
        if(relevantExtension == "core"){
            category->InsertEndChild(element);
            metaSection->InsertEndChild(category);
        }else{
            
            if( (entity != previousEntity ) || (previousExtension != relevantExtension)){
                // if different entity, time, or extension from the previous put extension tag
                extension = xml.NewElement((relevantExtension).c_str());
            }
            extension->InsertEndChild(element);
            category->InsertEndChild(extension);
            metaSection->InsertEndChild(category);
        }
        
        previousEntity = entity; // store current name in order to avoid the dupplication.
        previousExtension = relevantExtension;
    });

    // add ordering
    metaSection->InsertEndChild(XMLOrderingSection(xml, scene));

    return metaSection;
}

std::string sdXMLSaver::toString(const sdScene &scene){
    
    XMLDocument xml;
    xml.InsertEndChild(XMLDeclarationSection(xml));
    
    XMLElement* spatdif = xml.NewElement("spatdif");
    spatdif->SetAttribute("version", "0.4");
    xml.InsertEndChild(spatdif);
    
    spatdif->InsertEndChild(sdXMLSaver::XMLMetaSection(xml, scene));

    std::unordered_map<std::string, sdEntity> entities = scene.getEntities();
    
    // TIME Section
    /* ordered by time */
    if(scene.getOrdering() == EOrdering::SD_TIME){
        auto eventTime = scene.getNextEventTime(-1.0);
        while(eventTime.second){
            
            // add <time>
            XMLElement* timeElement = xml.NewElement("time");
            timeElement->InsertEndChild(xml.NewText(std::to_string(eventTime.first).c_str()));
            spatdif->InsertEndChild(timeElement);
            
            for(auto entity : entities){
                auto entityEventsAtTime = entity.second.getEvents(eventTime.first);
                if(entityEventsAtTime.empty()) continue;
                
                XMLElement * categoryElement = xml.NewElement(entity.second.getCategoryAsString().c_str());
                XMLElement * nameElement = xml.NewElement("name"); // entity name

                XMLText* nameText = xml.NewText(entity.first.c_str());
                nameElement->InsertEndChild(nameText);
                categoryElement->InsertEndChild(nameElement);

                XMLElement* extensionElement = nullptr;
                std::string previousExtension = "";
                for(auto entityEvent : entityEventsAtTime){
                    XMLElement* eventElement = xml.NewElement(entityEvent->getDescriptorAsString().c_str());
                    eventElement->InsertEndChild(xml.NewText(entityEvent->getValueAsString().c_str()));
                    std::string relevantExtension = sdSpec::extensionToString(sdSpec::getExtensionOfDescriptor(entityEvent->getDescriptor()));
        
                    if(relevantExtension == "core"){
                        categoryElement->InsertEndChild(eventElement);
                    }else{// extension
                        if(previousExtension == relevantExtension){
                            extensionElement->InsertEndChild(eventElement);
                        }else{
                            extensionElement = xml.NewElement((relevantExtension).c_str());
                            extensionElement->InsertEndChild(eventElement);
                            categoryElement->InsertEndChild(extensionElement);
                        }
                    }
                    previousExtension = relevantExtension;
                }
                spatdif->InsertEndChild(categoryElement);
            }
            eventTime = scene.getNextEventTime(eventTime.first);
        }
 
    }else if(scene.getOrdering() == EOrdering::SD_TRACK){
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
                
                XMLElement* category;
                category = xml.NewElement(entity.getCategoryAsString().c_str());
                XMLElement* name = xml.NewElement("name");
                XMLText* nameText = xml.NewText(entityName.c_str());
                name->InsertEndChild(nameText);
                category->InsertEndChild(name);
                
                XMLElement* element = xml.NewElement(event->getDescriptorAsString().c_str());
                XMLText* text = xml.NewText(event->getValueAsString().c_str());
                element->InsertEndChild(text);
                category->InsertEndChild(element);
                
                spatdif->InsertEndChild(category);
            }
        }
    }

    XMLPrinter printer;
    xml.Print(&printer);
    return std::string(printer.CStr());
    
}

std::string sdJSONSaver::toString(const sdScene &sdScene){
    return "";
}

std::string sdYAMLSaver::toString(const sdScene &sdScene){
    return "";
}
