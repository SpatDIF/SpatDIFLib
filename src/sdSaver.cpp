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
#include "rapidxml.hpp"
#include "rapidxml_print.hpp"
#include "JSONNode.h"
#include "sdSaver.h"

using namespace rapidxml;


xml_node<> * sdXMLSaver::createOrderingSection(xml_document<> &xml, const sdScene &scene){
    return xml.allocate_node(node_element, "ordering", xml.allocate_string(scene.getOrderingAsString().c_str()));
}

xml_node<> * sdXMLSaver::createMetaSection(xml_document<> &xml, const sdScene &scene){

    xml_node<> * metaSection = xml.allocate_node(node_element, "meta");

    // add info section to meta
    std::vector<EExtension> extensions = sdSpec::getDataSetEnabledExtensions();
    for(auto &extension : extensions){
        std::unordered_set<std::string> identifiers = scene.getIdentifiers(extension);
        if(identifiers.empty()) continue;
        for(auto &identifier : identifiers){
            std::shared_ptr<sdProtoDataSet> protoDataSet = scene.getProtoDataSet(extension, identifier);
            std::unordered_set<EDescriptor> keys = protoDataSet->getKeys();
            xml_node<> * extensionElement = xml.allocate_node(node_element, xml.allocate_string(sdSpec::extensionToString(extension).c_str()));
            
            for(auto &key : keys){
                if(key == SD_POINTSET_POINT_OR_HANDLE){
                    sdDataSet<EExtension::SD_POINTSET> *pointset = static_cast<sdDataSet<EExtension::SD_POINTSET> *>(protoDataSet.get());
                    auto pointHandles = pointset->getValue<SD_POINTSET_POINT_OR_HANDLE>();
                    for(auto pointHandle : pointHandles){
                        EDescriptor descriptor = pointHandle.first;
                        std::string descriptorString = sdSpec::descriptorToString(descriptor);
                        std::array<double, 3> value = pointHandle.second;
                        xml_node<> * datasetElement = xml.allocate_node(node_element, descriptorString.c_str(), xml.allocate_string(sdUtils::toString(value).c_str()));
                        extensionElement->append_node(datasetElement);
                    }
                }else{
                    std::string descriptorString = sdSpec::descriptorToString(key);
                    std::string value = protoDataSet->getValueAsString(key);
                    xml_node<> * datasetElement = xml.allocate_node(node_element, xml.allocate_string(descriptorString.c_str()));
                    if(!datasetElement) continue;
                    if(value.size() == 0) continue;

                    datasetElement->value(xml.allocate_string(value.c_str()));
                    extensionElement->append_node(datasetElement);
                }
            }
            metaSection->append_node(extensionElement);
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
        xml_node<> * category = nullptr;
        xml_node<> * extension = nullptr;

        //event
        //category = xml.allocate_node(entity->getCategoryAsString().c_str());
        category = xml.allocate_node(node_element, xml.allocate_string(std::string("source").c_str()));

        // the name of entity always comes first
        auto entityName = scene.getEntityName(entity);
        xml_node<> * name = xml.allocate_node(node_element, "name", xml.allocate_string(entityName.c_str())); // entity name

        category->append_node(name);
    
        // packaging - combining element and text (value)
        xml_node<> * element = xml.allocate_node(node_element, xml.allocate_string(meta->getDescriptorAsString().c_str()), xml.allocate_string(meta->getValueAsString().c_str()));
        
        std::string relevantExtension = sdSpec::extensionToString(sdSpec::getExtensionOfDescriptor(meta->getDescriptor()));
        
        if(relevantExtension == "core"){
            category->append_node(element);
            metaSection->append_node(category);
        }else{
            
            if( (entity != previousEntity ) || (previousExtension != relevantExtension)){
                // if different entity, time, or extension from the previous put extension tag
                extension = xml.allocate_node(node_element,xml.allocate_string((relevantExtension).c_str()));
            }
            extension->append_node(element);
            category->append_node(extension);
            metaSection->append_node(category);
        }
        
        previousEntity = entity; // store current name in order to avoid the dupplication.
        previousExtension = relevantExtension;
    });

    // add ordering
    metaSection->append_node(createOrderingSection(xml, scene));

    return metaSection;
}

