/** @file
 *
 * @ingroup spatdiflib
 *
 * @brief
 *
 *
 *
 *
 * @authors Chikashi Miyama, Trond Lossius
 *
 * @copyright Copyright (C) 2013 - 2014 Zurich University of the Arts, Institute for Computer Music and Sound Technology @n
 * This code is licensed under the terms of the "New BSD License" @n
 * http://creativecommons.org/licenses/BSD/
 */

#ifndef ____sdOSCResponder__
#define ____sdOSCResponder__

#include "sdScene.h"
#include "sdOSCMessage.h"
#include <string>


/*!
    This class interprets incoming OSC messages and forward it to a specified instance of sdScene.
    The response from sdScene will be formatted in OSC format and returned to the client.
    
    The List commands can be found in the following separate page.
    - \subpage osc_commands
 
 */
class sdOSCResponder{
    
private:
    /*! The interpreted OSC messages will be sent do this sdScene */
    sdScene *scene;

    /*! time index for queries*/
    double queryTime;

    /*! time index for editing*/
    double writeTime;

    /*! time interval for queries with time frame  */
    double interval;
    
    /*! private utility function for spliting strings by slash */
    std::vector <std::string> splitString(const std::string &str);
    std::vector<sdOSCMessage> getAction(std::string command, sdOSCMessage message);
    void setAction(std::string command, sdOSCMessage message, EExtension extension);

    sdOSCMessage getEventSetsFromAllEntities(const double &time) const;
public:
    
    /*! constructor without assignment of a scene
     */
    sdOSCResponder();

    /*! constructor.
     @param scene the target scene
     */
    sdOSCResponder(sdScene *scene);
    
    /*! setTergetScene
     */
    void setScene(sdScene *scene);
    
    /*! getTargetScene
     */
    sdScene* getScene(void);
    
    /*! this function interprets incoming raw OSCMessage and forward it to the scene
     @param message raw oscMessage, consisting of multiple blocks (4 byte blocks)
     */
    std::vector<std::vector<unsigned char> > forwardOSCMessage(std::vector<unsigned char> message);

    /*! this is the overloaded high level interface function of the function above.
     easier to handle but less compatible.
     @param message an instance sdOSCMessage
     */
    std::vector<sdOSCMessage> forwardOSCMessage(sdOSCMessage message);
    
    /*!
     @param time set queryTime to this value
     */
    inline void setQueryTime(double time);

    /*!
     @param time set writeTime to this value
     */
    inline void setWriteTime(double time);

    /*!
     @param time set interval to this value
     */
    inline void setInterval(double time);
    
    
    /*!
     returns queryTime
     */
    inline double getQueryTime(void);
    
    /*!
     returns writeTime
     */
    inline double getWriteTime(void);
    
    /*!
     returns interval
     */
    inline double getInterval(void);
    
};



inline void sdOSCResponder::setScene(sdScene *scene){
    sdOSCResponder::scene = scene;
}

inline sdScene* sdOSCResponder::getScene(void){
    return  scene;
}

inline void sdOSCResponder::setQueryTime(double time){
    queryTime = time;
}

inline void sdOSCResponder::setWriteTime(double time){
    writeTime = time;
}

inline void sdOSCResponder::setInterval(double time){
    interval =  time;
}

inline double sdOSCResponder::getQueryTime(void){
    return queryTime;
}

inline double sdOSCResponder::getWriteTime(void){
    return writeTime;
}

inline double sdOSCResponder::getInterval(void){
    return interval;
}



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
        errorMessage.appendArgument<std::string>("Invalid OSC Address Pattern");
        returnMessageVector.push_back(errorMessage);
        return returnMessageVector;
    }
    
    std::vector <std::string>ads = splitString(address);
    ads.erase (ads.begin());
    
    if (ads.size() < 2 ) {
        sdOSCMessage errorMessage("/spatdif/error");
        errorMessage.appendArgument<std::string>("Two few arguments");
        returnMessageVector.push_back(errorMessage);
        return returnMessageVector;
    }
    
    if(ads[0] != "spatdifcmd"){
        sdOSCMessage errorMessage("/spatdif/error");
        errorMessage.appendArgument<std::string>("Invalid Comannd");
        returnMessageVector.push_back(errorMessage);
        return returnMessageVector;
    }
    std::string command;
    EExtension extension = EExtension::SD_CORE;
    
    if(ads.size() == 2){ // core or extension
        command = ads[1]; // get the element right after /spatdifcmd
    }else if(ads.size() == 3){  // extension
        std::string extensionName = ads[1];
        extension = sdExtension::stringToExtension(extensionName);
        if(extension == EExtension::SD_EXTENSION_ERROR){
            sdOSCMessage errorMessage("/spatdif/error");
            errorMessage.appendArgument<std::string>("No such extension: " + extensionName);
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
            errorMessage.appendArgument<std::string>("Invalid Comannd");
            returnMessageVector.push_back(errorMessage);
            return returnMessageVector;
        }
    }
    return returnMessageVector;
}

