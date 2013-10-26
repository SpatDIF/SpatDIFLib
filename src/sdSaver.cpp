//
//  sdSaver.cpp
//

#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include "sdConst.h"
#include "sdEntity.h"
#include "sdScene.h"
#include "sdSaver.h"
#include "tinyxml2.h"
#include "JSONNode.h"

using namespace std;

string sdGlobalEvent::getKindAsString(void){
    string str;
    switch (kind) {
        case SD_SOURCE:
            str = string("source");
            break;
        case SD_SINK:
            str = string("sink");
            break;
        default:
            break;
    }
    return str;
}

string sdSaver::XMLFromScene(sdScene *scene){
    using namespace tinyxml2;
    
    XMLDocument xml;
 	XMLDeclaration* decl = xml.NewDeclaration();
	decl->SetValue("xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"");
	xml.InsertEndChild(decl);
    
    XMLElement* spatdif = xml.NewElement("spatdif");
	xml.InsertEndChild(spatdif);
    // meta section
    
    XMLElement* meta = xml.NewElement("meta");
    spatdif->SetAttribute("version", "0.3");
    spatdif->InsertEndChild(meta);
    
    XMLElement* info = xml.NewElement("info");
    meta->InsertEndChild(info);
    
    sdInfo information = scene->getInfo();

    string infoStrings[6];
    string elementNameStrings[6];
    
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
            XMLElement* e  = xml.NewElement(elementNameStrings[i].c_str());
            XMLText* t = xml.NewText(infoStrings[i].c_str());
            e->InsertEndChild(t);
            info->InsertEndChild(e);
        }
    }
    
    // check the number of extension
    int num = scene->getNumberOfActivatedExtensions();

    if(num > 0){
        XMLElement* extensions = xml.NewElement("extensions");
        string extString;
        for(int i = 0; i< num; i++){

            EExtension ext = scene->getActivatedExtension(i);
            extString = extString + extensionToString(ext);
        }
        
        XMLText* extensionsText = xml.NewText(extString.c_str());
        extensions->InsertEndChild(extensionsText);
    }
    XMLElement* order = xml.NewElement("ordering");
    XMLText* orderingText = xml.NewText(scene->getOrderingAsString().c_str());
    order->InsertEndChild(orderingText);
    meta->InsertEndChild(order);
    
    // time section
    
    vector<sdEntityCore*> entityVector = scene->getEntityVector();

    /* ordered by time */
    if(scene->getOrdering() == SD_TIME){
        
        // 1. pool all events in globalEvent Set
        vector<sdEntityCore*>::iterator it = entityVector.begin();
        multiset<sdGlobalEvent, sdGlobalEventCompare> allEventSet;
        
        while(it != entityVector.end()){
            sdEntityCore *entity = *it;
            multiset<sdEvent*, sdEventCompare> eventSet = entity->getEventSet();
            multiset<sdEvent*, sdEventCompare>::iterator iit =eventSet.begin();
            while(iit != eventSet.end()){
                sdEvent* event = *iit;
                sdGlobalEvent globalEvent(event, entity->getName(), entity->getKind());                
                allEventSet.insert(globalEvent); // gather pointer to all existing instances of sdEvent
                ++iit;
            }
            ++it;
        }

        // 2. create string
        multiset<sdGlobalEvent, sdGlobalEventCompare>::iterator eit = allEventSet.begin();
        while(eit != allEventSet.end()){
            sdGlobalEvent event = *eit;
            
            XMLElement* time = xml.NewElement("time");
            XMLText* timeText = xml.NewText(event.getEvent()->getTimeAsString().c_str());
            time->InsertEndChild(timeText);
            spatdif->InsertEndChild(time);

            XMLElement* kind;
            kind = xml.NewElement(event.getKindAsString().c_str());
            XMLElement* name = xml.NewElement("name");
            XMLText* nameText = xml.NewText(event.getEntityName().c_str());
            name->InsertEndChild(nameText);
            kind->InsertEndChild(name);
            
            XMLElement* element = xml.NewElement(event.getEvent()->getDescriptorAsString().c_str());
            XMLText* text = xml.NewText(event.getEvent()->getValueAsString().c_str());
            element->InsertEndChild(text);
            kind->InsertEndChild(element);
            
            spatdif->InsertEndChild(kind);
            ++eit;
        }

    }else if(scene->getOrdering() == SD_TRACK){
        // 1. Sort vector by name alphabetically
        
        sort(entityVector.begin(), entityVector.end(), sdEntityCore::sortAlphabetically);
        vector<sdEntityCore*>::iterator it = entityVector.begin();

        while(it != entityVector.end()){
            sdEntityCore *entity = *it;
            multiset<sdEvent*, sdEventCompare> eventSet = entity->getEventSet();
            multiset<sdEvent*, sdEventCompare>::iterator iit = eventSet.begin();

            while(iit != eventSet.end()){
                sdEvent* event = *iit;
                XMLElement* time = xml.NewElement("time");
                XMLText* timeText = xml.NewText(event->getTimeAsString().c_str());
                time->InsertEndChild(timeText);
                spatdif->InsertEndChild(time);
                
                XMLElement* kind;
                kind = xml.NewElement(entity->getKindAsString().c_str());
                XMLElement* name = xml.NewElement("name");
                XMLText* nameText = xml.NewText(entity->getName().c_str());
                name->InsertEndChild(nameText);
                kind->InsertEndChild(name);
                
                XMLElement* element = xml.NewElement(event->getDescriptorAsString().c_str());
                XMLText* text = xml.NewText(event->getValueAsString().c_str());
                element->InsertEndChild(text);
                kind->InsertEndChild(element);
                
                spatdif->InsertEndChild(kind);
                ++iit;
            }
            ++it;
        }
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