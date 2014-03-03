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
    This class interpret incoming OSC messages and forward it to a specified instance of sdScene.
    The response from sdScene will be formatted in OSC format and returned to the client.
    
    The List commands can be found in the following page.
    - \subpage osc_commands
 
 */
class sdOSCResponder{
    
private:
    /*! The interpreted OSC messages will be sent do this sdScene*/
    sdScene *scene;
    
    double queryTime;
    double writeTime;
    double interval;
    
    /*! private utility function for spliting string with slash*/
    vector <string> splitString(const string &str);
    
    /*!
     @param expectedNumber expected number of arguments
     @param actualNumber actual number of arguments
     @param command command name
     @param errorMessage the error message will be stored in this stirng
     */
    bool checkNumberOfArguments(int expectedNumber, int actualNumber, string command, string &errorMessage);


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
    vector<vector<char> > forwardOSCMessage(vector<char> message);

    /*! this is the overloaded high level interface function of the function above.
     easier to handle but less compatible.
     @param message an instance sdOSCMessage
     */
    vector<sdOSCMessage> forwardOSCMessage(sdOSCMessage message);
    
    /*!
     @param time set queryTime to this value
     */
    void setQueryTime(double time);

    /*!
     @param time set writeTime to this value
     */
    void setWriteTime(double time);

    /*!
     @param time set interval to this value
     */
    void setInterval(double time);
    
    
    /*!
     returns queryTime
     */
    double getQueryTime(void);
    
    /*!
     returns writeTime
     */
    double getWriteTime(void);
    
    /*!
     returns interval
     */
    double getInterval(void);
    
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