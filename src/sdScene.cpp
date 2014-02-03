//
//  sdScene.cpp
//

#include <iostream>
#include <sstream>
#include <iomanip>
#include "sdConst.h"
#include "sdScene.h"
#include "sdEntityExtensionMedia.h"

using namespace std;

string sdScene::getEntityName(int index)
{
    int cnt = 0;
    vector <sdEntityCore*>::iterator it = entityVector.begin();
    
    while( it != entityVector.end()) {
        if(cnt == index){
            return (*it)->getName();
        }
        it++;
        cnt++;
    }
    return NULL;
}

sdEntityCore* sdScene::getEntity(string name){
    vector <sdEntityCore*>::iterator it = entityVector.begin();
    while( it != entityVector.end() ){
		string entityName = (*it)->getName();
        if(entityName == name){
            return *it;
        }
	it++;
    }
    return NULL;
}

unsigned int sdScene::getNumberOfEntities(void){
    return entityVector.size();
}


sdEntityCore* sdScene::addEntity(string name, EKind kind){
    
    sdEntityCore* ent = getEntity(name);
    // if already exist just return the existing one
    if(ent){
        return ent;
    }
    
    sdEntityCore* entityCore = new sdEntityCore(name, kind); // private constructor

    // attach extensions to the newly created sdEntityCore.
    vector <EExtension> ::iterator it = activatedExtensionVector.begin();


    while(it != activatedExtensionVector.end()){
        entityCore->addExtension(*it);
        it++;
    }
    
    // insert the entity in the vector
    entityVector.push_back(entityCore);
    
    // return the pointer
    return entityCore;
}

void sdScene::removeEntity(string name){
    sdEntityCore* entity = getEntity(name);
    if(entity){
        removeEntity(entity);
    }
}

void sdScene::removeEntity(sdEntityCore *entity){
    vector <sdEntityCore*>::iterator it = entityVector.begin();
    while (it != entityVector.end()) {
        if(*it == entity){
            it = entityVector.erase(it);
            return;
        }
        it++;
    }
}

void sdScene::setValue(string name, double time, EDescriptor descriptor,  void* value){
    sdEntityCore* entity = getEntity(name);
    entity->addEvent(time, descriptor, value);
}

void* sdScene::getValue(string name, double time, EDescriptor descriptor){
    sdEntityCore* entity = getEntity(name);
    if (!entity) {
        return NULL;
    }
    return entity->getValue(time, descriptor);
}

int sdScene::getNumberOfActivatedExtensions(void){
    return activatedExtensionVector.size();
}



EExtension sdScene::getActivatedExtension(int index){
    return activatedExtensionVector[index];
}


void sdScene::addExtension(EExtension extension){
    vector <EExtension>::iterator pos;
    pos= find(activatedExtensionVector.begin(),
         activatedExtensionVector.end(),
         extension);

    //duplication check
    if(pos != activatedExtensionVector.end()){
        cout << "sdScene Error: Extension already activated." << endl;
        return;
    }

    activatedExtensionVector.push_back(extension);
    vector <sdEntityCore*>::iterator it = entityVector.begin();
    while (it != entityVector.end()) {
        sdEntityCore* entityCore = *it;
        entityCore->addExtension(extension);
        it++;
    }
    
}

void sdScene::addExtension(string extension){
    EExtension ext = stringToExtension(extension);
    if(ext == SD_EXTENSION_ERROR){
        cout << "sdScene Error: No such extension." << endl;
        return;
    }
    addExtension(ext);
}

bool sdScene::isExtensionActivated(EExtension extension){
    vector <EExtension>::iterator it = activatedExtensionVector.begin();
    while(it != activatedExtensionVector.end()){
        EExtension et = *it;
        if(et == extension){
            return true;
        }
        it++;
    }
    return false;
}

void sdScene::removeExtension(EExtension extension){

    vector <EExtension>::iterator it = activatedExtensionVector.begin();
    while (it != activatedExtensionVector.end()) {
        if(*it == extension){
            it = activatedExtensionVector.erase(it);
            return;
        }
        it++;
    }
    cout << "sdScene Error: No such activated extension." << endl;

}

void sdScene::removeExtension(string extension){
    EExtension ext = stringToExtension(extension);
    if(ext == SD_EXTENSION_ERROR){
        cout << "sdScene Error: No such activated extension." << endl;
    }
    removeExtension(ext);
}

