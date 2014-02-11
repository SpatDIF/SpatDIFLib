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

#ifndef ____sdLoader__
#define ____sdLoader__

#include <string>
#include "sdScene.h"

using namespace std;

/*!
    convert XML, JSON, or YAML strings to a sdScene
 */
class sdLoader
{
public:
    
    /*! converts a XML string to a sdScene
     @param xmlString a xml String to be converted
     */
    static sdScene sceneFromXML(string xmlString);

    /*! converts a JSON string to a sdScene
     @param jsonString a json String to be converted
     */
    static sdScene sceneFromJSON(string jsonString);
    
    /*! converts a YAML string to a sdScene
     @param yamlString a yaml String to be converted
     */
    static sdScene sceneFromYAML(string yamlString);
    
};


#endif /* defined(____sdLoader__) */
