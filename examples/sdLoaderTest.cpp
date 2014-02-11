/** @file
 *
 * @ingroup spatdiflib-examples
 *
 * @brief Example: Test #sdLoader
 *
 * @details
 *
 * @authors Chikashi Miyama, Trond Lossius
 *
 * @copyright Copyright © 2013 by ??? @n
 * This code is licensed under the terms of the "New BSD License" @n
 * http://creativecommons.org/licenses/BSD/
 */


#include "sdScene.h"
#include "sdLoader.h"
#include "sdEntityCore.h"
#include <string>
#include <iostream>
#include <fstream>

int main(void){
    using namespace std;
    
    ifstream ifsXML("simple_scene.xml");
    ifstream ifsJSON("simple_scene.json");

    string xmlString, jsonString;


    // load entire file onto a string
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
    
    if (ifsJSON.is_open())
    {
        while ( ifsJSON.good() )
        {
            string str;
            getline(ifsJSON,str);
            jsonString.append(str);
        }
        ifsJSON.close();
    }
    
    // give sdLoader the XMLstring and get a sdScene
    cout << "scene from XML" << endl;
    sdScene xmlScene = sdLoader::sceneFromXML(xmlString);
    xmlScene.dump();
    
    // give sdLoader the JSONstring and get a sdScene, The result should be identical
    cout << "scene from json" << endl;
    sdScene jsonScene = sdLoader::sceneFromJSON(jsonString);
    jsonScene.dump();
    
    return 0;
}
