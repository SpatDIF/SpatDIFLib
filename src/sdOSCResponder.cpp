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
        cout << "sdOSCResponer Error: invalid command" << endl;
    }
    return returnMessageVector;
}

vector<sdOSCMessage> sdOSCResponder::getAction(string command, sdOSCMessage message){
    vector<sdOSCMessage> returnMessageVector;
    
    // internal variable
    if(command == "getQueryTime"){
        sdOSCMessage returnMessage("/spatdif/queryTime");
        returnMessage.appendFloat(static_cast<float>(getQueryTime()));
        returnMessageVector.push_back(returnMessage);
    }else if(command == "getWriteTime"){
        sdOSCMessage returnMessage("/spatdif/WriteTime");
        returnMessage.appendFloat(static_cast<float>(getWriteTime()));
        returnMessageVector.push_back(returnMessage);
    }else if(command == "getInterval"){
        sdOSCMessage returnMessage("/spatdif/Interval");
        returnMessage.appendFloat(static_cast<float>(getInterval()));
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
    }
}

  /*
 
    }else if ( command == "getEventSetsFromAllEntities"){
        if(!checkNumberOfArguments( 0, arguments.size() ,command, errorMessage))
            returnMessageVector.push_back(errorMessage);
        else{
            vector<sdReport> reports;
            reports = scene->getEventSetsFromAllEntities(queryTime, queryTime+interval);
            vector<sdReport>::iterator rit = reports.begin();
            while(rit != reports.begin()){
                sdReport report = *rit;
                sdEntityCore* entity = report.entity;
                multiset<sdEvent*, sdEventCompare>eventSet = report.eventSet;
                multiset<sdEvent*, sdEventCompare>::iterator eit = eventSet.begin();
                while (eit != eventSet.end()) {
                    sdEvent *event = *eit;
                    
                    switch (event->getDescriptor()) {
                        case SD_POSITION:
                            returnMessageVector.push_back("/spatdif/event ,dssddd "
                                                          + event->getTimeAsString() + ' '
                                                          + entity->getName() + ' '
                                                          + event->getDescriptorAsString() + ' '
                                                          + event->getValueAsString());
                            break;
                        case SD_ORIENTATION:
                            returnMessageVector.push_back("/spatdif/event ,dssddd "
                                                          + event->getTimeAsString()
                                                          + entity->getName() + ' '
                                                          + event->getDescriptorAsString() + ' '
                                                          + event->getValueAsString());
                            break;
                        case SD_PRESENT:
                            returnMessageVector.push_back("/spatdif/event ,dsss "
                                                          + event->getTimeAsString()
                                                          + entity->getName() +' '
                                                          + event->getDescriptorAsString() +' '
                                                          + event->getValueAsString());
                            break;
                        default:
                            break;
                    }
                    eit++;
                }
                rit++;
            }

     
    }
    else if(command == "setOrientation"){
        if(!checkNumberOfArguments( 4, arguments.size() ,command, errorMessage)){
            returnMessageVector.push_back(errorMessage);
        }
        double ori[3];
        ori[0] = stringToDouble(arguments[1]);
        ori[1] = stringToDouble(arguments[2]);
        ori[2] = stringToDouble(arguments[3]);
        scene->setValue(arguments[0], writeTime, SD_ORIENTATION, (void*)ori);
        
    }else if(command == "getOrientation"){
        
        if(!checkNumberOfArguments( 1, arguments.size() ,command, errorMessage)){
            returnMessageVector.push_back(errorMessage);
        }
        
        sdEntityCore* entity =  scene->getEntity(arguments[0]);
        if (!entity) {
            returnMessageVector.push_back("/spatdif/error");
        }
        sdEventCore* event = (sdEventCore*)entity->getEvent(queryTime, SD_ORIENTATION);
        if(!event){
            returnMessageVector.push_back("/spatdif/error");
        }
        string returnStr = "/spatdif/source/";
        returnMessageVector.push_back(returnStr + entity->getName() + "/orientation ,ddd " + event->getValueAsString());
        
    }else if(command == "getNextOrientation"){
        if(!checkNumberOfArguments( 1, arguments.size() ,command, errorMessage)){
            returnMessageVector.push_back(errorMessage);
        }
        
        sdEntityCore* entity =  scene->getEntity(arguments[0]);
        if (!entity) {
            returnMessageVector.push_back("/spatdif/error");
        }
        sdEventCore* event = (sdEventCore*)entity->getNextEvent(queryTime, SD_ORIENTATION);
        if(!event){
            returnMessageVector.push_back("/spatdif/error");
        }
        string returnStr = "/spatdif/source/";
        returnMessageVector.push_back(returnStr + entity->getName() + "/orientation ,dddd " + event->getTimeAsString() + ' '+ event->getValueAsString());
        
    }else if(command == "getPreviousOrientation"){
        if(!checkNumberOfArguments( 1, arguments.size() ,command, errorMessage)){
            returnMessageVector.push_back(errorMessage);
        }
        
        sdEntityCore* entity =  scene->getEntity(arguments[0]);
        if (!entity) {
            returnMessageVector.push_back("/spatdif/error");
        }
        sdEventCore* event = (sdEventCore*)entity->getPreviousEvent(queryTime, SD_ORIENTATION);
        if(!event){
            returnMessageVector.push_back("/spatdif/error");
        }
        string returnStr = "/spatdif/source/";
        returnMessageVector.push_back(returnStr + entity->getName() + "/orientation ,dddd " + event->getTimeAsString() + ' ' + event->getValueAsString());
        
    }
    else if(command == "setPresent"){
        if(!checkNumberOfArguments( 2, arguments.size() ,command, errorMessage)){
            returnMessageVector.push_back(errorMessage);
        }
        bool present = stringToBool(arguments[1]);
        scene->setValue(arguments[0], writeTime, SD_PRESENT, (void*)&present);
        
    }else if(command == "getPresent"){
        
        if(!checkNumberOfArguments( 1, arguments.size() ,command, errorMessage)){
            returnMessageVector.push_back(errorMessage);
        }
        
        sdEntityCore* entity =  scene->getEntity(arguments[0]);
        if (!entity) {
            returnMessageVector.push_back("/spatdif/error");
        }
        sdEventCore* event = (sdEventCore*)entity->getEvent(queryTime, SD_PRESENT);
        if(!event){
            returnMessageVector.push_back("/spatdif/error");
        }
        returnMessageVector.push_back("/spatdif/source/" + entity->getName() + "/present ,s " + event->getValueAsString());

    }else if(command == "getNextPresent"){
        
        if(!checkNumberOfArguments( 1, arguments.size() ,command, errorMessage)){
            returnMessageVector.push_back(errorMessage);
        }
        
        sdEntityCore* entity =  scene->getEntity(arguments[0]);
        if (!entity) {
            returnMessageVector.push_back("/spatdif/error");
        }
        sdEventCore* event = (sdEventCore*)entity->getNextEvent(queryTime, SD_PRESENT);
        if(!event){
            returnMessageVector.push_back("/spatdif/error");
        }
        
        returnMessageVector.push_back("/spatdif/source/" + entity->getName() + "/present ,ds " + event->getTimeAsString() + ' ' + event->getValueAsString());
        
    }else if(command == "getPreviousPresent"){
        
        if(!checkNumberOfArguments( 1, arguments.size() ,command, errorMessage)){
            returnMessageVector.push_back(errorMessage);
        }
        
        sdEntityCore* entity =  scene->getEntity(arguments[0]);
        if (!entity) {
            returnMessageVector.push_back("/spatdif/error");
        }
        sdEventCore* event = (sdEventCore*)entity->getPreviousEvent(queryTime, SD_PRESENT);
        if(!event){
            returnMessageVector.push_back("/spatdif/error");
        }
        returnMessageVector.push_back("/spatdif/source/" + entity->getName() + "/present ,ds " + event->getTimeAsString() + ' ' + event->getValueAsString());
        
    }else if(command == "getNumberOfEntities"){
        
        if(!checkNumberOfArguments( 0, arguments.size() ,command, errorMessage))
            returnMessageVector.push_back(errorMessage);
        else
            returnMessageVector.push_back("/spatdif/source/numberOfEntities ,i " + intToString(scene->getNumberOfEntities()));
        
    }else if(command == "getEntityNames"){
        
        if(!checkNumberOfArguments( 0, arguments.size() ,command, errorMessage)){
            returnMessageVector.push_back(errorMessage);
        }
        int num = scene->getNumberOfEntities();
        string tt, names;
        tt = ',';
        for(int i = 0; i < num; i++){
            tt += 's';
            names += scene->getEntityName(i);
            names += ' ';
        }
        returnMessageVector.push_back( "/spatdif/source/entityNames " + tt + names);
        
    }else if(command == "getOrdering"){
        if(!checkNumberOfArguments( 0, arguments.size() ,command, errorMessage))
            returnMessageVector.push_back(errorMessage);
        else
            returnMessageVector.push_back("/spatdif/ordering ,s " + scene->getOrderingAsString());
   
    }else if(command == "setOrdering"){
        if(!checkNumberOfArguments( 1, arguments.size() ,command, errorMessage)){
            returnMessageVector.push_back(errorMessage);
        }
        
        if(arguments[0] == "track"){
            scene->setOrdering(SD_TRACK);
        }else if(arguments[0] == "time"){
            scene->setOrdering(SD_TIME);
        }else{
            returnMessageVector.push_back( "/spatdif/error" );
        }
    }else if(command == "addEntity"){
        if(!checkNumberOfArguments( 1, arguments.size() ,command, errorMessage))
            returnMessageVector.push_back(errorMessage);
        else
            scene->addEntity(arguments[0]);
    }else if(command == "removeEntity"){
        if(!checkNumberOfArguments( 1, arguments.size() ,command, errorMessage))
            returnMessageVector.push_back(errorMessage);
        else
            scene->removeEntity(arguments[0]);
    }else if(command == "removeAllEntities"){
        if(!checkNumberOfArguments( 0, arguments.size() ,command, errorMessage))
            returnMessageVector.push_back(errorMessage);
        else
            scene->removeAllEntities();
    }
    return returnMessageVector;
    
}
     */
            

