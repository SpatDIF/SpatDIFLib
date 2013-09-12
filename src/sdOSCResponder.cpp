
#include "sdOSCResponder.h"
#include <vector>
#include <sstream>

using namespace std;

sdOSCResponder::sdOSCResponder(sdScene *scene){
    sdOSCResponder::scene =scene;
}

vector<string> sdOSCResponder::splitString(const string &str){
    vector<string> res;
    size_t current = 0, found;
    while((found = str.find_first_of('/', current)) != string::npos){
        res.push_back(string(str, current, found - current));
        current = found + 1;
    }
    res.push_back(string(str, current, str.size() - current));
    return res;
}

void sdOSCResponder::forwardOSCMessage(string oscMessage){
    // interpret
    bool withTypeTag;
    string addressPattern, typeTag, argument, temp;
    vector <string>argumentVector;
    istringstream iss(oscMessage);
    
    int count = 0;
    while(iss.good()){
        if(count == 0){
            iss >> addressPattern;
            if (addressPattern[0] != '/') {
                cout << "sdOSCReponder Error: The first element of incoming OSC Message is not an address pattern." << endl;
                return;
            }
            //split into parts
            vector <string>ads = splitString(addressPattern);
            vector <string>::iterator it = ads.begin();
            while (it != ads.end()) {
                cout << *it << endl;
                it++;
            }
            
            
        }else if(count == 1){
            iss >> temp;
            if(temp[0] == ','){ 
                withTypeTag = true;
                
            }else{
                withTypeTag = false;
            }
        }else{
            iss >> argument;
            
        }
        
        count++;
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
            entity = scene->addEntity(entityName, kind);
        }
        
        string descriptor = addressVector[3];
        
        
    }else if(type == "time"){
        istringstream is;
        is.str(argumentVector[0]);
        is >> time;
    }else if(type == "meta"){
        
    }
    

    
}