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


sdOSCResponder::sdOSCResponder(void){
    queryTime = 0.0;
    writeTime = 0.0;
}

sdOSCResponder::sdOSCResponder(sdScene *scene){
    sdOSCResponder::scene = scene;
    sdOSCResponder();
}

std::vector<std::string> sdOSCResponder::splitString(const std::string &str){
    std::vector<std::string> res;
    size_t current = 0, found;
    while((found = str.find_first_of('/', current)) != std::string::npos){
        res.push_back(std::string(str, current, found - current));
        current = found + 1;
    }
    res.push_back(std::string(str, current, str.size() - current));
    return res;
}

// low level interface
std::vector<std::vector<unsigned char> > sdOSCResponder::forwardOSCMessage(std::vector<unsigned char> message){
    sdOSCMessage mes(message);
    std::vector<sdOSCMessage> rMesVector = forwardOSCMessage(mes);
    std::vector<sdOSCMessage>::iterator it = rMesVector.begin();
    std::vector<std::vector<unsigned char> > rRawVector;
    while(it != rMesVector.end()){
        sdOSCMessage m = *it;
        rRawVector.push_back(m.getOSCMessage());
        it++;
    }
    return rRawVector;
}

// high level interface
std::vector<sdOSCMessage> sdOSCResponder::forwardOSCMessage(sdOSCMessage message){
    std::vector<sdOSCMessage> returnMessageVector;
    
    std::string address = message.getAddressAsString();
    if (address[0] != '/') {
        sdOSCMessage errorMessage("/spatdif/error");
        errorMessage.appendString("Invalid OSC Address Pattern");
        returnMessageVector.push_back(errorMessage);
        return returnMessageVector;
    }
    
    std::vector <std::string>ads = splitString(address);
    ads.erase (ads.begin());
    
    if (ads.size() < 2 ) {
        sdOSCMessage errorMessage("/spatdif/error");
        errorMessage.appendString("Two few arguments");
        returnMessageVector.push_back(errorMessage);
        return returnMessageVector;
    }
    
    if(ads[0] != "spatdifcmd"){
        sdOSCMessage errorMessage("/spatdif/error");
        errorMessage.appendString("Invalid Comannd");
        returnMessageVector.push_back(errorMessage);
        return returnMessageVector;
    }
    std::string command;
    EExtension extension = SD_CORE;

    if(ads.size() == 2){ // core or extension
        command = ads[1]; // get the element right after /spatdifcmd
    }else if(ads.size() == 3){  // extension
        std::string extensionName = ads[1];
        extension = stringToExtension(extensionName);
        if(extension == SD_EXTENSION_ERROR){
            sdOSCMessage errorMessage("/spatdif/error");
            errorMessage.appendString("No such extension: " + extensionName);
            returnMessageVector.push_back(errorMessage);
            return returnMessageVector; 
        }else{
            scene->isExtensionActivated(extension);
            command  = ads[2];
        }
    }
    std::string action =  command.substr(0, 3); //  check if the client try to set or get

    if(action == "get"){
        returnMessageVector = getAction(command, message);
    }else if(action == "set"){
        setAction(command, message, extension);
    }else{
        if(command == "addEntity"){
            scene->addEntity(message.getArgumentAsString(0));
        }else if(command == "removeEntity"){
            scene->removeEntity(message.getArgumentAsString(0));
        }else if(command == "removeAllEntities"){
            scene->removeAllEntities();
        }else if(command == "addExtension"){
            scene->addExtension(message.getArgumentAsString(0));
        }else{
            sdOSCMessage errorMessage("/spatdif/error");
            errorMessage.appendString("Invalid Comannd");
            returnMessageVector.push_back(errorMessage);
            return returnMessageVector;
        }
    }
    return returnMessageVector;
}

