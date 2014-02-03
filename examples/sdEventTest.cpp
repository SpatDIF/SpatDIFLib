
#include "sdScene.h"
#include <string>
#include <iostream>

int main(void){
    using namespace std;
    
    // create a scene and add two entities
    sdScene scene;
    sdEntityCore * myEntity = scene.addEntity("myEntity");
    sdEntityCore * yourEntity = scene.addEntity("yourEntity");

    // myEntity setup
    double mypos[3] = {0.0, 1.0, 2.0};
    double myori[3] = {1.0, 2.0, 3.0};
    myEntity->addEvent(5.0, SD_POSITION, mypos);
    myEntity->addEvent(5.0, SD_ORIENTATION, myori);

    // yourEntity setup
    double yourpos[3] = {5.0, 4.0, 3.0};
    double yourori[3] = {10.0, 9.0, 8.0};
    yourEntity->addEvent(10.0, SD_POSITION, yourpos);
    yourEntity->addEvent(5.0, SD_ORIENTATION, yourori);
    
    multiset<sdEvent*, sdEventCompare> eventSet;
    multiset<sdEvent*, sdEventCompare>::iterator it;
    vector<sdReport> reports;
    vector<sdReport>::iterator rit;
    {
        cout << "===== getting event(s) at specfied time =====" << endl;
     
        // 1. specified entity and descriptor
        cout << "Time:5.0 Entity:myEntity Descriptor:position" << endl;
        sdEvent *myEvent = myEntity->getEvent(5.0, SD_POSITION);
        cout << "value: " << myEvent->getValueAsString() << endl;
        cout << "=====" << endl;
        
        // 2. specified entity
        cout << "Time:5.0 Entity:myEntity " << endl;
        eventSet = myEntity->getEventSet(5.0);
        it = eventSet.begin();
        while (it != eventSet.end()) {
            sdEvent* event = *it;
            cout << event->getDescriptorAsString() << ' ' << event->getValueAsString() << endl;
            it++;
        }
        cout << "=====" << endl;
        
        // 3. all events in the scene regardless of entity
        cout << "Time:5.0" << endl;
        reports = scene.getEventSetsFromAllEntities(5.0);
        rit = reports.begin();
        while (rit != reports.end()) {
            sdReport report = *rit;
            cout << "entity:" << report.entity->getName() << endl;
            it = report.eventSet.begin();
            while (it != report.eventSet.end()) {
                sdEvent* event = *it;
                cout << event->getDescriptorAsString() << ' ' << event->getValueAsString() << endl;
                it++;
            }
            rit++;
        }
    }
    {
        cout << "===== getting previous event(s)  =====" << endl;
        // 1. specified entity and descriptor
        cout << "Time:7.0 Entity:myEntity Descriptor:position" << endl;
        sdEvent *myEvent = myEntity->getPreviousEvent(7.0, SD_POSITION);
        cout << "value: " << myEvent->getValueAsString() << endl;
        cout << "=====" << endl;
        
        // 2. specified entity
        cout << "Time:3.0 Entity:myEntity " << endl;
        eventSet = myEntity->getNextEventSet(3.0);
        it = eventSet.begin();
        while (it != eventSet.end()) {
            sdEvent* event = *it;
            cout << event->getDescriptorAsString() << ' ' << event->getValueAsString() << endl;
            it++;
        }
        cout << "=====" << endl;
        
        // 3. all events in the scene regardless of entity
        cout << "Time:3.0" << endl;
        reports = scene.getNextEventSetsFromAllEntities(3.0);
        rit = reports.begin();
        while (rit != reports.end()) {
            sdReport report = *rit;
            cout << "entity:" << report.entity->getName() << endl;
            it = report.eventSet.begin();
            while (it != report.eventSet.end()) {
                sdEvent* event = *it;
                cout << event->getDescriptorAsString() << ' ' << event->getValueAsString() << endl;
                it++;
            }
            rit++;
        }
    }
    {
        cout << "===== getting previous event(s)  =====" << endl;
        // 1. specified entity and descriptor
        cout << "Time:7.0 Entity:myEntity Descriptor:position" << endl;
        sdEvent *myEvent = myEntity->getPreviousEvent(7.0, SD_POSITION);
        cout << "value: " << myEvent->getValueAsString() << endl;
        cout << "=====" << endl;
        
        // 2. specified entity
        cout << "Time:7.0 Entity:myEntity " << endl;
        eventSet = myEntity->getPreviousEventSet(7.0);
        it = eventSet.begin();
        while (it != eventSet.end()) {
            sdEvent* event = *it;
            cout << event->getDescriptorAsString() << ' ' << event->getValueAsString() << endl;
            it++;
        }
        cout << "=====" << endl;
        
        // 3. all events in the scene regardless of entity
        cout << "Time:7.0" << endl;
        reports = scene.getPreviousEventSetsFromAllEntities(7.0);
        rit = reports.begin();
        while (rit != reports.end()) {
            sdReport report = *rit;
            cout << "entity:" << report.entity->getName() << endl;
            it = report.eventSet.begin();
            while (it != report.eventSet.end()) {
                sdEvent* event = *it;
                cout << event->getDescriptorAsString() << ' ' << event->getValueAsString() << endl;
                it++;
            }
            rit++;
        }
    }

    
    return 0;
}
