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

std::string sdEntity::getName(){
    return parent->getEntityName(this);
}

bool sdEntity::isDescriptorValid(const EDescriptor &descriptor) const{
    return parent->isDescriptorValid(descriptor);
}

void sdEntity::addGlobalMetaAlias(std::shared_ptr<sdProtoMeta> meta) {
    parent->addMetaAlias(this, meta);
}

void sdEntity::addGlobalEventAlias(std::shared_ptr<sdProtoEvent> event) {
    parent->addEventAlias(this, event);
}
