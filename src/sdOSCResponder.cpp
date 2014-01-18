
#include "sdConst.h"
#include "sdOSCResponder.h"
#include <vector>
#include <sstream>

using namespace std;

sdOSCResponder::sdOSCResponder(sdScene *scene){
    sdOSCResponder::scene =scene;
    queryTimeString = "0.0";
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

void sdOSCResponder::setQueryTime(double time){
    queryTimeString = doubleToString(time);
}

void sdOSCResponder::forwardOSCMessage(string oscMessage, double time){
    setQueryTime(time);
    forwardOSCMessage(oscMessage);
}

bool sdOSCResponder::checkNumberOfArguments(int expectedNumber, int actualNumber, string command){
    if (actualNumber < expectedNumber) {
        cout << "sdOSCReponder Error: too few arguments for comannd." << command << endl;
        return false;
    }
    else if(actualNumber > expectedNumber){
        cout << "sdOSCReponder Error: too many arguments for comannd." << command << endl;
        return false;
    }
    return true;
}

string sdOSCResponder::forwardOSCMessage(string oscMessage){
    // interpret
    string element, command;
    vector<string> arguments;
    istringstream iss(oscMessage);
    int count = 0;
    EKind kd;
    while(iss.good()){
        iss >> element;
        
        switch(count){
            case 0:{
                //address pattern
                if (element[0] != '/') {
                    cout << "sdOSCReponder Error: The first element of incoming OSC Message is not an address pattern." << endl;
                    return NULL;
                }
                //split into parts
                vector <string>ads = splitString(element);
                ads.erase (ads.begin());
                
                if (ads.size() < 2 ) {
                    cout << "sdOSCResponder Error: The address pattern consists of two few elements." << endl;
                    return NULL;
                }
                
                if(ads[0] != "spatdifcmd"){
                    cout << "sdOSCResponder Error: The address pattern of the received message does not begin with \"spatdifcmd\"." << endl;
                    return NULL;
                }
                
                command = ads[1];
                
                break;
            }
            case 1:{
                if (element[0] == ',') {  // with type tag, just ignore
                    iss >> element;
                }
            }
            default:{
                arguments.push_back(element); // copy all arguments to vector
            }
        }
        count++;
    }


    if( command == "setQueryTime"){

        if(!checkNumberOfArguments( 1, arguments.size() ,command)){
            return "/spatdif/error";
        }

        queryTimeString = arguments[0];

        return "/spatdif/time " + queryTimeString;
        
    }else if(command == "getPosition"){
        checkNumberOfArguments( 1, arguments.size() ,command);
        sdEntityCore *entity = scene->getEntity(arguments[0]);
        if (!entity) {
            cout << "sdOSCResponder Error: no such entity " << arguments[0] << endl;
            return "/spatdif/error";
        }
        string returnMessage;
        sdEvent* event = entity->getEvent(stringToDouble(queryTimeString), SD_POSITION);
        if (!event) {
            cout << "sdOSCResponder Error: no such event " <<  endl;
            return "/spatdif/error";
        }
        returnMessage = "/spatdif/source/" + arguments[0] + "/position " + event->getValueAsString();
        return returnMessage;
    }
    
    return NULL;
    
    
}
