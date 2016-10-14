/** @file
 *
 * @ingroup spatdiflib
 *
 * @brief an abstract class of spat DIF event and its template subclass
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

#include "sdValue.h"
#include "sdProtoMeta.h"

/**
 * @brief class template for meta setting
 * @detail the value type, that the event handles, is based on given descriptor D.
 */
template<EDescriptor D>
class sdMeta: public sdProtoMeta, public sdValue<D>{
    friend sdEntity;
    
public:
    
    /*! constructor with initialization. should be invoked by the subclass*/
    sdMeta(const sdEntity * const parent, const typename sdDescriptorSpec<D>::type &value):
    sdProtoMeta(D, parent), sdValue<D>(value){}
    std::string getValueAsString() const{ return sdValue<D>::getValueAsString() ;}
};
