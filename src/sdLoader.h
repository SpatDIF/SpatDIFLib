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

#pragma once
#include <string>
#include "sdScene.h"


/*!
    convert XML, JSON, or YAML strings to a sdScene
 */
class sdXMLLoader
{
public:
    
    /*! converts a XML string to a sdScene
     @param xmlString a xml String to be converted
     */
    static sdScene sceneFromXML(std::string xmlString);

private:
    static void activateExtensions(sdScene &scene, const std::string &extensionString);
    
};


