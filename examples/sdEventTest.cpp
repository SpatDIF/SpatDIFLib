
#include "sdScene.h"
#include <string>
#include <iostream>

int main(void){
    using namespace std;
    
    sdScene scene;
    sdEntityCore * myEntity = scene.addEntity("myEntity");
    sdEntityCore * yourEntity = scene.addEntity("yourEntity");

    // myEntity setup
    double pos[3] = {0.0, 1.0, 2.0};
    double orientation[3] = {1.0, 2.0, 2.0};

    // two different events at the same time
    myEntity->addEvent(5.0, SD_POSITION, pos);
    myEntity->addEvent(5.0, SD_ORIENTATION, orientation);

    // get next event(s) from time index 3.0
    multiset<sdEvent*, sdEventCompare> eventSet = myEntity->getNextEventSet(3.0);
    multiset<sdEvent*, sdEventCompare>::iterator it = eventSet.begin();
    while (it != eventSet.end()) {
        sdEvent* event = *it;
        cout << event->getTime() << ' ' << event->getDescriptorAsString() << ' ' << event->getValueAsString() << endl;
        it++;
    }
    
    // yourEntity setup
    pos[0] = 2.0;
    pos[1] = 3.0;
    pos[2] = 4.0;
    
    // two different events at the same time
    yourEntity->addEvent(9.0, SD_POSITION, pos);
    
    
    // ask Scene the event of all entities
    
    vector<sdReport> reportVector;
    reportVector = scene.getNextEventSetsFromAllEntities(2.0);
    cout << "First events of all entities:" <<  endl;
    vector<sdReport>::iterator rit = reportVector.begin() ;
    while (rit != reportVector.end()) {
        sdReport report = *rit;
        cout << "---Entity:" << report.entity->getName() << endl;
        multiset<sdEvent*, sdEventCompare>::iterator eit = report.eventSet.begin();
        sdEvent* event = *eit;
        cout << "time:" << event->getTime() << endl;
        int i = 0;
        while (eit != report.eventSet.end()) {
            sdEvent* event = *eit;
            cout << "event " << i << ": "<< event->getDescriptorAsString() << ' ' << event->getValueAsString() << endl;
            eit++;
            i++;
        }
        rit++;
    }
    
    cout << "events between 6.0 and 12.0 of the all entities:" <<  endl;
    reportVector = scene.getEventSetsFromAllEntities(6.0, 12.0);
    cout << "number of match:" << reportVector.size() << endl;
    rit = reportVector.begin() ;
    while (rit != reportVector.end()) {
        sdReport report = *rit;
        cout << "---Entity:" << report.entity->getName() << endl;
        multiset<sdEvent*, sdEventCompare>::iterator eit = report.eventSet.begin();
        sdEvent* event = *eit;
        cout << "time:" << event->getTime() << endl;
        int i = 0;
        while (eit != report.eventSet.end()) {
            sdEvent* event = *eit;
            cout << "event " << i << ": "<< event->getDescriptorAsString() << ' ' << event->getValueAsString() << endl;
            eit++;
            i++;
        }
        rit++;
    }
    
    return 0;
}
