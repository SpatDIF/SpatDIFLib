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


bool sdEntity::isDescriptorValid(const EDescriptor &descriptor) const{
    return parent->isDescriptorValid(descriptor);
}

void sdEntity::addGlobalEventAlias(std::shared_ptr<sdProtoEvent> event) {
    parent->addEventAlias(event);
}
