#include "sdScene.h"
#include "sdLoader.h"
#include "sdEntityCore.h"
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
    
    // dump the content of the sdScene
    myScene.dump();
    
    return 0;
}
