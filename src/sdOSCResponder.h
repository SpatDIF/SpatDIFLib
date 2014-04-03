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
 
 The following commands can be accepted and interpreted by sdOSCRespnder. Both OSC messages with typetags and without typetags can be accepted. The incomming data will be checked against the spatDIF format and returns /spatdif/error in case the format is incorrect.
 
 Setting / Getting time index
 ----------------------------
 
 This group of commands are used to set the index time for writing / querying events stored in the SpatDIF library.
 These commands should be executed before the actual queries or modifications of data.
 
 @par  /spatdifcmd/getQueryTime
    returns current queryTime stored in the responder.@n
 
    returned message: /spatdif/queryTime d @a time
 
 @par /spatdifcmd/getWriteTime
    returns current writeTime stored in the responder.@n
 
    returned message: /spatdif/writeTime d @a time

 @par /spatdifcmd/setQueryTime d @a time
    set queryTime to designated @a time.@n
    @a time (double)... new query time.@n
 

 @par /spatdifcmd/setWriteTime d @a time
    set writeTime to designated @a time.@n
    @a time (double)... new write time.@n
 

 
 Modification
 ----------------------------
 @par /spatdifcmd/setPosition @a entity @a x @a y @a z
    set the position of the specified @a entity to @a x @a y @a z @n
    if the specified entity does not exist within the scene, an entity will be implicitly created. @n
    @a entity (string) ... the name of the target entity. @n
    @a x, y, z (double)... position in 3D space. @n
 
 @par /spatdifcmd/setOrientation @a entity @a x @a y @a z
    set the orentation of the specified @a entity to @a x @a y @a z @n
    @a entity (string) ... the name of the target entity @n
    @a x, y, z (double)... orientation in 3D space @n

 @par /spatdifcmd/setPresent @a entity @a present
    set the present parameter of the specified @a entity to @a present. @n
    @a present (int) ... present if non zero. @n
 
 
 Queries
 ----------------------------
 @par /spatdifcmd/getOrdering
    returns ordering as a symbol
    returned message: /spatdif/ordering s @a ordering
 
 @par /spatdifcmd/getNumberOfEntities
    returns the number of entity in the scene @n
    returned message: /spatdif/numberOfEntities i @a number
 
 @par /spatdifcmd/getEntityNames
    returns the list of entity names. @n
    returned message: /spatdif/numberOfEntities s... @a entityName ... @n

 @par /spatdifcmd/getPosition @a entity
 
    returned message: /spatdif/writeTime ddd @a x @a y @a z@n

 @par /spatdifcmd/getOrientation @a entity
 
    returned message: /spatdif/writeTime ddd @a x @a y @a z@n

 @par /spatdifcmd/getPresent @a entity
 
    returned message: /spatdif/writeTime i @a present

 @par /spatdifcmd/getNextPosition @a entity
 @par /spatdifcmd/getNextOrientation @a entity
 @par /spatdifcmd/getNextPresent @a entity
 
 @par /spatdifcmd/getPreviousPosition @a entity
 @par /spatdifcmd/getPreviousOrientation @a entity
 @par /spatdifcmd/getPreviousPresent @a entity

*/

#endif /* defined(____sdOSCResponder__) */