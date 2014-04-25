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

using namespace std;

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
    vector <string> splitString(const string &str);
    vector<sdOSCMessage> getAction(string command, sdOSCMessage message);
    void setAction(string command, sdOSCMessage message, EExtension extension);

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
    inline void setScene(sdScene *scene);
    
    /*! getTargetScene
     */
    inline sdScene* getScene(void);
    
    /*! this function interprets incoming raw OSCMessage and forward it to the scene
     @param message raw oscMessage, consisting of multiple blocks (4 byte blocks)
     */
    vector<vector<unsigned char> > forwardOSCMessage(vector<unsigned char> message);

    /*! this is the overloaded high level interface function of the function above.
     easier to handle but less compatible.
     @param message an instance sdOSCMessage
     */
    vector<sdOSCMessage> forwardOSCMessage(sdOSCMessage message);
    
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



void sdOSCResponder::setScene(sdScene *scene){
    sdOSCResponder::scene = scene;
}

sdScene* sdOSCResponder::getScene(void){
    return  scene;
}

void sdOSCResponder::setQueryTime(double time){
    queryTime = time;
}

void sdOSCResponder::setWriteTime(double time){
    writeTime = time;
}

void sdOSCResponder::setInterval(double time){
    interval =  time;
}

double sdOSCResponder::getQueryTime(void){
    return queryTime;
}

double sdOSCResponder::getWriteTime(void){
    return writeTime;
}

double sdOSCResponder::getInterval(void){
    return interval;
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