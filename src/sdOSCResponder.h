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
 * @copyright Copyright © 2013 by ??? @n
 * This code is licensed under the terms of the "New BSD License" @n
 * http://creativecommons.org/licenses/BSD/
 */

#ifndef ____sdOSCResponder__
#define ____sdOSCResponder__

#include "sdScene.h"
#include <string>

using namespace std;

class sdOSCResponder{
    
private:
    /*! The interpreted OSC messages will be sent do this sdScene*/
    sdScene *scene;
    
    string currentTimeString; /*< contains current time of responder as a string. */
    
    /*! private utility function for spliting string with slash*/
    vector <string> splitString(const string &str);
    
public:
    /*! constructor. the scene must be created before the instantiation of this class
     @param scene the target scene
     */
    sdOSCResponder(sdScene *scene);
    
    /*! this function interprets incoming string as a OSCMessage and forward it to the scene
     @param oscMessage a OSC string to be interpreted and forwarded to the library
     */
    void forwardOSCMessage(string oscMessage);
    
    /*!
     overloaded funtion.
     @param oscMessage a OSC string to be interpreted and forwarded to the library
     @param time set current time to this value before forwarding the message
     */
    void forwardOSCMessage(string oscMessage, double time);

    /*!
     @param time set current time to this value
     */
    void setCurrentTime(double time);
};

#endif /* defined(____sdOSCResponder__) */