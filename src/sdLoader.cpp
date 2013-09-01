//
//  sdLoader.cpp
//

#include <iostream>
#include <string>
#include <sstream>
#include "sdLoader.h"
#include "tinyxml2.h"
#include "libjson.h"
#include "JSONNode.h"
#include "JSONOptions.h"

using namespace std;

sdScene sdLoader::sceneFromXML(string xmlString){
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
	else if(tagType == "sink"){
	    //to be coded
	}
        element = element->NextSiblingElement();
    }
    
    return scene;
}

sdScene sdLoader::sceneFromJSON(string jsonString){
    JSONNode json;
    sdScene scene;
    sdInfo info;
    
    json = libjson::parse(jsonString);
    JSONNode::iterator it = json.find("spatdif");
    if(it != json.end()){
	JSONNode spatdif = *it;
	JSONNode::iterator iit = spatdif.find("meta");
	string timeString;

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
		    iiit++;
		}
		scene.setInfo(info);
	    }else if(iit->name() == "time"){
		JSONNode timeNode = *iit;
		timeString = timeNode.as_string();
	    }else if(iit->name() == "source"){
		JSONNode source = *iit;
		JSONNode::iterator iiit = source.begin();
		sdEntityCore* targetEntity;
		
		while(iiit != source.end()){
		    if(iiit ->name() == "name"){
			targetEntity = scene.getEntity(iiit->as_string());
			if(!targetEntity){
			    targetEntity = scene.addEntity(iiit->as_string());
			}
		    }
		    else{

			targetEntity->addEvent(timeString, iiit->name(), iiit->as_string());
		    }
		    iiit++;
		}	

	    }else if(iit->name() == "sink"){
		//to be coded 
	    }
	    
	    iit++;
	}
    };
    return scene;
}

sdScene sdLoader::sceneFromYAML(string yamlString){
    sdScene scene;
    return scene;
}