vector<sdReport> sdScene::getNextEventSetsFromAllEntities(double time){
    vector<sdReport> reportVector;
    vector<sdEntityCore*>::iterator it = entityVector.begin();
    while (it != entityVector.end()) {
        sdReport report;
        sdEntityCore* entityCore = *it;
        report.entity = entityCore;
        report.eventSet= entityCore->getNextEventSet(time);
        if(report.eventSet.size() != 0){
            multiset<sdEvent*, sdEventCompare>::iterator it=report.eventSet.begin();
            sdEvent* firstEvent = *it;
            reportVector.push_back(report);
        }
        it++;
    }
    return reportVector;
}

vector<sdReport> sdScene::getFirstEventSetsFromAllEntities(){
    return getNextEventSetsFromAllEntities(-1.0);
}

vector<sdReport> sdScene::getNextEventSets(double time){
    vector<sdReport> reportVector = getNextEventSetsFromAllEntities(time);
    vector<sdReport> soonestReportVector;
    
    vector<sdReport>::iterator it = reportVector.begin();
    double soonestEventTime = (*((*it).eventSet.begin()))->getTime();

    // look for the soonest event
    while (it != reportVector.end()) {
        sdReport report = *it;
        multiset<sdEvent*, sdEventCompare>::iterator rit=report.eventSet.begin();
        while (rit != report.eventSet.end()) {
            sdEvent* event = *rit;
            if( soonestEventTime > event->getTime()){
                soonestEventTime = event->getTime();
            }
            rit++;
        }
        it++;
    }
    // include entities and events if events have soonestEventTime
    
    it = reportVector.begin();
    while (it != reportVector.end()) {
        sdReport report = *it;
        if(soonestEventTime == (*((*it).eventSet.begin()))->getTime()){
            soonestReportVector.push_back(report);
        }
        it++;
    }
    return soonestReportVector;
}

vector<sdReport> sdScene::getFirstEventSets(){
    return getNextEventSets(-1.0);
}

vector<sdReport> sdScene::getEventSetsFromAllEntities(double start, double end){
    vector<sdReport> reportVector;
    vector<sdEntityCore*>::iterator it = entityVector.begin();
    while (it != entityVector.end()) {
        sdEntityCore* entityCore = *it;
        sdReport report;
        report.entity = entityCore;
        report.eventSet= entityCore->getEventSet(start, end);
        if(report.eventSet.size() != 0){
            reportVector.push_back(report);
        }
        it++;
    }
    return reportVector;
}


void sdScene::dump(void){
    using namespace std;
    cout << "---------- Meta ----------"<< endl;
    cout << "author:" << info.getAuthor() <<endl;
    cout << "annotaion:" <<  info.getAnnotation() <<endl;
    cout << "ordering:" <<  getOrderingAsString()<<endl;
    cout << "extensions:" << activatedExtensionVector.size() << endl;
    
    cout << "number of entity used:" <<  getNumberOfEntities()<<endl;

    cout << "---------- Entities --------" << endl;
    vector <sdEntityCore*>::iterator it = entityVector.begin();
    while( it != entityVector.end() )
	{
		string entityName = (*it)->getName();
        string kind = (*it)->getKindAsString();
        cout << ">>entity name:" << entityName <<endl;
        cout << "kind:" << kind <<endl;
        cout << "number of events:" << (*it)->getNumberOfEvents()<< endl;
        multiset <sdEvent*, sdEventCompare> evnts = (*it)->getEventSet();
        multiset <sdEvent*, sdEventCompare>::iterator itt = evnts.begin();

        while(itt != evnts.end()){
            sdEventCore *eventCore = static_cast<sdEventCore*>(*itt);
            cout << "["  << eventCore->getTimeAsString() << " " << eventCore->getDescriptorAsString() << " " << eventCore->getValueAsString() << "]" << endl;
            itt++;
            
        }
        vector <sdEntityExtension*> extensionVector = (*it)->getExtensionVector();
        vector <sdEntityExtension*>::iterator vit = extensionVector.begin();
        
        while(vit != extensionVector.end()){
            sdEntityExtension *extension= (*vit);
            cout << "-extension:" << extension->getExtensionNameAsString() << endl;
            multiset <sdEvent*, sdEventCompare> eevnts = extension->getEventSet();
            multiset <sdEvent*, sdEventCompare>::iterator eitt = eevnts.begin();
            
            while(eitt != eevnts.end()){
                sdEvent* extensionEvent = *eitt;
                cout << "["  << extensionEvent->getTimeAsString() << " " << extensionEvent->getDescriptorAsString() << " " << extensionEvent->getValueAsString() << "]" << endl;
                eitt++;
            }
            
            vit++;
        }
		it++;
	}
}
