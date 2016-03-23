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

#ifndef ____sdEvent__
#define ____sdEvent__

#include <string>
#include "sdConst.h"
#include "sdMeta.h"

#pragma mark definitions
class sdEntity;
/**
 * @brief  An abstract class of event inherited from sdProtoMeta. This class maintains additionaly time value.
 * @detail This class can only be used by sdEntity. All member variables are declared as const. Thus, all parameter must be set by initializer list. Modification of events are not possible in SpatDIF Library. Discard the instance, create new one, and move the unchanged data to new one, if modification of member data is required.
 */

class sdProtoEvent : public sdProtoMeta{
protected:
    /*! protected constructor. only friend of this class can invoke */
    sdProtoEvent(const double time, const EDescriptor descriptor, const sdEntity * const parent);
    const double time; /*< the absolute time of event. unmutable */

public:
    /*!
     return event time 
     @returns the time of event in second
    */
    double getTime() const;
    
    /*!
     returns event time as std::string
     @returns the time of event as a std::string
     */
    std::string getTimeAsString() const;
    
 };

inline sdProtoEvent::sdProtoEvent(const double time, const EDescriptor descriptor, const sdEntity * const parent):
time(time),
sdProtoMeta(descriptor, parent)
{}

inline double sdProtoEvent::getTime() const{
    return time;
}

inline std::string sdProtoEvent::getTimeAsString() const {
    return std::to_string(time);
}


/**
 * @brief class template for event
 * @detail the value type, that the event handles, is based on given descriptor D.
 */
template<EDescriptor D>
class sdEvent: public sdProtoEvent{
    friend sdEntity;
    
protected:
    const typename sdDescriptor<D>::type  value; /*< the value of the event. the type of value is determined by EDescriptor D */
    
public:
    
    /*! constructor with initialization. should be invoked by the subclass*/
    sdEvent(const double time, const sdEntity * const parent, const typename sdDescriptor<D>::type &value):
    sdProtoEvent(time, D, parent), value(value){}
    
    /*! @name Value handling
     @{ */
    
    /*! returns value. */
    const typename sdDescriptor<D>::type &getValue(void) const{ return value; };
    
    /*! returns value as string */
    const std::string getValueAsString() const override{ return toString(value);};
    /*! @} */

};


#endif
