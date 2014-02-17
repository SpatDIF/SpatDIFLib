/** @file
 *
 * @ingroup spatdiflib-examples
 *
 * @brief Example: Test #stEntityCore
 *
 * @details
 *
 * @authors Chikashi Miyama, Trond Lossius
 *
 * @copyright Copyright (C) 2013 - 2014 Zurich University of the Arts, Institute for Computer Music and Sound Technology @n
 * This code is licensed under the terms of the "New BSD License" @n
 * http://creativecommons.org/licenses/BSD/
 */


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
        cout << "Time:5 Entity:myEntity Descriptor:position" << endl;
        sdEvent *myEvent = myEntity->getEvent(5.0, SD_POSITION);
        cout << "value: " << myEvent->getValueAsString() << endl;
        cout << "=====" << endl;
        
        // 2. specified entity
        cout << "Time:5 Entity:myEntity " << endl;
        eventSet = myEntity->getEventSet(5.0);
        it = eventSet.begin();
        while (it != eventSet.end()) {
            sdEvent* event = *it;
            cout << event->getDescriptorAsString() << ' ' << event->getValueAsString() << endl;
            it++;
        }
        cout << "=====" << endl;
        
        // 3. all events in the scene regardless of entity
        cout << "Time:5" << endl;
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
        cout << "===== getting next event(s)  =====" << endl;
        // 1. specified entity and descriptor
        cout << "Time:3 Entity:myEntity Descriptor:position" << endl;
        sdEvent *myEvent = myEntity->getNextEvent(3.0, SD_POSITION);
        cout << "time:" << myEvent->getTime() << " value: " << myEvent->getValueAsString() << endl;
        cout << "=====" << endl;
        
        // 2. specified entity
        cout << "Time:3 Entity:myEntity " << endl;
        eventSet = myEntity->getNextEventSet(3.0);
        it = eventSet.begin();
        while (it != eventSet.end()) {
            sdEvent* event = *it;
            cout << "time:" << event->getTime() << " descriptor:" <<event->getDescriptorAsString() << ' ' << event->getValueAsString() << endl;
            it++;
        }
        cout << "=====" << endl;
        
        // 3. all events in the scene regardless of entity
        cout << "Time:3" << endl;
        reports = scene.getNextEventSetsFromAllEntities(3.0);
        rit = reports.begin();
        while (rit != reports.end()) {
            sdReport report = *rit;
            cout << "entity:" << report.entity->getName() << endl;
            it = report.eventSet.begin();
            while (it != report.eventSet.end()) {
                sdEvent* event = *it;
                cout << "time:" << event->getTime() << " descriptor:" << event->getDescriptorAsString() << ' ' << event->getValueAsString() << endl;
                it++;
            }
            rit++;
        }
    }
    {
        cout << "===== getting previous event(s)  =====" << endl;
        // 1. specified entity and descriptor
        cout << "Time:7 Entity:myEntity Descriptor:position" << endl;
        sdEvent *myEvent = myEntity->getPreviousEvent(7.0, SD_POSITION);
        cout << "time:" << myEvent->getTime() << " value: " << myEvent->getValueAsString() << endl;
        cout << "=====" << endl;
        
        // 2. specified entity
        cout << "Time:7 Entity:myEntity " << endl;
        eventSet = myEntity->getPreviousEventSet(7.0);
        it = eventSet.begin();
        while (it != eventSet.end()) {
            sdEvent* event = *it;
            cout << "time:" << event->getTime() << " descriptor:" <<event->getDescriptorAsString() << ' ' << event->getValueAsString() << endl;
            it++;
        }
        cout << "=====" << endl;
        
        // 3. all events in the scene regardless of entity
        cout << "Time:7" << endl;
        reports = scene.getPreviousEventSetsFromAllEntities(7.0);
        rit = reports.begin();
        while (rit != reports.end()) {
            sdReport report = *rit;
            cout << "entity:" << report.entity->getName() << endl;
            it = report.eventSet.begin();
            while (it != report.eventSet.end()) {
                sdEvent* event = *it;
                cout << "time:" << event->getTime()  << " descriptor:" <<event->getDescriptorAsString() << ' ' << event->getValueAsString() << endl;
                it++;
            }
            rit++;
        }
    }
    {
        cout << "===== getting first event(s)  =====" << endl;
        // 1. specified entity and descriptor
        cout << "Entity:myEntity Descriptor:position" << endl;
        sdEvent *myEvent = myEntity->getFirstEvent(SD_POSITION);
        cout << "time:" << myEvent->getTime() << " value: " << myEvent->getValueAsString() << endl;
        cout << "=====" << endl;
        
        // 2. specified entity
        cout << "Entity:myEntity " << endl;
        eventSet = myEntity->getFirstEventSet();
        it = eventSet.begin();
        while (it != eventSet.end()) {
            sdEvent* event = *it;
            cout << "time:" << event->getTime()  << " descriptor:" << event->getDescriptorAsString() << ' ' << event->getValueAsString() << endl;
            it++;
        }
        cout << "=====" << endl;
        
        // 3. all events in the scene regardless of entity
        reports = scene.getFirstEventSetsFromAllEntities();
        rit = reports.begin();
        while (rit != reports.end()) {
            sdReport report = *rit;
            cout << "entity:" << report.entity->getName() << endl;
            it = report.eventSet.begin();
            while (it != report.eventSet.end()) {
                sdEvent* event = *it;
                cout << "time:" << event->getTime()  << " descriptor:" << event->getDescriptorAsString() << ' ' << event->getValueAsString() << endl;
                it++;
            }
            rit++;
        }
    }
    {
        cout << "===== getting last event(s)  =====" << endl;
        // 1. specified entity and descriptor
        cout << "Entity:myEntity Descriptor:position" << endl;
        sdEvent *myEvent = myEntity->getLastEvent(SD_POSITION);
        cout << "time:" << myEvent->getTime() << " value: " << myEvent->getValueAsString() << endl;
        cout << "=====" << endl;
        
        // 2. specified entity
        cout << "Entity:myEntity " << endl;
        eventSet = myEntity->getLastEventSet();
        it = eventSet.begin();
        while (it != eventSet.end()) {
            sdEvent* event = *it;
            cout << "time:" << event->getTime()  << " descriptor:" << event->getDescriptorAsString() << ' ' << event->getValueAsString() << endl;
            it++;
        }
        cout << "=====" << endl;
        
        // 3. all events in the scene regardless of entity
        reports = scene.getLastEventSetsFromAllEntities();
        rit = reports.begin();
        while (rit != reports.end()) {
            sdReport report = *rit;
            cout << "entity:" << report.entity->getName() << endl;
            it = report.eventSet.begin();
            while (it != report.eventSet.end()) {
                sdEvent* event = *it;
                cout << "time:" << event->getTime()  << " descriptor:" << event->getDescriptorAsString() << ' ' << event->getValueAsString() << endl;
                it++;
            }
            rit++;
        }
    }
    {
      //cout << "===== getting events in the specified time frame =====" << endl;
        // 1. specified entity and descriptor
        //cout << "Entity:myEntity Descriptor:position" << endl;
	//        eventSet = myEntity->getEventSet(3.0, 7.0, SD_POSITION, );
      
        
        // 2. specified entity
//        cout << "Entity:myEntity " << endl;
//        eventSet = myEntity->getLastEventSet();
//        it = eventSet.begin();
//        while (it != eventSet.end()) {
//            sdEvent* event = *it;
//            cout << "time:" << event->getTime()  << " descriptor:" << event->getDescriptorAsString() << ' ' << event->getValueAsString() << endl;
//            it++;
//        }
//        cout << "=====" << endl;
        
        // 3. all events in the scene regardless of entity
//        reports = scene.getLastEventSetsFromAllEntities();
//        rit = reports.begin();
//        while (rit != reports.end()) {
//            sdReport report = *rit;
//            cout << "entity:" << report.entity->getName() << endl;
//            it = report.eventSet.begin();
//            while (it != report.eventSet.end()) {
//                sdEvent* event = *it;
//                cout << "time:" << event->getTime()  << " descriptor:" << event->getDescriptorAsString() << ' ' << event->getValueAsString() << endl;
//                it++;
//            }
//            rit++;
//        }
    }
    
    return 0;
}
