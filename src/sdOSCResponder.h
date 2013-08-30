//
//  sdOSCResponder.h
//

#ifndef ____sdOSCResponder__
#define ____sdOSCResponder__

#include "sdScene.h"

using namespace std;

class sdOSCResponder{
    
private:
    /*! The interpreted OSC messages will be sent do this sdScene*/
    sdScene *scene;
    float time;
    
public:
    /*! the scene must be created before the instantiation of this class
     @param scene the target scene
     */
    
    /*! constructor. a pointer to the target scene should be provided*/
    sdOSCResponder(sdScene *scene);
    
    /*! this function interprets incoming string as a OSCMessage and forward it to the scene
     */
    void forwardOSCMessage(string oscMessage);
    
};

#endif /* defined(____sdOSCResponder__) */