xml_node<> * sdXMLSaver::createExtentionSection(xml_document<> &xml, const sdScene &scene ){
    size_t num = scene.getNumberOfActivatedExtensions();
    if(num == 0) return nullptr;
        
    xml_node<> * extensions = xml.allocate_node(node_element, "extensions");
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
    extensions->value(xml.allocate_string(extString.c_str()));
    return extensions;
}

std::string sdXMLSaver::toString(const sdScene &scene){
    
    xml_document<> xml;
    xml_node<> * spatdif = xml.allocate_node(node_element, "spatdif");
    spatdif->append_attribute(xml.allocate_attribute("version", "0.4"));
    xml.append_node(spatdif);
    spatdif->append_node(sdXMLSaver::createMetaSection(xml, scene));

    std::unordered_map<std::string, sdEntity> entities = scene.getEntities();
    
    // TIME Section
    /* ordered by time */
    if(scene.getOrdering() == EOrdering::SD_TIME){
        auto eventTime = scene.getNextEventTime(-1.0);
        while(eventTime.second){
            
            // add <time>
            spatdif->append_node(xml.allocate_node(node_element, "time", xml.allocate_string(std::to_string(eventTime.first).c_str())));
            for(auto entity : entities){
                auto entityEventsAtTime = entity.second.getEvents(eventTime.first);
                if(entityEventsAtTime.empty()) continue;
                
                xml_node<> * categoryElement = xml.allocate_node(node_element, xml.allocate_string(entity.second.getCategoryAsString().c_str()));
                xml_node<> * nameElement = xml.allocate_node(node_element, "name", xml.allocate_string(entity.first.c_str())); // entity name
                categoryElement->append_node(nameElement);

                xml_node<> * extensionElement = nullptr;
                std::string previousExtension = "";
                for(auto entityEvent : entityEventsAtTime){
                    xml_node<> * eventElement = xml.allocate_node(node_element, xml.allocate_string(entityEvent->getDescriptorAsString().c_str()), xml.allocate_string(entityEvent->getValueAsString().c_str()));
                    std::string relevantExtension = sdSpec::extensionToString(sdSpec::getExtensionOfDescriptor(entityEvent->getDescriptor()));
        
                    if(relevantExtension == "core"){
                        categoryElement->append_node(eventElement);
                    }else{// extension
                        if(previousExtension == relevantExtension){
                            extensionElement->append_node(eventElement);
                        }else{
                            extensionElement = xml.allocate_node(node_element, xml.allocate_string((relevantExtension).c_str()));
                            extensionElement->append_node(eventElement);
                            categoryElement->append_node(extensionElement);
                        }
                    }
                    previousExtension = relevantExtension;
                }
                spatdif->append_node(categoryElement);
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
                spatdif->append_node(xml.allocate_node(node_element, "time", xml.allocate_string(event->getTimeAsString().c_str())));
                
                xml_node<> * category = xml.allocate_node(node_element, xml.allocate_string(entity.getCategoryAsString().c_str()));
                category->append_node(xml.allocate_node(node_element, "name", xml.allocate_string(entityName.c_str())));
                
                xml_node<> * element = xml.allocate_node(node_element, xml.allocate_string(event->getDescriptorAsString().c_str()), xml.allocate_string(event->getValueAsString().c_str()));
                category->append_node(element);
                spatdif->append_node(category);
            }
        }
    }
    std::string xmlString;
    print(std::back_inserter(xmlString), xml, 0);
    return xmlString;
    
}

std::string sdJSONSaver::toString(const sdScene &sdScene){
    return "";
}

std::string sdYAMLSaver::toString(const sdScene &sdScene){
    return "";
}
