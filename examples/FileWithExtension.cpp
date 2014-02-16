/** @file
 *
 * @ingroup spatdiflib-examples
 *
 * @brief Example: Examine all functionalities of the #sdDate class
 *
 * @details
 *
 * @authors Chikashi Miyama, Trond Lossius
 *
 * @copyright Copyright (C) 2013 - 2014 Zurich University of the Arts, Institute for Computer Music and Sound Technology @n
 * This code is licensed under the terms of the "New BSD License" @n
 * http://creativecommons.org/licenses/BSD/
 */


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
    sdEvent* insectMediaLocationEvent = insect->getFirstEvent(SD_MEDIA_LOCATION);
    sdEvent* elephantMediaLocationEvent = elephant->getFirstEvent(SD_MEDIA_LOCATION);
    
    
    cout << "Entity Name:" << insect->getName() << endl;
    cout << "Attached Sound File:" << insectMediaLocationEvent->getValueAsString() << endl;
    cout << "Attached at:" << insectMediaLocationEvent->getTimeAsString() << endl;
    cout << "----" << endl;
    
    cout << "Entity Name:" << elephant->getName() << endl;
    cout << "Attached Sound File:" << elephantMediaLocationEvent->getValueAsString() << endl;
    cout << "Attached at:" << elephantMediaLocationEvent->getTimeAsString() << endl;
    
    string generatedString = sdSaver::XMLFromScene(&scene);
    // save string as a file
    ofstream ofs("turenas_generated.xml");
    ofs << generatedString << endl;
    ofs.close();
    
    return 0;
    
    
}
