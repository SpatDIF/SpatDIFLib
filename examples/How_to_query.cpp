/** @file
 *
 * @ingroup spatdiflib-examples
 *
 * @brief Example: How to query a SoatDIF scene
 *
 * @details
 *
 * @authors Chikashi Miyama, Trond Lossius
 *
 * @copyright Copyright © 2013 by ??? @n
 * This code is licensed under the terms of the "New BSD License" @n
 * http://creativecommons.org/licenses/BSD/
 */

#include <string>
#include <iostream>
#include <fstream>

#include "sdScene.h"
#include "sdLoader.h"

int main(void){
    using namespace std;
    
    ifstream ifs("simple_scene.xml");
    string xmlString;
    
    // load entire file onto a string
    if (ifs.is_open())
    {
        while ( ifs.good() )
        {
            string str;
            getline(ifs,str);
            xmlString.append(str);
        }
        ifs.close();
    }
    // give sdLoader the string and get a sdScene
    sdScene myScene = sdLoader::sceneFromXML(xmlString);

    // there is two ways to ask data
    
    /*** 1. get an entity and ask the entity ***/
    // more complicated but efficient when you need to ask many things
    
    // ask myScene for a pointer to sdEntityCore voice1
   
    sdEntityCore *voice1 = myScene.getEntity(string("voice1"));

    // check the pointer
    if(!voice1){
        cout << "no such entity" << endl;
        
    }
    
    cout << "-- timed query test" << endl;
    // get value pointer from the sdEntityCore cast it and post
    double* pos = static_cast<double*>(voice1->getValue(2.0 , SD_POSITION));
    cout << "position of voice1 at 2.0:" << pos[0] << ' ' << pos[1] << ' ' << pos[2] << endl;
    
    
    cout << "-- sequential query test" << endl;
    // get Event sequentially
    double currentTime = 0.0;
    while (true) {
        sdEvent* sEvent = voice1->getNextEvent(currentTime, SD_POSITION);
        if(sEvent == NULL) break;
        cout << "Position event at:" << sEvent->getTimeAsString() << " value:" << sEvent->getValueAsString() << endl;
        currentTime = sEvent->getTime();
    }
    
    cout << "-- time frame query test" << endl;

    // you can also ask events in a certain time frame.
    multiset <sdEvent*, sdEventCompare> myEventSet;
    myEventSet = voice1->getEventSet(0.00, 5.00, SD_POSITION);
    cout << "there are " << myEventSet.size() << " position events within 0.00 and 5.00" << endl;
    multiset <sdEvent*, sdEventCompare> ::iterator it = myEventSet.begin();
    
    // much easier way to post
    int count = 0;
    while (it!=myEventSet.end()) {
        cout << "position of event #" << count << ": " << (*it)->getValueAsString() << endl;
        it++;
        count++;
    }
    
    cout << "-- utility functions" << endl;
    
    sdEvent* first = voice1->getFirstEvent(SD_POSITION);
    cout << "first position event- time:" << first->getTime() << " value:" << first->getValueAsString() << endl;
    sdEvent* last = voice1->getLastEvent(SD_POSITION);
    cout << "last  position event- time:" << last->getTime() << " value:" << last->getValueAsString() << endl;
    
    // just get last and first time tag
    cout << "first time tag of voice1 :" << voice1->getFirstTimeTag() << endl;
    cout << "last  time tag of voice1 :" << voice1->getLastTimeTag() << endl;

    
    sdEntityCore *voice2 = myScene.getEntity(string("voice2"));

    multiset <sdEvent*, sdEventCompare> yourEventSet;
    yourEventSet = voice2->getEventSet(0.0, 8.0);
    
    cout << "all event of " << voice2->getName() << endl;
    it = yourEventSet.begin();
    while (it!=yourEventSet.end()) {
        cout << (*it)->getTimeAsString() << ' ' << (*it)->getDescriptorAsString() << ' ' << (*it)->getValueAsString() << endl;
        it++;
    }
    

    
    /*** 2. let sdScene handle the query ***/
    // simpler but less efficient
    
    pos = static_cast<double*>(myScene.getValue(string("voice1"), 0.0, SD_POSITION));
    cout << "position of voice1 at 0.0:" << pos[0] << ' ' << pos[1] << ' ' << pos[2] << endl;

    
    
    return 0;
}
