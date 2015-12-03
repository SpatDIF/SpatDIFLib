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

#ifndef ____sdEvent__
#define ____sdEvent__

#include <string>
#include "sdConst.h"

#pragma mark definitions

/**
 * @brief  An abstract class of event. This class maintains time, descriptor
 * @detail This class can only be used by sdEntity. all member variables are declared as conast. Thus, all parameter must be set by initializer list. Discard the instance, create new one, and move the unchanged data to new one, using move semantics, if modification of member data is required.
 */

class sdEntity;
class sdProtoEvent{
protected:
    /*! protected constructor. only friend of this class can invoke */
    sdProtoEvent(const double time, const EDescriptor descriptor, const sdEntity * const parent);

    const sdEntity * const parent;/*!< a pointer to the belonging entity. unmutable */
    const double time; /*< the absolute time of event. unmutable */
    const EDescriptor descriptor /*< the descriptor type of event. unmutable */;

public:
    double getTime() const;
    std::string getTimeAsString() const;
    EDescriptor getDescriptor() const;
    const sdEntity * const getParent() const;
    virtual std::string getValueAsString() = 0;
};

inline sdProtoEvent::sdProtoEvent(const double time, const EDescriptor descriptor, const sdEntity * const parent):
time(time), descriptor(descriptor), parent(parent){}

inline double sdProtoEvent::getTime() const{
    return time;
}

inline EDescriptor sdProtoEvent::getDescriptor() const{
    return descriptor;
}

inline std::string sdProtoEvent::getTimeAsString() const {
    return std::move(doubleToString(time));
}

inline const sdEntity * const sdProtoEvent::getParent() const{
    return parent;
}

/**
 * @brief class template for event
 * @detail the value type, that the event handles, is based on given descriptor D.
 */

template<typename T>
class sdEvent: sdProtoEvent{
    friend sdEntity;
    
protected:
    const T value;
    
public:
    
    /*! constructor with initialization. should be invoked by the subclass*/
    sdEvent(const double time, const EDescriptor descriptor, const sdEntity * const parent, const T value):
    sdProtoEvent(time, descriptor, parent), value(value){}
    
    /*! @name Value handling
     @{ */
    
    /*!returns value. */
    const T &getValue(void) const{ return value; };
    
    /*! @} */
};

#endif
