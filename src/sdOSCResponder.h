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

#ifndef ____sdOSCResponder__
#define ____sdOSCResponder__

#include "sdScene.h"
#include <string>

using namespace std;

class sdOSCResponder{
    
private:
    /*! The interpreted OSC messages will be sent do this sdScene*/
    sdScene *scene;
    
    string queryTimeString; /*< contains current time of responder as a string. */
    
    /*! private utility function for spliting string with slash*/
    vector <string> splitString(const string &str);
    
    /*!
     @param expectedNumber expected number of arguments
     @param actualNumber actual number of arguments
     @param command command name
     */
    bool checkNumberOfArguments(int expectedNumber, int actualNumber, string command);
    
public:
    /*! constructor. the scene must be created before the instantiation of this class
     @param scene the target scene
     */
    sdOSCResponder(sdScene *scene);
    
    /*! this function interprets incoming string as a OSCMessage and forward it to the scene
     @param oscMessage a OSC string to be interpreted and forwarded to the library
     */
    string forwardOSCMessage(string oscMessage);
    
    /*!
     overloaded funtion.
     @param oscMessage a OSC string to be interpreted and forwarded to the library
     @param time set current time to this value before forwarding the message
     */
    void forwardOSCMessage(string oscMessage, double time);

    /*!
     @param time set current time to this value
     */
    void setQueryTime(double time);
    
    
};

#endif /* defined(____sdOSCResponder__) */