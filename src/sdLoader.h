//
//  sdLoader.h
//

#ifndef ____sdLoader__
#define ____sdLoader__

#include <string>
#include "sdScene.h"

/*!
    convert XML, JSON, or YAML strings to a sdScene
 */
class sdLoader
{
public:
    
    /*! converts a XML string to a sdScene
     @param xmlString a xml String to be converted
     */
    static sdScene sceneFromXML(std::string xmlString);

    /*! converts a JSON string to a sdScene
     @param jsonString a json String to be converted
     */
    static sdScene sceneFromJSON(std::string jsonString);
    
    /*! converts a YAML string to a sdScene
     @param yamlString a yaml String to be converted
     */
    static sdScene sceneFromYAML(std::string yamlString);
    
};

#endif /* defined(____sdLoader__) */
