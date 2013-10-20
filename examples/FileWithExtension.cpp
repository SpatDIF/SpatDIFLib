/* this example examines all functionalities of sdDate class */

#include <string>
#include <iostream>
#include <fstream>
#include "sdLoader.h"
#include "sdScene.h"
#include "sdEntityCore.h"

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
    scene.dump();
    return 0;
}
