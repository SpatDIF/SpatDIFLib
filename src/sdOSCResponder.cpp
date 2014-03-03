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
    
    if( command == "setQueryTime"){
        if (message.getArguments().size() != 1) {
            cout << "sdOSCResponder Error: invalid number of arguments for setQueryTime";
            return returnMessageVector;
        }
        setQueryTime(static_cast<double>(message.getArgumentAsFloat(0)));
        sdOSCMessage message("/spatdif/success");
        returnMessageVector.push_back(message);
        return returnMessageVector;
    }else if(command == "getQueryTime"){
        if (message.getArguments().size() != 0) {
            cout << "sdOSCResponder Error: invalid number of arguments for setQueryTime";
            return returnMessageVector;
        }
        sdOSCMessage returnMessage("/spatdif/queryTime");
        returnMessage.appendFloat(static_cast<float>(getQueryTime()));
        returnMessageVector.push_back(returnMessage);
    }
    
    return returnMessageVector;;
}



    /*
    string element, command, errorMessage;
    vector<string> arguments;
    istringstream iss(oscMessage);
    int count = 0;
    EKind kd;
    
    // analyze
    while(iss.good()){
        iss >> element;
        
        switch(count){
            case 0:{
                //address pattern
                if (element[0] != '/') {
                    cout << "sdOSCReponder Error: The first element of incoming OSC Message is not an address pattern." << endl;
                    return returnMessageVector;
                }
                //split into parts
                vector <string>ads = splitString(element);
                ads.erase (ads.begin());
                
                if (ads.size() < 2 ) {
                    cout << "sdOSCResponder Error: The address pattern consists of two few elements." << endl;
                    return returnMessageVector;
                }
                
                if(ads[0] != "spatdifcmd"){
                    cout << "sdOSCResponder Error: The address pattern of the received message does not begin with \"spatdifcmd\"." << endl;
                    return returnMessageVector;
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

    // interpet the command
    if( command == "setQueryTime"){
        if (arguments.size() != 0) {
            
        }
        setQueryTime();
        
    }else if(command == "getQueryTime"){
        
        returnMessageVector.push_back(errorMessage);
        returnMessageVector.push_back("/spatdif/queryTime ,d " + doubleToString(getQueryTime()));
        
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
    }else if(command == "setWriteTime"){
        
        if(!checkNumberOfArguments( 1, arguments.size(), command, errorMessage))
            returnMessageVector.push_back(errorMessage);
        else
            setWriteTime(stringToDouble(arguments[0]));
        
    }else if(command == "getWriteTime"){
        
        if(!checkNumberOfArguments( 0, arguments.size(), command, errorMessage))
            returnMessageVector.push_back(errorMessage);
        else
            returnMessageVector.push_back("/spatdif/writeTime ,d " + doubleToString(getWriteTime()));
        
    }else if(command == "setInterval"){
        
        if(!checkNumberOfArguments( 1, arguments.size(), command, errorMessage))
            returnMessageVector.push_back(errorMessage);
        else
            setInterval(stringToDouble(arguments[0]));
        
    }else if(command == "getInterval"){
        
        if(!checkNumberOfArguments( 0, arguments.size(), command, errorMessage))
            returnMessageVector.push_back(errorMessage);
        else
            returnMessageVector.push_back("/spatdif/interval ,d " + doubleToString(getInterval()));
        
    }else if(command == "setPosition"){
        if(!checkNumberOfArguments( 4, arguments.size() ,command, errorMessage))
            returnMessageVector.push_back(errorMessage);
        else{
            double pos[3];
            pos[0] = stringToDouble(arguments[1]);
            pos[1] = stringToDouble(arguments[2]);
            pos[2] = stringToDouble(arguments[3]);
            scene->setValue(arguments[0], writeTime, SD_POSITION, (void*)pos);
        }
    }else if(command == "getPosition"){

        if(!checkNumberOfArguments( 1, arguments.size() ,command, errorMessage)){
            returnMessageVector.push_back(errorMessage);
        }
        
        sdEntityCore* entity =  scene->getEntity(arguments[0]);
        if (!entity) {
            returnMessageVector.push_back("/spatdif/error");
        }
        sdEventCore* event = (sdEventCore*)entity->getEvent(queryTime, SD_POSITION);
        if(!event){
            returnMessageVector.push_back("/spatdif/error");
        }
        string returnStr = "/spatdif/source/";
        returnMessageVector.push_back(returnStr + entity->getName() + "/position ,ddd " + event->getValueAsString());
        
    }else if(command == "getNextPosition"){
        if(!checkNumberOfArguments( 1, arguments.size() ,command, errorMessage)){
            returnMessageVector.push_back(errorMessage);
        }
        
        sdEntityCore* entity =  scene->getEntity(arguments[0]);
        if (!entity) {
            returnMessageVector.push_back("/spatdif/error");
        }
        sdEventCore* event = (sdEventCore*)entity->getNextEvent(queryTime, SD_POSITION);
        if(!event){
            returnMessageVector.push_back("/spatdif/error");
        }
        string returnStr = "/spatdif/source/";
        returnMessageVector.push_back(returnStr + entity->getName() + "/position ,dddd " + event->getTimeAsString() + ' ' + event->getValueAsString());
        
    }else if(command == "getPreviousPosition"){
        if(!checkNumberOfArguments( 1, arguments.size() ,command, errorMessage)){
            returnMessageVector.push_back(errorMessage);
        }
        
        sdEntityCore* entity =  scene->getEntity(arguments[0]);
        if (!entity) {
            returnMessageVector.push_back("/spatdif/error");
        }
        sdEventCore* event = (sdEventCore*)entity->getPreviousEvent(queryTime, SD_POSITION);
        if(!event){
            returnMessageVector.push_back("/spatdif/error");
        }
        string returnStr = "/spatdif/source/";
        returnMessageVector.push_back(returnStr + entity->getName() + "/position ,dddd " + event->getTimeAsString() + ' ' + event->getValueAsString());
        
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
            

