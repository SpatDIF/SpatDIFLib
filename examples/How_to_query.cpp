/* this example examines all functionalities of sdDate class */

#include "sdScene.h"
#include "sdLoader.h"

#include <string>
#include <iostream>
#include <fstream>

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
        abort();
    }
    
    // get value pointer from the sdEntityCore cast it and post
    float* pos = static_cast<float*>(voice1->getValue(2.52 , SD_POSITION));
    cout << "position of voice1 at 2.52:" << pos[0] << ' ' << pos[1] << ' ' << pos[2] << endl;
    
    // you can also ask events in a certain time frame.
    set <sdEvent*, sdEventCompare> myEventSet;

    myEventSet = voice1->getValue(0.00, 5.00, SD_POSITION);
    cout << "there are " << myEventSet.size() << " position events within 0.00 and 5.00" << endl;
    
    set <sdEvent*, sdEventCompare> ::iterator it = myEventSet.begin();
    
    int count = 0;
    while (it!=myEventSet.end()) {
        pos = static_cast<float*>((*it)->value);
        cout << "position of event #" << count << ": " << pos[0] << ' ' << pos[1] << ' ' << pos[2] << endl;
        it++;
        count++;
    }
    
    /*** 2. let sdScene handle the query ***/
    // simpler but less efficient
    
    pos = static_cast<float*>(myScene.getValue(string("voice1"), 0.0, SD_POSITION));
    cout << "position of voice1 at 0.0:" << pos[0] << ' ' << pos[1] << ' ' << pos[2] << endl;

    
    
    
    return 0;
}
