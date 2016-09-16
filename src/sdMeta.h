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

#include <string>
#include "sdDescriptor.h"
#include "sdSpec.h"
#include "sdUtils.h"
#include "sdData.h"

#pragma mark definitions
class sdEntity;

/**
 * @brief  An abstract class that connect an sdEntity and descriptor.
 * @detail This class holds a descriptor and a pointer to the parent Entity
 */

class sdProtoMeta : public sdProtoData{

public:

    /*!
     returns an immutable pointer to its parent entity
     @returns a pointer to its parent entity
     */
    const sdProtoEntity * const getParent() const;

protected:
    const sdProtoEntity * const parent;/*!< a pointer to the belonging entity. unmutable */
    sdProtoMeta(const EDescriptor descriptor, const sdProtoEntity * const parent);

};

inline sdProtoMeta::sdProtoMeta(const EDescriptor descriptor, const sdProtoEntity * const parent):
sdProtoData(descriptor),
parent(parent)
{}


inline const sdProtoEntity * const sdProtoMeta::getParent() const{
    return parent;
}

/**
 * @brief class template for meta setting
 * @detail the value type, that the event handles, is based on given descriptor D.
 */
template<EDescriptor D>
class sdMeta: public sdProtoMeta{
    friend sdEntity;
    
protected:
    const typename sdDescriptor<D>::type value; /*< the value of the event. the type of value is determined by EDescriptor D */
    
public:
    
    /*! constructor with initialization. should be invoked by the subclass*/
    sdMeta(const sdProtoEntity * const parent, const typename sdDescriptor<D>::type &value):
    sdProtoMeta(D, parent), value(value){}
    
    /*! @name Value handling
     @{ */
    
    /*! returns value. */
    const typename sdDescriptor<D>::type &getValue(void) const{ return value; };
    
    /*! returns value as string */
    const std::string getValueAsString() const override{ return sdDescriptor<D>::toString(value);};
    /*! @} */
    
};
