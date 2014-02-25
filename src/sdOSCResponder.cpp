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

bool sdOSCResponder::checkNumberOfArguments(int expectedNumber, int actualNumber, string command, string &errorMessage){
    string mes;
    if (actualNumber == expectedNumber){
        return true;
    }else if (actualNumber < expectedNumber) {
        mes = "too few arguments";
    }else if(actualNumber > expectedNumber){
        mes = "too many arguments";
    }
    errorMessage = "/spatdif/error";
    return false;
}


vector<string> sdOSCResponder::forwardOSCMessage(string oscMessage, double time){
    setQueryTime(time);
    return forwardOSCMessage(oscMessage);
}

vector<string> sdOSCResponder::forwardOSCMessage(string oscMessage){

    string element, command, errorMessage;
    vector<string> returnMessageVector;
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
    
        if(!checkNumberOfArguments( 1, arguments.size() ,command, errorMessage))
            returnMessageVector.push_back(errorMessage);
        else
            setQueryTime(stringToDouble(arguments[0]));

    }else if(command == "getQueryTime"){

        if(!checkNumberOfArguments( 0, arguments.size(), command, errorMessage))
            returnMessageVector.push_back(errorMessage);
        else
            returnMessageVector.push_back("/spatdif/queryTime ,d " + doubleToString(getQueryTime()));

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
            