std::vector<sdOSCMessage> sdOSCResponder::getAction(std::string command, sdOSCMessage message){
    sdOSCMessage returnMessage;
    std::vector<sdOSCMessage> returnMessageVector;
    bool singleMessage = true;
    // internal variable
    if(command == "getEventSetsFromAllEntities"){
        std::vector<sdReport> reports;
        if(interval != 0.0){
           reports = scene->getEventSetsFromAllEntities(queryTime, queryTime+interval); // in the specified region
        }else{
           reports = scene->getEventSetsFromAllEntities(queryTime); 
        }
        std::vector<sdReport>::iterator rit = reports.begin();
        while(rit != reports.end()){
            sdReport report = *rit;
            sdEntityCore* entity = report.entity;
            std::multiset<sdEvent*, sdEventCompare>eventSet = report.eventSet;
            std::multiset<sdEvent*, sdEventCompare>::iterator eit = eventSet.begin();
            while (eit != eventSet.end()) {
                sdEvent *event = *eit;
                returnMessage.clear();
                if(entity->isCoreDescriptor(event->getDescriptor())){
                    returnMessage.setAddress("/spatdif/source/"+entity->getName()+"/"+event->getDescriptorAsString());
                }else{
                    sdEntityExtension* entityExtension = entity->getResponsibleExtension(event->getDescriptor());
                    if(entityExtension){
                        returnMessage.setAddress("/spatdif/source/"+entity->getName()+"/"+entityExtension->getExtensionNameAsString()+"/"+event->getDescriptorAsString());
                    }
                }
                
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
                    case SD_MEDIA_TYPE:
                    case SD_MEDIA_LOCATION:
                    case SD_MEDIA_ID:{
                        returnMessage.appendString(event->getValueAsString());
                        returnMessageVector.push_back(returnMessage);
                        break;
                    }
                    case SD_MEDIA_CHANNEL:{
                        int *channel = static_cast<int*>(event->getValue());
                        returnMessage.appendInt(*channel);
                        returnMessageVector.push_back(returnMessage);
                        break;
                    }
                    case SD_MEDIA_TIME_OFFSET:
                    case SD_MEDIA_GAIN:{
                        double *value = static_cast<double*>(event->getValue());
                        returnMessage.appendFloat(static_cast<float>(*value));
                        returnMessageVector.push_back(returnMessage);
                        break;
                    }
                    default:{
                        sdOSCMessage errorMessage("/spatdif/error");
                        errorMessage.appendString("No such descriptors");
                        returnMessageVector.push_back(errorMessage);
                        return returnMessageVector;
                    }
                }
                eit++;
            }
            rit++;
        }
        singleMessage = false;
    }else if(command == "getNextEventTime"){
        returnMessage.setAddress("/spatdif/nextEventTime");
        double nextEventTime = scene->getNextEventTime(queryTime);
        returnMessage.appendFloat(static_cast<float>(nextEventTime));
    }else if(command == "getDeltaTimeToNextEvent"){
        returnMessage.setAddress("/spatdif/deltaTime");
        double deltaTime = scene->getDeltaTimeToNextEvent(queryTime);
        returnMessage.appendFloat(static_cast<float>(deltaTime));
    }else if(command == "getQueryTime"){
        returnMessage.setAddress("/spatdif/queryTime");
        returnMessage.appendFloat(static_cast<float>(getQueryTime()));
    }else if(command == "getWriteTime"){
        returnMessage.setAddress("/spatdif/writeTime");
        returnMessage.appendFloat(static_cast<float>(getWriteTime()));
    }else if(command == "getInterval"){
        returnMessage.setAddress("/spatdif/interval");
        returnMessage.appendFloat(static_cast<float>(getInterval()));
    }// query about scene{
    else if(command == "getNumberOfEntities"){
        returnMessage.setAddress("/spatdif/numberOfEntities");
        returnMessage.appendInt(scene->getNumberOfEntities());
    }else if(command == "getEntityNames"){
        returnMessage.setAddress("/spatdif/entityNames");
        returnMessage.appendStrings(scene->getEntityNames());
    }else if(command == "getOrdering"){
        returnMessage.setAddress("/spatdif/ordering");
        returnMessage.appendString(scene->getOrderingAsString());
    }else if(command == "getAuthor"){
        returnMessage.setAddress("/spatdif/author");
        returnMessage.appendString(scene->getInfo().getAuthor());
    }else if(command == "getHost"){
        returnMessage.setAddress("/spatdif/host");
        returnMessage.appendString(scene->getInfo().getHost());
    }else if(command == "getDate"){
        returnMessage.setAddress("/spatdif/date");
        returnMessage.appendString(scene->getInfo().getDateAsString());
    }else if(command == "getLocation"){
        returnMessage.setAddress("/spatdif/location");
        returnMessage.appendString(scene->getInfo().getLocation());
    }else if(command == "getSession"){
        returnMessage.setAddress("/spatdif/session");
        returnMessage.appendString(scene->getInfo().getSession());
    }else if(command == "getAnnotation"){
        returnMessage.setAddress("/spatdif/annotation");
        returnMessage.appendString(scene->getInfo().getAnnotation());
    }else if(command == "getNumberOfActivatedExtensions"){
        returnMessage.setAddress("/spatdif/numberOfActivatedExtensions");
        returnMessage.appendInt(scene->getNumberOfActivatedExtensions());
    }else if(command == "getActivatedExtensions"){
        returnMessage.setAddress("/spatdif/activatedExtensions");
        returnMessage.appendInt(scene->getNumberOfActivatedExtensions());
    }
    // core descriptors
    else if(command.find("Position") != std::string::npos){ // contains keyword "Position"
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
        std::string address = "/spatdif/source/" + entity->getName() + "/position";
        returnMessage.setAddress(address);
        returnMessage.appendString(entity->getName());
        returnMessage.appendFloats(doublesToFloats(dpos, fpos, 3),3);
    }else if(command.find("Orientation") != std::string::npos){ // contains keyword "Position"
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
        std::string address = "/spatdif/source/" + entity->getName() + "/orientation";
        returnMessage.setAddress(address);
        returnMessage.appendString(entity->getName());
        returnMessage.appendFloats(doublesToFloats(dori, fori, 3),3);
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
        std::string address = "/spatdif/source/" + entity->getName() + "/present";
        returnMessage.setAddress(address);
        returnMessage.appendString(entity->getName());
        returnMessage.appendInt(static_cast<int>(flag));
    }else{
        returnMessage.setAddress("/spatdif/error");
        returnMessage.appendString("invalid command");
        returnMessageVector.push_back(returnMessage);  
        return returnMessageVector;
    }

    if(singleMessage){
        returnMessageVector.push_back(returnMessage);  
    }
    return returnMessageVector;
}

