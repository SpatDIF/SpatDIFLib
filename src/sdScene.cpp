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

#include <iostream>
#include <sstream>
#include <iomanip>
#include <cfloat>
#include "sdMain.h"


using namespace std;

string sdScene::getEntityName(int index)
{
    vector<string> names = getEntityNames();
    return names[index];
}

vector<string> sdScene::getEntityNames()
{
    vector <string> names;
    vector <sdEntityCore *>::iterator it = entityVector.begin();
    while(it != entityVector.end()){
        sdEntityCore* entity = *it;
        names.push_back(entity->getName());
        it++;
    }
    return names;
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

void sdScene::removeAllEntities(void){
    entityVector.clear();
}

void sdScene::setValue(string name, double time, EDescriptor descriptor,  void* value){
    sdEntityCore* entity = getEntity(name);
    if(!entity){
        entity = addEntity(name);
    }
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

vector<EExtension> sdScene::getActivatedExtensions(){
    return activatedExtensionVector;
}

vector<string> sdScene::getActivatedExtensionsAsStrings(){
    vector<string> strings;
    vector<EExtension>::iterator it = activatedExtensionVector.begin();
    while(it != activatedExtensionVector.end()){
        strings.push_back(extensionToString(*it));
        it++;
    }
    return strings;
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

bool sdScene::isExtensionActivated(string extensionString){
    EExtension extension = stringToExtension(extensionString);
    return isExtensionActivated(extension);
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
            reportVector.push_back(report);
        }
        it++;
    }
    return reportVector;
}

vector<sdReport> sdScene::getPreviousEventSetsFromAllEntities(double time){
    vector<sdReport> reportVector;
    vector<sdEntityCore*>::iterator it = entityVector.begin();
    while (it != entityVector.end()) {
        sdReport report;
        sdEntityCore* entityCore = *it;
        report.entity = entityCore;
        report.eventSet= entityCore->getPreviousEventSet(time);
        if(report.eventSet.size() != 0){
            reportVector.push_back(report);
        }
        it++;
    }
    return reportVector;
}

vector<sdReport> sdScene::getFirstEventSetsFromAllEntities(){
    return getNextEventSetsFromAllEntities(-1.0);
}

vector<sdReport> sdScene::getLastEventSetsFromAllEntities(){
    return getPreviousEventSetsFromAllEntities(DBL_MAX);
}

vector<sdReport> sdScene::getNextEventSets(double time){
    // collect next events from all entities, compare them and return the soonest events 
    vector<sdReport> reportVector = getNextEventSetsFromAllEntities(time);
    vector<sdReport> soonestReportVector;
    if(reportVector.empty()){
        return soonestReportVector;
    }

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

double sdScene::getNextEventTime(double time){
    vector<sdReport> reportVector = getNextEventSets(time);
    if(reportVector.empty()){
        return -1.0;
    }
    sdReport report = reportVector[0];
    multiset<sdEvent*, sdEventCompare> eventSet = report.eventSet;
    multiset<sdEvent*, sdEventCompare>::iterator it = eventSet.begin();
    return (*it)->getTime();
}

double sdScene::getDeltaTimeToNextEvent(double time){
    return getNextEventTime(time) - time;
}

vector<sdReport> sdScene::getFirstEventSets(){
    return getNextEventSets(-1.0);
}

vector<sdReport> sdScene::getEventSetsFromAllEntities(double time){
    vector<sdReport> reportVector;
    vector<sdEntityCore*>::iterator it = entityVector.begin();
    while (it != entityVector.end()) {
        sdEntityCore* entityCore = *it;
        sdReport report;
        report.entity = entityCore;
        report.eventSet= entityCore->getEventSet(time);
        if(report.eventSet.size() != 0){
            reportVector.push_back(report);
        }
        it++;
    }
    return reportVector;
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


string sdScene::dump(bool consoleOut){
    using namespace std;
    ostringstream os;
    
    os << "---------- Meta ----------"<< endl;
    os << "author:" << info.getAuthor() <<endl;
    os << "annotaion:" <<  info.getAnnotation() <<endl;
    os << "ordering:" <<  getOrderingAsString()<<endl;
    os << "extensions:" << activatedExtensionVector.size() << endl;
    
    os << "number of entity used:" <<  getNumberOfEntities()<<endl;

    os << "---------- Entities --------" << endl;
    vector <sdEntityCore*>::iterator it = entityVector.begin();
    while( it != entityVector.end() )
	{
		string entityName = (*it)->getName();
        string kind = (*it)->getKindAsString();
        os << ">>entity name:" << entityName <<endl;
        os << "kind:" << kind <<endl;
        os << "number of events:" << (*it)->getNumberOfEvents()<< endl;
        multiset <sdEvent*, sdEventCompare> evnts = (*it)->getEventSet();
        multiset <sdEvent*, sdEventCompare>::iterator itt = evnts.begin();

        while(itt != evnts.end()){
            sdEventCore *eventCore = static_cast<sdEventCore*>(*itt);
            os << "["  << eventCore->getTimeAsString() << " " << eventCore->getDescriptorAsString() << " " << eventCore->getValueAsString() << "]" << endl;
            itt++;
            
        }
		it++;
	}
    if(consoleOut){
        cout << os.str() << endl;
    }
    return os.str();
}
