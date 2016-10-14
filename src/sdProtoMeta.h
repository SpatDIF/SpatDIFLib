//
//  sdProtoMeta.h
//  libSpatDIFTest
//
//  Created by Chikashi Miyama on 14/10/2016.
//  Copyright © 2016 ICST. All rights reserved.
//

#pragma once
#include "sdDescriptor.h"

/**
 * @brief  An abstract class that connect an sdEntity and descriptor.
 * @detail This class holds a descriptor and a pointer to the parent Entity
 */

class sdEntity;
class sdProtoMeta: public sdDescriptor{
public:
    /*!
     returns an immutable pointer to its parent entity
     @returns a pointer to its parent entity
     */
    const sdEntity * const getParent() const {return parent;}
    
    virtual std::string getValueAsString() const = 0;
    
protected:
    sdProtoMeta(const EDescriptor descriptor, const sdEntity * const parent):
    sdDescriptor(descriptor),
    parent(parent)
    {}
    
    const sdEntity * const parent;/*!< a pointer to the belonging entity. unmutable */
    
};
