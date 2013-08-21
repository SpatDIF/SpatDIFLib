//
//  sdLoader.cpp
//

#include <iostream>
#include <string>
#include <sstream>
#include "sdLoader.h"
#include "sdConverter.h"
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
    float time;
    XMLElement* element = meta->NextSiblingElement();

    while(element){
        
        string tagType = element->Name();
        if(tagType == "time"){
            string timeString = element->GetText();
            istringstream is;
            is.str(timeString);
            is >> time;
        }
        else if(tagType == "source"){
            XMLElement *name = element->FirstChildElement("name");
            string entityName = string(name->GetText());
            sdEntityCore* targetEntity;
            
            targetEntity = scene.getEntity(entityName);
            if(!targetEntity){
                targetEntity = scene.addEntity(entityName);
            }

            XMLElement *descriptor = element->FirstChildElement();
            while(descriptor) {
                //let the entity spawn an event
                EDescriptor desc = (EDescriptor)sdConverter::convert(descriptor->Name());
                switch(desc){
                    case SD_TYPE:
                    {
                        string type = string(descriptor->GetText());
                        break;
                    }
                    case SD_PRESENT:
                    {
                        bool present = string(descriptor->GetText())  == "true" ? true : false;
                        targetEntity->addEvent(time, SD_PRESENT, static_cast<void*>(&present));
                        break;
                    }
                    case SD_POSITION:
                    {
                        istringstream is(descriptor->GetText());
                        float *position = new float[3];
                        is >> position[0] >> position[1] >> position[2];
                        targetEntity->addEvent(time, SD_POSITION, static_cast<void*>(position));
                        break;
                    }
                    case SD_ORIENTATION:
                    {
                        istringstream is(descriptor->GetText());
                        float *orientation = new float[3];
                        is >> orientation[0] >> orientation[1] >> orientation[2];
                        targetEntity->addEvent(time, SD_ORIENTATION, static_cast<void*>(orientation));
                        break;
                    }
                }
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




