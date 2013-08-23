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
        ostringstream timeStringStream;
        timeStringStream << event.getTime();
        XMLText* timeText = xml.NewText(timeStringStream.str().c_str());
        time->InsertEndChild(timeText);
        xml.InsertEndChild(time);

        XMLElement* kind;

        switch (event.getKind()) {
            case SD_SOURCE:
                kind = xml.NewElement("source");
                break;
            case SD_SINK:
                kind = xml.NewElement("sink");
                break;
        }
        
        XMLElement* name = xml.NewElement("name");
        XMLText* nameText = xml.NewText(event.getEntityName().c_str());
        name->InsertEndChild(nameText);
        kind->InsertEndChild(name);
        
        switch(event.getDescriptor()){
            case SD_POSITION:{
                XMLElement* position;
                XMLText* positionText;

                ostringstream positionString;
                float* pos = static_cast<float*>(event.getValue());
                positionString << pos[0] << ' ' << pos[1] << ' ' << pos[2];
                position = xml.NewElement("position");
                positionText = xml.NewText(positionString.str().c_str());
                position->InsertEndChild(positionText);
                kind->InsertEndChild(position);
                break;
            }
            case SD_PRESENT:{
                XMLElement* present = xml.NewElement("present");
                XMLText* presentText;
                bool* prst = (bool*)event.getValue();
                if(*prst == true){
                    presentText = xml.NewText("true");
                }else{
                    presentText = xml.NewText("false");
                }
                kind->InsertEndChild(presentText);
                break;
            }
        }
        
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