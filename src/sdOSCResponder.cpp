/** @file
 *
 * @ingroup spatdiflib
 *
 * @brief
 *
 * @details
 *
 * @authors Chikashi Miyama, Trond Lossius
 *
 * @copyright Copyright (C) 2013 - 2014 Zurich University of the Arts, Institute for Computer Music and Sound Technology @n
 * This code is licensed under the terms of the "New BSD License" @n
 * http://creativecommons.org/licenses/BSD/
 */

#include "sdConst.h"
#include "sdOSCResponder.h"
#include <vector>
#include <sstream>

using namespace std;

sdOSCResponder::sdOSCResponder(void){
    queryTime = 0.0;
    writeTime = 0.0;
}

sdOSCResponder::sdOSCResponder(sdScene *scene){
    sdOSCResponder::scene =scene;
    sdOSCResponder();
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

// low level interface
vector<vector<unsigned char> > sdOSCResponder::forwardOSCMessage(vector<unsigned char> message){
    sdOSCMessage mes(message);
    vector<sdOSCMessage> rMesVector = forwardOSCMessage(mes);
    vector<sdOSCMessage>::iterator it = rMesVector.begin();
    vector<vector<unsigned char> > rRawVector;
    while(it != rMesVector.end()){
        sdOSCMessage m = *it;
        rRawVector.push_back(m.getOSCMessage());
        it++;
    }
    return rRawVector;
}

// high level interface
vector<sdOSCMessage> sdOSCResponder::forwardOSCMessage(sdOSCMessage message){
    vector<sdOSCMessage> returnMessageVector;
    
    string address = message.getAddressAsString();
    if (address[0] != '/') {
        cout << "sdOSCReponder Error: invalid OSC address" << endl;
        returnMessageVector.clear();
        return returnMessageVector;
    }
    
    vector <string>ads = splitString(address);
    ads.erase (ads.begin());
    
    if (ads.size() < 2 ) {
        cout << "sdOSCResponder Error: The address pattern consists of two few elements." << endl;
        return returnMessageVector;
    }
    
    if(ads[0] != "spatdifcmd"){
        cout << "sdOSCReponder Error: invalid command" << endl;
        returnMessageVector.clear();
        return returnMessageVector;
    }
    
    string command = ads[1]; // get the element right after /spatdifcmd
    string action =  command.substr(0, 3); //  check if the client try to set or get

    if(action == "get"){
        returnMessageVector = getAction(command, message);
    }else if(action == "set"){
        setAction(command, message);
    }else{
        if(command == "addEntity"){
            scene->addEntity(message.getArgumentAsString(0));
        }else if(command == "removeEntity"){
            scene->removeEntity(message.getArgumentAsString(0));
        }else if(command == "removeAllEntities"){
            scene->removeAllEntities();
        }else{
            cout << "sdOSCReponder Error: invalid command" << endl;
        }
    }
    return returnMessageVector;
}

vector<sdOSCMessage> sdOSCResponder::getAction(string command, sdOSCMessage message){
    vector<sdOSCMessage> returnMessageVector;
    
    // internal variable
    if(command == "getEventSetsFromAllEntities"){
        vector<sdReport> reports;
        reports = scene->getEventSetsFromAllEntities(queryTime, queryTime+interval); // in the specified region
        vector<sdReport>::iterator rit = reports.begin();
        while(rit != reports.end()){
            sdReport report = *rit;
            sdEntityCore* entity = report.entity;
            multiset<sdEvent*, sdEventCompare>eventSet = report.eventSet;
            multiset<sdEvent*, sdEventCompare>::iterator eit = eventSet.begin();
            while (eit != eventSet.end()) {

                sdEvent *event = *eit;
                sdOSCMessage returnMessage("/spatdif/event");
                returnMessage.appendFloat(static_cast<float>(event->getTime()));
                returnMessage.appendString(entity->getName());
                returnMessage.appendString(event->getDescriptorAsString());
                switch (event->getDescriptor()){
                    case SD_POSITION:
                    case SD_ORIENTATION:{
                        float fpos[3];
                        double* dpos = static_cast<double *>(event->getValue());
                        returnMessage.appendFloats(doublesToFloats(dpos, fpos, 3),3);
                        returnMessageVector.push_back(returnMessage);
                        break;
                    }
                    case SD_PRESENT:{
                        bool *flag = static_cast<bool *>(event->getValue());
                        returnMessage.appendInt(static_cast<int>(*flag));
                        returnMessageVector.push_back(returnMessage);
                        break;
                    }
                    default:{
                        cout << "sdOSCResponder Error: unexpected descriptor" << endl;
                    }

                }
                eit++;
            }
            rit++;
        }
    
    }else if(command == "getQueryTime"){
        sdOSCMessage returnMessage("/spatdif/queryTime");
        returnMessage.appendFloat(static_cast<float>(getQueryTime()));
        returnMessageVector.push_back(returnMessage);
    }else if(command == "getWriteTime"){
        sdOSCMessage returnMessage("/spatdif/writeTime");
        returnMessage.appendFloat(static_cast<float>(getWriteTime()));
        returnMessageVector.push_back(returnMessage);
    }else if(command == "getInterval"){
        sdOSCMessage returnMessage("/spatdif/interval");
        returnMessage.appendFloat(static_cast<float>(getInterval()));
        returnMessageVector.push_back(returnMessage);
    }// query about scene{
    else if(command == "getNumberOfEntities"){
        sdOSCMessage returnMessage("/spatdif/numberOfEntities");
        returnMessage.appendInt(scene->getNumberOfEntities());
        returnMessageVector.push_back(returnMessage);
    }else if(command == "getEntityNames"){
        sdOSCMessage returnMessage("/spatdif/entityNames");
        returnMessage.appendStrings(scene->getEntityNames());
        returnMessageVector.push_back(returnMessage);
    }else if(command == "getOrdering"){
        sdOSCMessage returnMessage("/spatdif/ordering");
        returnMessage.appendString(scene->getOrderingAsString());
        returnMessageVector.push_back(returnMessage);   
    }
    // core descriptors
    else if(command.find("Position") != string::npos){ // contains keyword "Position"
        sdEntityCore* entity = scene->getEntity(message.getArgumentAsString(0));
        if(!entity) 
            return returnMessageVector;
        float fpos[3];
        double* dpos;
        if(command == "getPosition"){
            dpos = static_cast<double*>(entity->getValue(queryTime, SD_POSITION));
        }else if(command == "getNextPosition"){
            dpos = static_cast<double*>(entity->getNextValue(queryTime, SD_POSITION));
        }else if(command == "getPreviousPosition"){
            dpos = static_cast<double*>(entity->getPreviousValue(queryTime, SD_POSITION));
        }
        sdOSCMessage returnMessage("/spatdif/position");
        returnMessage.appendString(entity->getName());
        returnMessage.appendFloats(doublesToFloats(dpos, fpos, 3),3);
        returnMessageVector.push_back(returnMessage);
    }else if(command.find("Orientation") != string::npos){ // contains keyword "Position"
        sdEntityCore* entity = scene->getEntity(message.getArgumentAsString(0));
        if(!entity) 
            return returnMessageVector;
        float fori[3];
        double* dori;
        if(command == "getOrientation"){
            dori = static_cast<double*>(entity->getValue(queryTime, SD_ORIENTATION));
        }else if(command == "getNextOrientation"){
            dori = static_cast<double*>(entity->getNextValue(queryTime, SD_ORIENTATION));
        }else if(command == "getPreviousOrientation"){
            dori = static_cast<double*>(entity->getPreviousValue(queryTime, SD_ORIENTATION));
        }
        sdOSCMessage returnMessage("/spatdif/orientation");
        returnMessage.appendString(entity->getName());
        returnMessage.appendFloats(doublesToFloats(dori, fori, 3),3);
        returnMessageVector.push_back(returnMessage);
    }else if(command.find("Present")){
        sdEntityCore* entity = scene->getEntity(message.getArgumentAsString(0));
        if(!entity) 
            return returnMessageVector;
        bool flag;
        if(command == "getPresent"){
            flag = static_cast<bool*>(entity->getValue(queryTime, SD_PRESENT));
        }else if(command == "getNextPresent"){
            flag = static_cast<bool*>(entity->getNextValue(queryTime, SD_PRESENT));
        }else if(command == "getPreviousPresent"){
            flag = static_cast<bool*>(entity->getPreviousValue(queryTime, SD_PRESENT));
        }
        sdOSCMessage returnMessage("/spatdif/present");
        returnMessage.appendString(entity->getName());
        returnMessage.appendInt(static_cast<int>(flag));
        returnMessageVector.push_back(returnMessage);
    }else{
        cout << "sdOSCMessage Error: unknown command:" << command << endl;
    }
    return returnMessageVector;
}

void sdOSCResponder::setAction(string command, sdOSCMessage message){
    
    if(command == "setQueryTime"){
        setQueryTime(static_cast<double>(message.getArgumentAsFloat(0)));
    }else if(command == "setWriteTime"){
        setWriteTime(static_cast<double>(message.getArgumentAsFloat(0)));
    }else if(command == "setInterval"){
        setInterval(static_cast<double>(message.getArgumentAsFloat(0)));
    }
    // core descriptors
    else if(command == "setPosition"){
        double dpos[3];
        float fpos[3];
        fpos[0] = message.getArgumentAsFloat(1);
        fpos[1] = message.getArgumentAsFloat(2);
        fpos[2] = message.getArgumentAsFloat(3);
        scene->setValue(message.getArgumentAsString(0), writeTime, SD_POSITION, floatsToDoubles(fpos,dpos,3));
    }else if(command == "setOrientation"){
        double dori[3];
        float fori[3];
        fori[0] = message.getArgumentAsFloat(1);
        fori[1] = message.getArgumentAsFloat(2);
        fori[2] = message.getArgumentAsFloat(3);
        scene->setValue(message.getArgumentAsString(0), writeTime, SD_ORIENTATION, floatsToDoubles(fori,dori,3));
    }else if(command == "setPresent"){
        bool flag;
        flag = static_cast<bool>(message.getArgumentAsInt(1));
        scene->setValue(message.getArgumentAsString(0), writeTime, SD_PRESENT, &flag);
    }
    // scene properties
    else if(command == "setOrdering"){
        scene->setOrdering(message.getArgumentAsString(0));
    }
}

    