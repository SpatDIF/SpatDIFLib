
#include "sdOSCResponder.h"
#include <vector>
#include <sstream>

using namespace std;

sdOSCResponder::sdOSCResponder(sdScene *scene){
    sdOSCResponder::scene =scene;
}

void sdOSCResponder::forwardOSCMessage(string oscMessage){
    // interpret
    
    string addressPattern, typeTag, argument;
    vector <string>argumentVector;
    istringstream iss(oscMessage);
    iss >> addressPattern;
    iss >> typeTag; // type tag will be ignored
    
    if(typeTag[0] != ','){
        // typeTag ommited
        argumentVector.push_back(typeTag);
    }
    
    iss >> argument;
    while(argument){
        argumentVector.push_back(argument);
        iss >> argument;
    }
    
    // separate address Pattern to segments
    vector<string> addressVector;
    size_t pos = 0, offset = 1;
    while (pos != string::npos) {
        int len;
        pos  = addressPattern.find('/',offset);
        len = pos - offset;
        addressVector.push_back(addressPattern.substr(offset, len));
        offset = pos+1;
    }
    
    // if it's not a spatdif message
    if(addressVector[0] != "spatdif") return;
    
    // check type of message
    string type = addressVector[1];
    if(type == "source" || type == "sink"){
        
        string entityName = addressVector[2];
        sdEntity* entity = scene-> getEntity(entityName);
        if(!entity){
            EKind kind = type == "source" ? SD_SOURCE : SD_SINK;
            entity = scene.addEntity(entityName, kind);
        }
        
        string descriptor = addressVector[3];
        
        
    }else if(type == "time"){
        istringstream is;
        is.str(argumentVector[0]);
        is >> time;
    }else if(type == "meta"){
        
    }
    

    
}