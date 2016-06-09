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

#ifndef ____sdSaver__
#define ____sdSaver__

#include <string>
#include "sdScene.h"

#include "tinyxml2.h"

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


protected:
    
    static tinyxml2::XMLElement* XMLInfoSection(tinyxml2::XMLDocument &xml, sdScene *scene);
    static tinyxml2::XMLElement* XMLOrderingSection(tinyxml2::XMLDocument &xml, sdScene *scene);

    static tinyxml2::XMLElement* XMLMetaSection(tinyxml2::XMLDocument &xml, sdScene *scene);
    static tinyxml2::XMLElement* XMLTrajectorySection(tinyxml2::XMLDocument &xml, sdScene *scene);

};

#endif /* defined(____sdSaver__) */
