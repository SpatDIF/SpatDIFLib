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
 * @copyright Copyright (C) 2013 - 2016 Zurich University of the Arts, Institute for Computer Music and Sound Technology @n
 * This code is licensed under the terms of the "New BSD License" @n
 * http://creativecommons.org/licenses/BSD/
 */

#ifndef ____sdEntity__
#define ____sdEntity__

#include <vector>
#include <string>
#include <memory>
#include "sdException.h"
#include "sdConst.h"
#include "sdMeta.h"
#include "sdEvent.h"
#include "sdMetaHandler.h"
#include "sdEventHandler.h"

/*! sdEntity
 sdEntity is a pure abstract class. This class maintains and handles all events associated to relevant descriptors. This class is also responsible for answering queries about it's relevant descriptors.
*/
class sdScene;
class sdEntity : public sdMetaHandler, public sdEventHandler{
    friend sdScene;

public:
    
    /*! answer if this entity source or sink */
    const EKind &getKind() const;
    std::string getKindAsString() const;
    
    /*! ask parent the name given to this entity */
    std::string getName();
    
    /*! this function is the only way to instantiate sdMeta.*/
    template <EDescriptor D>
    sdMeta<D> * const addMeta(typename sdDescriptor<D>::type value);
    
    /*! add meta by string*/
    const std::shared_ptr<sdProtoMeta> addMeta(const std::string &descriptor, const std::string &value);
    
    /*! return value cast to a specific subclass event.*/
    template <EDescriptor D>
    sdEvent<D> * const addEvent(const double &time, typename sdDescriptor<D>::type value);

    /*! add event by string*/
    const std::shared_ptr<sdProtoEvent> addEvent(const std::string &time, const std::string &descriptor, const std::string &value);
    
    
    /*!
     @}
     */

    /*! @name Getting value directly
     @{
     */

    /*!
     this function looks for an event at specified time and descriptor and returns a pointer to the data.
     @param time time of the event
     @param descriptor specify descriptor defined in sdConst.h
     */
    template <EDescriptor D>
    const typename sdDescriptor<D>::type * const getValue(const double &time) const;
    
    /*!
     this function search for an event at specified time and descriptor and return the data as a string.
     @param time time of the event
     @param descriptor specify descriptor defined in sdConst.h
     */
    template <EDescriptor D>
    const std::string getValueAsString(const double &time) const;

    /*!
     this function looks for a next event from the specified time with the designated descriptor and returns a pointer to the data.
     @param time 
     @param descriptor specify descriptor defined in sdConst.h
     */
    template <EDescriptor D>
    const typename sdDescriptor<D>::type * const getNextValue(const double &time) const;

    /*!
     this function looks for a next event from the specified time with the designated descriptor and returns a pointer to the data.
     @param time 
     @param descriptor specify descriptor defined in sdConst.h
     */
    template <EDescriptor D>
    const typename sdDescriptor<D>::type * const getPreviousValue(const double &time) const;
   /*!
     @} 
    */
    
protected:
    
    /*!sdEntity can be invoked only by sdScene*/
    sdEntity(sdScene * const parent, EKind kind = EKind::SD_SOURCE):parent(parent), kind(kind){};
    
    /*! ask parent if the extension for the descriptor is activated */
    bool isDescriptorValid(const EDescriptor &descriptor) const;
    
    /*! add a pointer a meta to the global event vector. */
    void addGlobalMetaAlias(std::shared_ptr<sdProtoMeta> meta) override;
    
    /*! add a pointer an event to the global event vector */
    void addGlobalEventAlias(std::shared_ptr<sdProtoEvent> event) override;
    
    sdScene * const parent;
    const EKind kind;
};


inline const EKind &sdEntity::getKind() const{
    return kind;
}

inline std::string sdEntity::getKindAsString() const{
    return kind == EKind::SD_SOURCE ? "source": "sink";
}

template <EDescriptor D>
inline sdMeta<D> * const sdEntity::addMeta(typename sdDescriptor<D>::type value){
    if(!isDescriptorValid(D)) return nullptr; // invalid descriptor
    return sdMetaHandler::addMeta<D>(value, this);
}

inline const std::shared_ptr<sdProtoMeta> sdEntity::addMeta(const std::string &descriptor, const std::string &value){
    EDescriptor dtr = sdExtension::stringToDescriptor(descriptor);
    if(dtr == EDescriptor::SD_ERROR) return nullptr; // undefined descriptor
    if(!isDescriptorValid(dtr)) return nullptr; // invalid descriptor
    
    auto addMetaFunc = sdExtension::getAddMetaFunc(std::move(dtr));
    return addMetaFunc(this, value);
}

template <EDescriptor D>
inline sdEvent<D> * const sdEntity::addEvent(const double &time,  typename sdDescriptor<D>::type value){
    if(!isDescriptorValid(D)) return nullptr; // invalid descriptor
    return sdEventHandler::addEvent<D>(time, value, this);
}

inline const std::shared_ptr<sdProtoEvent> sdEntity::addEvent(const std::string &time, const std::string &descriptor, const std::string &value){
    EDescriptor dtr = sdExtension::stringToDescriptor(descriptor);
    if(dtr == EDescriptor::SD_ERROR) return nullptr; // undefined descriptor
    if(!isDescriptorValid(dtr)) return nullptr; // invalid descriptor
    
    auto dtime = std::stod(time);
    auto addEventFunc = sdExtension::getAddEventFunc(std::move(dtr));
    return addEventFunc(this, dtime, value);
}

template <EDescriptor D>
inline const typename sdDescriptor<D>::type * const sdEntity::getValue(const double &time) const{
    auto event = getEvent<D>(time);
    if(!event) return  nullptr;
    return &(event->getValue());
}

template <EDescriptor D>
const std::string sdEntity::getValueAsString(const double &time) const{
    auto event = getEvent<D>(time);
    if(!event) return  std::string();
    return getEvent<D>(time)->getValueAsString();
}

template <EDescriptor D>
const typename sdDescriptor<D>::type * const sdEntity::getNextValue(const double &time) const{
    auto nextEvent = getNextEvent<D>(time);
    if(!nextEvent) return nullptr;
    return &nextEvent->getValue();
}

template <EDescriptor D>
const typename sdDescriptor<D>::type * const sdEntity::getPreviousValue(const double &time) const{
    auto previousEvent = getPreviousEvent<D>(time);
    if(!previousEvent) return nullptr;
    return &previousEvent ->getValue();
}


#endif