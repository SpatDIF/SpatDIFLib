/** @file
 *
 * @ingroup spatdiflib
 *
 * @brief
 *
 * @details
 *
 * @authors Chikashi Miyama
 *
 * @copyright Copyright (C) 2013 - 2014 Zurich University of the Arts, Institute for Computer Music and Sound Technology @n
 * This code is licensed under the terms of the "New BSD License" @n
 * http://creativecommons.org/licenses/BSD/
 */

#pragma once

/**
 * @brief  An abstract class that hold a descriptor
 */

class sdProtoData{
public:
    sdProtoData(const EDescriptor descriptor):descriptor(descriptor){};
    /*!
     returns event descriptor
     @returns event descriptor ENUM
     */
    EDescriptor getDescriptor() const;
    
    /*!
     @returns descriptor as string
     */
    std::string getDescriptorAsString() const;
    
    
    /*!
     returns value of the event as a std::string
     @returns a std::string that contains string
     */
    virtual const std::string getValueAsString() const = 0;
    
protected:
    const EDescriptor descriptor /*< the descriptor type of meta. unmutable */;
};

inline EDescriptor sdProtoData::getDescriptor() const{
    return descriptor;
}

inline std::string sdProtoData::getDescriptorAsString() const{
    return sdSpec::descriptorToString(descriptor);
}

template <EDescriptor D>
class sdData :public sdProtoData{
    
protected:
    const typename sdDescriptorSpec<D>::type value; /*< the value of the event. the type of value is determined by EDescriptor D */
    
public:
    sdData(const typename sdDescriptorSpec<D>::type &value):sdProtoData(D), value(value){}
    const typename sdDescriptorSpec<D>::type &getValue(void) const{ return value; };
    const std::string getValueAsString() const override{ return sdDescriptorSpec<D>::toString(value);};
};