inline sdOSCMessage sdOSCResponder::getEventSetsFromAllEntities(const double &queryTime) const{
    sdOSCMessage returnMessage;
    std::vector<std::pair<const sdEntity* , std::shared_ptr<sdProtoEvent>>> reports;
    if(interval != 0.0){
        reports = scene->getEventsFromAllEntities(queryTime, queryTime+interval); // in the specified region
    }else{
        reports = scene->getEventsFromAllEntities(queryTime);
    }
    
    for(auto rit = reports.begin(); rit != reports.end(); rit++){
        auto report = *rit;
        auto entity = report.first;
        auto event  = report.second;
        auto entityName = scene->getEntityName(entity);
        returnMessage.clear();
        
        returnMessage.setAddress("/spatdif/source/"+entityName+"/"+event->getDescriptorAsString());
        
        switch (event->getDescriptor()){
            case SD_POSITION:
            case SD_ORIENTATION:{
                auto values = dynamic_cast< sdEvent<SD_ORIENTATION> * >(event.get())->getValue();
                std::for_each(values.begin(), values.end(), [&returnMessage](float value){returnMessage.appendArgument<float>(value);});
                break;
            }
            case SD_PRESENT:{
                returnMessage.appendArgument(dynamic_cast< sdEvent<SD_PRESENT> * >(event.get())->getValue());
                break;
            }
            case SD_MEDIA_TYPE:
            case SD_MEDIA_LOCATION:
            case SD_MEDIA_ID:{
                auto values = dynamic_cast< sdEvent<SD_MEDIA_TYPE> * >(event.get())->getValue();
                returnMessage.appendArgument(event->getValueAsString());
                break;
            }
            case SD_MEDIA_CHANNEL:{
                returnMessage.appendArgument<int>(dynamic_cast< sdEvent<SD_MEDIA_CHANNEL> *>(event.get())->getValue());
                break;
            }
            case SD_MEDIA_TIME_OFFSET:
            case SD_MEDIA_GAIN:{
                returnMessage.appendArgument<float>(dynamic_cast< sdEvent<SD_MEDIA_GAIN> *>(event.get())->getValue());
                break;
            }
            default:{
                break;
            }
        }
    }
    return std::move(returnMessage);
}

