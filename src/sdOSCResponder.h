//
//  sdOSCResponder.h
//

#ifndef ____sdOSCResponder__
#define ____sdOSCResponder__

#include "sdScene.h"
#include <string>

using namespace std;

class sdOSCResponder{
    
private:
    /*! The interpreted OSC messages will be sent do this sdScene*/
    sdScene *scene;
    string currentTime;
    vector <string> splitString(const string &str);
    
public:
    /*! constructor. the scene must be created before the instantiation of this class
     @param scene the target scene
     */
    sdOSCResponder(sdScene *scene);
    
    /*! this function interprets incoming string as a OSCMessage and forward it to the scene
     */
    void forwardOSCMessage(string oscMessage);
    
};

#endif /* defined(____sdOSCResponder__) */