//
//  sdLoader.cpp
//

#include <iostream>
#include <string>
#include <sstream>
#include "sdLoader.h"
#include "tinyxml2.h"

using namespace std;

sdScene sdLoader::sceneFromXML(std::string xmlString){
    using namespace tinyxml2;
    XMLDocument xml;
    xml.Parse(xmlString.c_str());
    sdScene scene;
    sdInfo info;
    
    XMLElement* spatdif = xml.FirstChildElement("spatdif");
    XMLElement* meta = spatdif->FirstChildElement("meta");
    XMLElement* information = meta->FirstChildElement("info");
    
    XMLElement* author = information->FirstChildElement("author");
    XMLElement* host = information->FirstChildElement("host");
    XMLElement* date = information->FirstChildElement("date");
    XMLElement* session = information->FirstChildElement("session");
    XMLElement* location = information->FirstChildElement("location");
    XMLElement* annotation = information->FirstChildElement("annotation");

    if(author){
        info.setAuthor(string(author->GetText()));
    }
    if(date){
        info.setDate(string(date->GetText()));
    }
    if(host){
        info.setHost(string(host->GetText()));
    }
    if(session){
        info.setSession(string(session->GetText()));
    }
    if(location){
        info.setLocation(string(location->GetText()));
    }
    if(annotation){
        info.setAnnotation(string(annotation->GetText()));
    }
    scene.setInfo(info);
    
    //first time tag
    string timeString;
    XMLElement* element = meta->NextSiblingElement();

    while(element){
        
        string tagType = element->Name();
        if(tagType == "time"){
            timeString = string(element->GetText());
        }
        else if(tagType == "source"){
            XMLElement *name = element->FirstChildElement("name");
            string entityName = string(name->GetText());
            sdEntityCore* targetEntity;
            
            targetEntity = scene.getEntity(entityName);
            if(!targetEntity){
                targetEntity = scene.addEntity(entityName);
            }

            XMLElement *descriptor = name->NextSiblingElement();
            while(descriptor) {
                targetEntity->addEvent(timeString, string(descriptor->Name()), string(descriptor->GetText()));
                descriptor = descriptor->NextSiblingElement();
            }
        }
        element = element->NextSiblingElement();
    }
    
    return scene;
}

sdScene sdLoader::sceneFromJSON(std::string jsonString){
    sdScene scene;
    return scene;
}

sdScene sdLoader::sceneFromYAML(std::string yamlString){
    sdScene scene;
    return scene;
}