inline std::vector<sdOSCMessage> sdOSCResponder::getAction(std::string command, sdOSCMessage message){
    sdOSCMessage returnMessage;
    std::vector<sdOSCMessage> returnMessageVector;
    bool singleMessage = true;
    // internal variable
    if(command == "getEventSetsFromAllEntities"){

    }else if(command == "getNextEventTime"){
        
        returnMessage.setAddress("/spatdif/nextEventTime");
        auto nextEvent = scene->getNextEventTime(queryTime);
        if(nextEvent.second) return std::vector<sdOSCMessage>();
        returnMessage.appendArgument(static_cast<float>(nextEvent.first));
        
    }else if(command == "getDeltaTimeToNextEvent"){
        
        returnMessage.setAddress("/spatdif/deltaTime");
        auto deltatime = scene->getDeltaTimeToNextEvent(queryTime);
        if(deltatime.second) return std::vector<sdOSCMessage>();
        returnMessage.appendArgument(deltatime);
        
    }else if(command == "getQueryTime"){
        
        returnMessage.setAddress("/spatdif/queryTime");
        returnMessage.appendArgument<float>(getQueryTime());
        
    }else if(command == "getWriteTime"){
        
        returnMessage.setAddress("/spatdif/writeTime");
        returnMessage.appendArgument<float>(getWriteTime());
        
    }else if(command == "getInterval"){
        
        returnMessage.setAddress("/spatdif/interval");
        returnMessage.appendArgument<float>(getInterval());
        
    }// query about scene{
    else if(command == "getNumberOfEntities"){
        returnMessage.setAddress("/spatdif/numberOfEntities");
        returnMessage.appendArgument<int>(static_cast<int>(scene->getNumberOfEntities()));
    }else if(command == "getEntityNames"){
        returnMessage.setAddress("/spatdif/entityNames");
        auto entityNames = scene->getEntityNames();
        std::for_each(entityNames.begin(), entityNames.end(), [&returnMessage]( std::string name ){
            returnMessage.appendArgument<std::string>(name);
        });
    }else if(command == "getOrdering"){
        returnMessage.setAddress("/spatdif/ordering");
        returnMessage.appendArgument<std::string>(scene->getOrderingAsString());
    }else if(command == "getAuthor"){
        returnMessage.setAddress("/spatdif/author");
        returnMessage.appendArgument<std::string>(scene->getInfo().getAuthor());
    }else if(command == "getHost"){
        returnMessage.setAddress("/spatdif/host");
        returnMessage.appendArgument<std::string>(scene->getInfo().getHost());
    }else if(command == "getDate"){
        returnMessage.setAddress("/spatdif/date");
        returnMessage.appendArgument<std::string>(scene->getInfo().getDateAsString());
    }else if(command == "getLocation"){
        returnMessage.setAddress("/spatdif/location");
        returnMessage.appendArgument<std::string>(scene->getInfo().getLocation());
    }else if(command == "getSession"){
        returnMessage.setAddress("/spatdif/session");
        returnMessage.appendArgument<std::string>(scene->getInfo().getSession());
    }else if(command == "getAnnotation"){
        returnMessage.setAddress("/spatdif/annotation");
        returnMessage.appendArgument<std::string>(scene->getInfo().getAnnotation());
    }else if(command == "getNumberOfActivatedExtensions"){
        returnMessage.setAddress("/spatdif/numberOfActivatedExtensions");
        returnMessage.appendArgument<int>(static_cast<int>(scene->getNumberOfActivatedExtensions()));
    }else if(command == "getActivatedExtensions"){
        returnMessage.setAddress("/spatdif/activatedExtensions");
        returnMessage.appendArgument<int>(static_cast<int>(scene->getNumberOfActivatedExtensions()));
    }
    // core descriptors
    else if(command.find("Position") != std::string::npos){ // contains keyword "Position"
        auto entityName = message.getArgumentAsString(0);
        sdEntity* entity = scene->getEntity(message.getArgumentAsString(0));
        if(!entity)
            return returnMessageVector;
        std::string address = "/spatdif/source/" + entityName + "/position";
        returnMessage.setAddress(address);
        returnMessage.appendArgument<std::string>(entityName);
        const std::array<double, 3>* pos;
        if(command == "getPosition"){
            pos = entity->getValue<SD_POSITION>(queryTime);
        }else if(command == "getNextPosition"){
            pos = entity->getNextValue<SD_POSITION>(queryTime);
        }else if(command == "getPreviousPosition"){
            pos = entity->getPreviousValue<SD_POSITION>(queryTime);
        }
        returnMessage.appendArgument(static_cast<float>(pos->at(0)));
        returnMessage.appendArgument(static_cast<float>(pos->at(1)));
        returnMessage.appendArgument(static_cast<float>(pos->at(2)));

    }else if(command.find("Orientation") != std::string::npos){ // contains keyword "Position"
        auto entityName = message.getArgumentAsString(0);
        sdEntity* entity = scene->getEntity(message.getArgumentAsString(0));
        if(!entity)
            return returnMessageVector;
        std::string address = "/spatdif/source/" + entityName + "/orientation";
        returnMessage.setAddress(address);
        returnMessage.appendArgument<std::string>(entityName);
        const std::array<double,3> *ori;
        if(command == "getOrientation"){
            ori = entity->getValue<SD_ORIENTATION>(queryTime);
        }else if(command == "getNextOrientation"){
            ori = entity->getNextValue<SD_ORIENTATION>(queryTime);
        }else if(command == "getPreviousOrientation"){
            ori = entity->getPreviousValue<SD_ORIENTATION>(queryTime);
        }
        returnMessage.appendArgument(static_cast<float>(ori->at(0)));
        returnMessage.appendArgument(static_cast<float>(ori->at(1)));
        returnMessage.appendArgument(static_cast<float>(ori->at(2)));
    }else if(command.find("Present")){
        auto entityName = message.getArgumentAsString(0);
        sdEntity* entity = scene->getEntity(message.getArgumentAsString(0));
        if(!entity)
            return returnMessageVector;
        bool flag;
        if(command == "getPresent"){
            flag = entity->getValue<SD_PRESENT>(queryTime);
        }else if(command == "getNextPresent"){
            flag = entity->getNextValue<SD_PRESENT>(queryTime);
        }else if(command == "getPreviousPresent"){
            flag = entity->getPreviousValue<SD_PRESENT>(queryTime);
        }
        std::string address = "/spatdif/source/" + entityName + "/present";
        returnMessage.setAddress(address);
        returnMessage.appendArgument<std::string>(entityName);
        returnMessage.appendArgument<int>(flag);
    }else{
        returnMessage.setAddress("/spatdif/error");
        returnMessage.appendArgument<std::string>("invalid command");
        returnMessageVector.push_back(returnMessage);
        return returnMessageVector;
    }
    if(singleMessage) returnMessageVector.push_back(returnMessage);
    return returnMessageVector;
}

