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

#include "rapidxml.hpp"

using namespace rapidxml;

class sdXMLSaver{
    
public:
    static std::string toString(const sdScene &scene);

private:
    static xml_node<> * createOrderingSection(xml_document<> &xml, const sdScene &scene);
    static xml_node<> * createMetaSection(xml_document<> &xml, const sdScene &scene);
    static xml_node<> * createExtentionSection(xml_document<> &xml, const sdScene &scene);

    static xml_node<> * XMLHeaderSection(xml_document<> &xml);
    static xml_node<> * XMLInfoSection(xml_document<> &xml, const sdScene &scene);
    static xml_node<> * XMLDataSets(xml_document<> &xmle, const sdScene &scene, xml_node<> *metaSection);

    static xml_node<> XMLTrajectorySection(xml_document<> &xml, const sdScene &scene);
};

class sdJSONSaver{
public:
    static std::string toString(const sdScene &scene);
    
};

class sdYAMLSaver{
public:
    static std::string toString(const sdScene &scene);
};

