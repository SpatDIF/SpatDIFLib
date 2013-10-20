/* this example examines all functionalities of sdDate class */

#include <string>
#include <iostream>
#include <fstream>
#include "sdMain.h"

int main(void){
    
    using namespace std;
    ifstream ifsXML("turenas_insect.xml");
    string xmlString;
    
    if (ifsXML.is_open())
    {
        while ( ifsXML.good() )
        {
            string str;
            getline(ifsXML,str);
            xmlString.append(str);
        }
        ifsXML.close();
    }
    
    sdScene scene = sdLoader::sceneFromXML(xmlString);
    //scene.dump();
    
    // here how you get a pointer to two source entities
    sdEntityCore* insect = scene.getEntity("insect");
    sdEntityCore* elephant = scene.getEntity("elephant");
    
    // now you can ask these entities about their attached extensions
    sdEvent* insectMediaLocationEvent = insect->getNextEvent(-1.0, SD_MEDIA_LOCATION);
    sdEvent* elephantMediaLocationEvent = elephant->getNextEvent(-1.0, SD_MEDIA_LOCATION);
    
    cout << "Entity Name:" << insect->getName() << endl;
    cout << "Attached Sound File:" << insectMediaLocationEvent->getValueAsString() << endl;
    cout << "Attached at:" << insectMediaLocationEvent->getTimeAsString() << endl;
    cout << "----" << endl;
    
    cout << "Entity Name:" << elephant->getName() << endl;
    cout << "Attached Sound File:" << elephantMediaLocationEvent->getValueAsString() << endl;
    cout << "Attached at:" << elephantMediaLocationEvent->getTimeAsString() << endl;
    return 0;
}