inline void sdOSCResponder::setAction(std::string command, sdOSCMessage message, EExtension extension){
    std::string entityName = message.getArgumentAsString(0);
    switch(extension){
        case EExtension::SD_MEDIA:{
            // media extension
            if(command == "setID"){
                std::string mediaID = message.getArgumentAsString(1);
                scene->addEvent<SD_MEDIA_ID>(entityName, writeTime, mediaID);
            }else if(command == "setType"){
                std::string type = message.getArgumentAsString(1);
                scene->addEvent<SD_MEDIA_TYPE>(entityName, writeTime, type);
            }else if(command == "setLocation"){
                std::string location = message.getArgumentAsString(1);
                scene->addEvent<SD_MEDIA_LOCATION>(entityName, writeTime, location);
            }else if(command == "setChannel"){
                int channel = message.getArgument<int>(1);
                scene->addEvent<SD_MEDIA_CHANNEL>(entityName, writeTime, channel);
            }else if(command == "setTimeOffset"){
                double timeOffset = static_cast<double>(message.getArgument<float>(1));
                scene->addEvent<SD_MEDIA_TIME_OFFSET>(entityName, writeTime, timeOffset);
            }else if(command == "setGain"){
                double gain = static_cast<double>(message.getArgument<float>(1));
                scene->addEvent<SD_MEDIA_GAIN>(entityName, writeTime, gain);
            }
            break;
        }
        default:{
            if(command == "setQueryTime"){
                setQueryTime(static_cast<double>(message.getArgument<float>(0)));
            }else if(command == "setQueryTimeToNextEvent"){
                auto nextEventTime = scene->getNextEventTime(queryTime);
                if(!nextEventTime.second) return;
                setQueryTime(nextEventTime.first);
            }else if(command == "setWriteTime"){
                setWriteTime(message.getArgument<float>(0));
            }else if(command == "setInterval"){
                setInterval(message.getArgument<float>(0));
                // core descriptors
                
            }else if(command == "setPosition"){
                scene->addEvent<SD_POSITION>(message.getArgumentAsString(0), writeTime, {message.getArgument<float>(1), message.getArgument<float>(2), message.getArgument<float>(3)});
            }else if(command == "setOrientation"){
                scene->addEvent<SD_ORIENTATION>(message.getArgumentAsString(0), writeTime, {message.getArgument<float>(1), message.getArgument<float>(2), message.getArgument<float>(3)});
            }else if(command == "setPresent"){
                scene->addEvent<SD_PRESENT>(message.getArgumentAsString(0), writeTime, message.getArgument<int>(1));
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


/*! \page osc_commands
List of OSC commands
===================================

The following commands can be accepted and interpreted by sdOSCRespnder.
sdOSCResponder is capable of accepting both OSC messages with typetags and without typetags.
The incomming data will be checked against the spatDIF format and returns /spatdif/error in case the format of the received message is incorrect.

Getter
----------------------------

This group of commands are for setting the index time for writing / querying events stored in the SpatDIF library.
These commands should be executed before the actual queries or modifications of data.

@par /spatdifcmd/getEventSetsFromAllEntities
    returns all events from all entities at the queryTime. If the interval > 0.0, all event between queryTime and 
    queryTime+interval will be returned.@n

@par /spatdifcmd/getNextEventTime
    returns the time of next event in ths scene.@n

@par /spatdifcmd/getDeltaTimeToNextEvent
    returns the delta time to the next event in the scene.@n

@par /spatdifcmd/getQueryTime
    returns current queryTime value stored in the responder.@n
 
@par /spatdifcmd/getWriteTime
    returns current writeTime value stored in the responder.@n

@par /spatdifcmd/getInterval
    returns current interval value stored in the responder.@n

@par /spatdifcmd/getNumberOfEntities
    returns the number of entities in the scene.@n

@par /spatdifcmd/getEntityNames
    returns the names of all entities in the scene.@n

@par /spatdifcmd/getOrdering
    returns the ordering of the scene.@n

@par /spatdifcmd/getAuthor
    returns the author of the scene.@n

@par /spatdifcmd/getHost
    return the host of the scene.@n

@par /spatdifcmd/getDate
    return the date of the scene.@n

@par /spatdifcmd/getLocation
    return the location of the scene.@n

@par /spatdifcmd/getSession
    return the session of the scene.@n

@par /spatdifcmd/getAnnotation
    return the annotation of the scene.@n

@par /spatdifcmd/getNumberOfActivatedExtensions
    return the number of activated extensions in the scene.@n

@par /spatdifcmd/getActivatedExtensions
    return the names of activated extensions. @n

@par /spatdifcmd/getPosition s @a entity
    returns the position of the @a entity at the queryTime.@n

@par /spatdifcmd/getNextPosition s @a entity
    returns the next position of the @a entity from the queryTime.@n

@par /spatdifcmd/getPreviousPosition s @a entity
    returns the previous position of the @a entity from the queryTime.@n

@par /spatdifcmd/getOrientation s @a entity
    returns the orientation of the @a entity at the queryTime.@n

@par /spatdifcmd/getNextOrientation s @a entity
    returns the next orientation of the @a entity from the queryTime.@n

@par /spatdifcmd/getPreviousOrientation s @a entity
    returns the previous orientation of the @a entity from the queryTime.@n

@par /spatdifcmd/getPresent s @a entity
    returns the present of the @a entity at the queryTime.@n

@par /spatdifcmd/getNextPresent s @a entity
    returns the next present of the @a entity from the queryTime.@n

 
Setters
----------------------------


@par /spatdifcmd/setQueryTime f @a queryTime
    set queryTime to @a queryTime.@n

@par /spatdifcmd/setQueryTimeToNextEvent 
    set queryTime to the time of next event in the scene
 
@par /spatdifcmd/setWriteTime f @a writeTime
    set writeTime to @a writeTime.@n
 
@par /spatdifcmd/setInterval f @a interval
    set interval to @a interval.@n

@par /spatdifcmd/setPosition sfff @a entity @a x @a y @a z
    set the position of the specified @a entity to @a x @a y @a z @n
    if the specified entity does not exist within the scene, an entity will be implicitly created. @n

@par /spatdifcmd/setOrientation sfff @a entity @a x @a y @a z
    set the orientation of the specified @a entity to @a x @a y @a z @n
    if the specified entity does not exist within the scene, an entity will be implicitly created. @n

@par /spatdifcmd/setPresent si @a entity @a present
    set the present parameter of the specified @a entity to @a present. @n
    if the specified entity does not exist within the scene, an entity will be implicitly created. @n

@par /spatdifcmd/setOrdering s @a ordering
    set ordering of the scene to @a ordering.@n

@par /spatdifcmd/setAuthor s @a author
    set author of the scene to @a author. @n

@par /spatdifcmd/setHost s @a host
    set host of the scene to @a host.@n

@par /spatdifcmd/setDate s @a date
    set date of the scene to @a date.@n

@par /spatdifcmd/setLocation s @a location
    set location of the scene to @a location.@n

@par /spatdifcmd/setSession s @a session
    set session of the scene to @a session.@n

@par /spatdifcmd/setAnnotation s @a annotation
    set annotation of the scne to @a annotation.@n


@par /spatdifcmd/media/setID ss @a entity @a id
    associate designated media @a id and an @a entity.@n

@par /spatdifcmd/media/setType ss @a entity @a type
    set the media type of @a entity to @a type.@n

@par /spatdifcmd/media/setLocation ss @a entity @a location
    set the location of the file of the media to @a location, associated to @a entity.@n

@par /spatdifcmd/media/setChannel si @a entity @a channel
    set the channel of the media to @a channel.@n

@par /spatdifcmd/media/setTimeOffset sf @a entity @a timeOffset
    set the time offset of the file to @a timeOffset.@n

@par /spatdifcmd/media/setGain sf @a entity @a gain
    set the gain of the media to @a gain.@n
 
Add and Remove
----------------------------

@par /spatdifcmd/addEntity s @a entityName
    add an entity to the scene and name it with @a entityName.

@par /spatdifcmd/removeEntity s @a entityName
    remove an entity named @a entityName from the scene.

@par /spatdifcmd/removeAllEntities 
    remove all entities from the scene. All associated event data will be lost.

@par /spatdifcmd/addExtension s @extension
    activate @extension
*/

#endif /* defined(____sdOSCResponder__) */