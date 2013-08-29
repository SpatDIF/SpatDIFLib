
#include "sdOSCResponder.h"
#include <list>

using namespace std;

sdOSCResponder::sdOSCResponder(sdScene *scene){
    sdOSCResponder::scene =scene;
}

void sdOSCResponder::forwardOSCMessage(string oscMessage){
    // interpret
    
    string addressPattern, typeTag, arguments;
    istringstream iss(oscMessage);
    iss >> addressPattern;
    
    // separate address Pattern to segments
    vector<string> argumentVector;
    size_t pos = 0, offset = 1;
    while (pos != string::npos) {
        int len;
        pos  = addressPattern.find('/',offset);
        len = pos - offset;
        argumentVector.push_back(addressPattern.substr(offset, len));
        offset = pos+1;
    }
    
    // if it's not a spatdif message
    if(argumentVector[0] != "spatdif") return;
    
    // check type of message
    string type = argumentVector[1];
    if(type == "source"){
    }else if(type == "sink"){
    }else if(type == "time"){
        
    }else if(type == "meta"){
        
    }
    

    
}