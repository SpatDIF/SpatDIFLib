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
