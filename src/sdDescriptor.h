//
//  sdDescriptor.h
//  libSpatDIFTest
//
//  Created by Chikashi Miyama on 14/10/2016.
//  Copyright © 2016 ICST. All rights reserved.
//

#pragma once
#include "sdDescriptorSpec.h"

class sdDescriptor{
public:
    sdDescriptor(EDescriptor descriptor):descriptor(descriptor){}
    /*!
     returns event descriptor
     @returns event descriptor ENUM
     */
    EDescriptor getDescriptor() const {return descriptor;}
    
    /*!
     @returns descriptor as string
     */
    std::string getDescriptorAsString() const {return sdSpec::descriptorToString(descriptor);}
    
protected:
    const EDescriptor descriptor /*< the descriptor type of meta. unmutable */;
};