void sdOSCResponder::setAction(std::string command, sdOSCMessage message, EExtension extension){

    switch(extension){
        case SD_MEDIA:{
            // media extension
            if(command == "setID"){
                std::string id = message.getArgumentAsString(1);
                scene->setValue(message.getArgumentAsString(0), writeTime, SD_MEDIA_ID, static_cast<void*>(&id));
            }else if(command == "setType"){
                std::string type = message.getArgumentAsString(1);
                scene->setValue(message.getArgumentAsString(0), writeTime, SD_MEDIA_TYPE, static_cast<void*>(&type));
            }else if(command == "setLocation"){
                std::string location = message.getArgumentAsString(1);
                scene->setValue(message.getArgumentAsString(0), writeTime, SD_MEDIA_LOCATION, static_cast<void*>(&location));
            }else if(command == "setChannel"){
                int channel = message.getArgumentAsInt(1);
                scene->setValue(message.getArgumentAsString(0), writeTime, SD_MEDIA_CHANNEL, static_cast<void*>(&channel));
            }else if(command == "setTimeOffset"){
                double timeOffset = static_cast<double>(message.getArgumentAsFloat(1));
                scene->setValue(message.getArgumentAsString(0), writeTime, SD_MEDIA_TIME_OFFSET, static_cast<void*>(&timeOffset));
            }else if(command == "setGain"){
                double gain = static_cast<double>(message.getArgumentAsFloat(1));
                scene->setValue(message.getArgumentAsString(0), writeTime, SD_MEDIA_GAIN, static_cast<void*>(&gain));
            }
            break;
        }
        default:{
            if(command == "setQueryTime"){
                setQueryTime(static_cast<double>(message.getArgumentAsFloat(0)));
            }else if(command == "setQueryTimeToNextEvent"){
                setQueryTime(scene->getNextEventTime(queryTime));
            }else if(command == "setWriteTime"){
                setWriteTime(static_cast<double>(message.getArgumentAsFloat(0)));
            }else if(command == "setInterval"){
                setInterval(static_cast<double>(message.getArgumentAsFloat(0)));
            // core descriptors

            }else if(command == "setPosition"){
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
                scene->setValue(message.getArgumentAsString(0), writeTime, SD_PRESENT, static_cast<void*>(&flag));
            }
            // scene properties
            else if(command == "setOrdering"){
                scene->setOrdering(message.getArgumentAsString(0));
            }else if(command == "setAuthor"){
                sdInfo info = scene->getInfo();
                info.setAuthor(message.getArgumentAsString(0));
                scene->setInfo(info);
            }else if(command == "setHost"){
                sdInfo info = scene->getInfo();
                info.setHost(message.getArgumentAsString(0));
                scene->setInfo(info);
            }else if(command == "setDate"){
                sdInfo info = scene->getInfo();
                info.setDate(message.getArgumentAsString(0));
                scene->setInfo(info);
            }else if(command == "setLocation"){
                sdInfo info = scene->getInfo();
                info.setLocation(message.getArgumentAsString(0));
                scene->setInfo(info);
            }else if(command == "setSession"){
                sdInfo info = scene->getInfo();
                info.setSession(message.getArgumentAsString(0));
                scene->setInfo(info);
            }else if(command == "setAnnotation"){
                sdInfo info = scene->getInfo();
                info.setAnnotation(message.getArgumentAsString(0));
                scene->setInfo(info);
            }
            break;
        }
    }
}


    