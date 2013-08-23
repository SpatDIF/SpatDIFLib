//
//  sdSaver.cpp
//

#include <vector>
#include <iostream>
#include <sstream>
#include "sdEntity.h"
#include "sdScene.h"
#include "sdSaver.h"
#include "tinyxml2.h"

using namespace std;

string sdSaver::XMLFromScene(sdScene *scene){
    using namespace tinyxml2;
    
    XMLDocument xml;
 	XMLDeclaration* decl = xml.NewDeclaration();
	decl->SetValue("xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"");
	xml.InsertEndChild(decl);
    
    // meta section
    XMLElement* meta = xml.NewElement("meta");
    xml.InsertEndChild(meta);
    
    XMLElement* info = xml.NewElement("info");
    meta->InsertEndChild(info);
    
    sdInfo information = scene->getInfo();
    
    XMLElement* author = xml.NewElement("author");
    XMLText* authorText = xml.NewText(information.getAuthor().c_str());
    author->InsertEndChild(authorText);
    info->InsertEndChild(author);
   
    XMLElement* host = xml.NewElement("host");
    XMLText* hostText = xml.NewText(information.getHost().c_str());
    host->InsertEndChild(hostText);
    info->InsertEndChild(host);
    
    XMLElement* date = xml.NewElement("date");
    XMLText* dateText = xml.NewText(information.getDateAsString().c_str());
    date->InsertEndChild(dateText);
    info->InsertEndChild(date);
    
    XMLElement* location = xml.NewElement("location");
    XMLText* locationText = xml.NewText(information.getLocation().c_str());
    location->InsertEndChild(locationText);
    info->InsertEndChild(location);
 
    XMLElement* session = xml.NewElement("session");
    XMLText* sessionText = xml.NewText(information.getSession().c_str());
    session->InsertEndChild(sessionText);
    info->InsertEndChild(session);
    
    XMLElement* annotation = xml.NewElement("annotation");
    XMLText* annotationText = xml.NewText(information.getAnnotation().c_str());
    annotation->InsertEndChild(annotationText);
    info->InsertEndChild(annotation);
    
    // time section
    vector<sdEntityCore*> entityVector = scene->getEntityVector();
    vector<sdEntityCore*>::iterator it = entityVector.begin();
    set<sdGlobalEvent, sdGlobalEventCompare> allEventSet;
    
    while(it != entityVector.end()){
        sdEntityCore *entity = *it;
        set<sdEvent*, sdEventCompare> eventSet = entity->getEventSet();
        set<sdEvent*, sdEventCompare>::iterator iit =eventSet.begin();
        while(iit != eventSet.end()){
            sdEvent* event = *iit;
            sdGlobalEvent globalEvent;
            
            globalEvent.set(event->getTime(), event->getDescriptor(), event->getValue());
            globalEvent.setEntityName(entity->getName());
            globalEvent.setKind(entity->getKind());
            
            float* val = static_cast<float*>(event->getValue());
            
            allEventSet.insert(globalEvent); // gather pointer to all existing instances of sdEvent
            ++iit;
        }
        ++it;
    }
    
    set<sdGlobalEvent, sdGlobalEventCompare>::iterator eit = allEventSet.begin();
    while(eit != allEventSet.end()){
        sdGlobalEvent event = *eit;
        
        XMLElement* time = xml.NewElement("time");
        XMLText* timeText = xml.NewText(event.getTimeAsString().c_str());
        time->InsertEndChild(timeText);
        xml.InsertEndChild(time);

        XMLElement* kind;
        kind = xml.NewElement(event.getKindAsString().c_str());
        XMLElement* name = xml.NewElement("name");
        XMLText* nameText = xml.NewText(event.getEntityName().c_str());
        name->InsertEndChild(nameText);
        kind->InsertEndChild(name);
        
        XMLElement* element = xml.NewElement(event.getDescriptorAsString().c_str());
        XMLText* text = xml.NewText(event.getValueAsString().c_str());
        element->InsertEndChild(text);
        kind->InsertEndChild(element);
        
        xml.InsertEndChild(kind);
        ++eit;
    }

    XMLPrinter printer;
    xml.Print(&printer);
    return string(printer.CStr());
    
}

string sdSaver::JSONFromScene( sdScene *sdScene){
    return NULL;
}

string sdSaver::YAMLFromScene( sdScene *sdScene){
    return NULL;
}