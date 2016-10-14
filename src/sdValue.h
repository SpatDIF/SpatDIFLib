//
//  sdValue.h
//  libSpatDIFTest
//
//  Created by Chikashi Miyama on 14/10/2016.
//  Copyright © 2016 ICST. All rights reserved.
//

#pragma once
#include "sdDescriptorSpec.h"

template <EDescriptor D>
class sdValue{
public:
    sdValue(const typename sdDescriptorSpec<D>::type &value):value(value){}
    const typename sdDescriptorSpec<D>::type &getValue(void) const{ return value; }
    const std::string getValueAsString() const { return sdDescriptorSpec<D>::toString(value);}
protected:
    const typename sdDescriptorSpec<D>::type value;
};
