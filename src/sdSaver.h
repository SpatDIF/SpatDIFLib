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

#include "tinyxml2.h"


template <EFormat F>
class sdSaver{};

template<>
class sdSaver<EFormat::SD_XML>{
    
public:
    static std::string toString(const sdScene &scene);

protected:
    static tinyxml2::XMLDeclaration* XMLDeclarationSection(tinyxml2::XMLDocument &xmle);
    static tinyxml2::XMLElement* XMLHeaderSection(tinyxml2::XMLDocument &xml);
    static tinyxml2::XMLElement* XMLInfoSection(tinyxml2::XMLDocument &xml, const sdScene &scene);
    static void XMLDataSets(tinyxml2::XMLDocument &xml, const sdScene &scene, tinyxml2::XMLElement *metaSection);
    static tinyxml2::XMLElement* XMLOrderingSection(tinyxml2::XMLDocument &xml, const sdScene &scene);

    static tinyxml2::XMLElement* XMLMetaSection(tinyxml2::XMLDocument &xml, const sdScene &scene);
    static tinyxml2::XMLElement* XMLTrajectorySection(tinyxml2::XMLDocument &xml, const sdScene &scene);
};

template<>
class sdSaver<EFormat::SD_JSON>{
public:
    static std::string toString(const sdScene &scene);
    
};

template<>
class sdSaver<EFormat::SD_YAML>{
public:
    static std::string toString(const sdScene &scene);
};