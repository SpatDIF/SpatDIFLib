
#include "sdConst.h"
#include "sdOSCResponder.h"
#include <vector>
#include <sstream>

using namespace std;

sdOSCResponder::sdOSCResponder(sdScene *scene){
    sdOSCResponder::scene =scene;
    currentTimeString = "0.0";
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

void sdOSCResponder::setCurrentTime(double time){
    currentTimeString = doubleToString(time);
}

void sdOSCResponder::forwardOSCMessage(string oscMessage, double time){
    setCurrentTime(time);
    forwardOSCMessage(oscMessage);
}

void sdOSCResponder::forwardOSCMessage(string oscMessage){
    // interpret
    bool timeFlag = false;
    string addressPattern, typeTag, temp, kind, entityName, descriptor, argument, arguments;
    istringstream iss(oscMessage);
    int count = 0;
    EKind kd;
    while(iss.good()){
        if(count == 0){
            iss >> addressPattern;
            if (addressPattern[0] != '/') {
                cout << "sdOSCReponder Error: The first element of incoming OSC Message is not an address pattern." << endl;
                return;
            }
            //split into parts
            vector <string>ads = splitString(addressPattern);
            ads.erase (ads.begin());
            
            if(ads[0] != "spatdif"){
                cout << "sdOSCResponder Error: The address pattern of the received message does not begin with \"spatdif\"." << endl;
                return;
            }
            kind = ads[1];
            if (kind == "source") {
                kd = SD_SOURCE;
                entityName = ads[2];
                descriptor = ads[3];
            }else if(kind == "sink"){
                kd = SD_SINK;
                entityName = ads[2];
                descriptor = ads[3];
            }else if(kind == "time"){
                timeFlag = true;
            }else{
                cout << "sdOSCResponder Error: Kind of given address pattern should be  \"source\", \"sink\", or \"time\"." << endl;
                return;
            }
        }else if(count == 1){
            iss >> temp;
            if(temp[0] != ','){
                arguments = temp;
                arguments += " ";
            }
        }else{
            iss >> argument;
            arguments += argument;
            arguments += " ";
        }
        count++;
    }
    
    // store in the library
    if (timeFlag) {
        currentTimeString = arguments;
    }
    sdEntityCore* ent = scene->getEntity(entityName);
    if(!ent){
        ent = scene->addEntity(entityName, kd);
    }
    cout << "addEvent" << entityName << arguments << endl;
    ent->addEvent(currentTimeString, descriptor, arguments);

}
