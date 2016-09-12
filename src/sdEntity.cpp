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

#include "sdScene.h"
#include "sdEntity.h"

// all functions which need to access the parental sdScene instance

std::string sdEntity::getName() const{
    if(!parent)return std::string("");
    return parent->getEntityName(this);
}

bool sdProtoEntity::isDescriptorValid(const EDescriptor &descriptor) const{
    if(!parent)return false;
    return parent->isDescriptorValid(descriptor);
}

void sdProtoEntity::addGlobalMetaAlias(std::shared_ptr<sdProtoMeta> meta) {
    if(!parent)return;
    parent->addMetaAlias(this, meta);
}

void sdProtoEntity::addGlobalEventAlias(std::shared_ptr<sdProtoEvent> event) {
    if(!parent)return;
    parent->addEventAlias(this, event);
}

std::shared_ptr<sdProtoDataSet> sdEntity::getProtoDataSetPtr(EExtension extension, std::string identifier) const{
    if(!parent)return nullptr;
    return parent->getProtoDataSet(extension,identifier);
}
