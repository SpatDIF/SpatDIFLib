//
//  sdSaver.h
//

#ifndef ____sdSaver__
#define ____sdSaver__

#include <string>
#include "sdScene.h"

using namespace std;

/*!
 convert a sdScene to XML, JSON, or YAML strings
 */
class sdSaver
{
public:
    typedef enum {
        SD_TIME,
        SD_TRACK
    } EOrdering;
    
    /*! converts a scene to XML string
     @param sdScene a sdScene
     */
    static string XMLFromScene(sdScene *scene, EOrdering ordering = SD_TIME);

    /*! converts a scene to JSON string
     @param sdScene a sdScene
     */
    static string JSONFromScene(sdScene *scene);
    
    /*! converts a scene to JSON string
     @param sdScene a sdScene
     */
    static string YAMLFromScene(sdScene *scene);
};

#endif /* defined(____sdSaver__) */
