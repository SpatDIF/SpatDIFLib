//
//  sdSaver.h
//

#ifndef ____sdSaver__
#define ____sdSaver__

#include <string>
#include "sdScene.h"

/*!
 convert a sdScene to XML, JSON, or YAML strings
 */
class sdSaver
{
public:
    /*! converts a scene to XML string
     @param sdScene a sdScene
     */
    static std::string XMLFromScene(sdScene *scene);

    /*! converts a scene to JSON string
     @param sdScene a sdScene
     */
    static std::string JSONFromScene(sdScene *scene);
    
    /*! converts a scene to JSON string
     @param sdScene a sdScene
     */
    static std::string YAMLFromScene(sdScene *scene);
};

#endif /* defined(____sdSaver__) */
