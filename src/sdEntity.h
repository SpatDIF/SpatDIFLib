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

#pragma once
#include <vector>
#include <string>
#include <memory>
#include "sdException.h"
#include "sdDescriptor.h"
#include "sdDataSet.h"
#include "sdMeta.h"
#include "sdEvent.h"
#include "sdMetaHandler.h"
#include "sdEventHandler.h"

/*! sdEntity
 sdEntity is a pure abstract class. This class maintains and handles all events associated to relevant descriptors. This class is also responsible for answering queries about it's relevant descriptors.
*/
class sdScene;
class sdProtoEntity : public sdMetaHandler, public sdEventHandler{
    friend sdScene;

public:
    
    virtual std::string getName() const = 0;


    /*! this function is the only way to instantiate sdMeta.*/
    template <EDescriptor D>
    sdMeta<D> * const addMeta(typename sdDescriptor<D>::type value);
    
    /*! add meta by string*/
    const std::shared_ptr<sdProtoMeta> addMeta(const std::string &descriptor, const std::string &extension, const std::string &value);
    
    /*! return value cast to a specific subclass event.*/
    template <EDescriptor D>
    sdEvent<D> * const addEvent(const double &time, typename sdDescriptor<D>::type value);

    /*! add event by string*/
    const std::shared_ptr<sdProtoEvent> addEvent(const std::string &time, const std::string &extension, const std::string &descriptor, const std::string &value);
    
    
    /*!
     @}
     */

    /*! @name Getting value directly
     @{
     */


    /*!
     this function looks for a meta of the specified descriptor and returns a pointer to the data.
     @param descriptor specify descriptor defined in sdDescriptors.h
     */
    template <EDescriptor D>
    inline const typename sdDescriptor<D>::type * const getValue() const;


    /*!
     this function search for a meta of the specified  descriptor and return the data as a string.
     @param descriptor specify descriptor defined in sdDescriptors.h
     */
    template <EDescriptor D>
    const std::string getValueAsString() const;
    
    /*!
     this function search for an event at specified time and descriptor and return the data as a string.
     @param time time of the event
     @param descriptor specify descriptor defined in sdDescriptors.h
     */
    template <EDescriptor D>
    const std::string getValueAsString(const double &time) const;

    /*!
     this function looks for a next event from the specified time with the designated descriptor and returns a pointer to the data.
     @param time 
     @param descriptor specify descriptor defined in sdDescriptors.h
     */
    template <EDescriptor D>
    const typename sdDescriptor<D>::type * const getNextValue(const double &time) const;

    /*!
     this function looks for a next event from the specified time with the designated descriptor and returns a pointer to the data.
     @param time 
     @param descriptor specify descriptor defined in sdDescriptors.h
     */
    template <EDescriptor D>
    const typename sdDescriptor<D>::type * const getPreviousValue(const double &time) const;
   /*!
     @} 
    */
    
protected:
    
    /*!sdEntity can be invoked only by sdScene*/
    sdProtoEntity(sdScene * const parent):parent(parent){};

    /*! ask parent if the extension for the descriptor is activated */
    bool isDescriptorValid(const EDescriptor &descriptor) const;
    
    /*! add a pointer a meta to the global event vector. */
    void addGlobalMetaAlias(std::shared_ptr<sdProtoMeta> meta) override;
    
    /*! add a pointer an event to the global event vector */
    void addGlobalEventAlias(std::shared_ptr<sdProtoEvent> event) override;
    
    sdScene * parent;
    
    
};

template <EDescriptor D>
inline sdMeta<D> * const sdProtoEntity::addMeta(typename sdDescriptor<D>::type value){
    if(!isDescriptorValid(D)) return nullptr; // invalid descriptor
    return sdMetaHandler::addMeta<D>(value, this);
}

inline const std::shared_ptr<sdProtoMeta> sdProtoEntity::addMeta(const std::string &extension, const std::string &descriptor, const std::string &value){
    EDescriptor dtr = sdSpec::stringToDescriptor(extension, descriptor);
    if(dtr == EDescriptor::SD_ERROR) return nullptr; // undefined descriptor
    if(!isDescriptorValid(dtr)) return nullptr; // invalid descriptor
    
    auto addMetaFunc = sdSpec::getAddMetaFunc(std::move(dtr));
    return addMetaFunc(this, value);
}


template <EDescriptor D>
inline sdEvent<D> * const sdProtoEntity::addEvent(const double &time,  typename sdDescriptor<D>::type value){
    if(!isDescriptorValid(D)) throw InvalidDescriptorException(sdSpec::descriptorToString(D) + " is not valid. Activate corresponding extension before using it."); // invalid descriptor
    return sdEventHandler::addEvent<D>(time, value, this);
}

inline const std::shared_ptr<sdProtoEvent> sdProtoEntity::addEvent(const std::string &time, const std::string &extension, const std::string &descriptor, const std::string &value){
    EDescriptor dtr = sdSpec::stringToDescriptor(extension, descriptor);
    if(dtr == EDescriptor::SD_ERROR) return nullptr; // undefined descriptor
    if(!isDescriptorValid(dtr)) return nullptr; // invalid descriptor
    
    auto dtime = std::stod(time);
    auto addEventFunc = sdSpec::getAddEventFunc(std::move(dtr));
    return addEventFunc(this, dtime, value);
}



template <EDescriptor D>
inline const std::string sdProtoEntity::getValueAsString() const{
    auto meta = getMeta<D>();
    if(!meta) return  std::string();
    return meta->getValueAsString();
}

template <EDescriptor D>
inline const std::string sdProtoEntity::getValueAsString(const double &time) const{
    auto event = getEvent<D>(time);
    if(!event) return  std::string();
    return event->getValueAsString();
}

template <EDescriptor D>
inline const typename sdDescriptor<D>::type * const sdProtoEntity::getNextValue(const double &time) const{
    auto nextEvent = getNextEvent<D>(time);
    if(!nextEvent) return nullptr;
    return &nextEvent->getValue();
}

template <EDescriptor D>
inline const typename sdDescriptor<D>::type * const sdProtoEntity::getPreviousValue(const double &time) const{
    auto previousEvent = getPreviousEvent<D>(time);
    if(!previousEvent) return nullptr;
    return &previousEvent ->getValue();
}



class sdEntity : public sdProtoEntity{
    friend sdScene;

public:
    sdEntity(sdScene * const parent, EKind kind = EKind::SD_SOURCE):
    sdProtoEntity(parent),
    kind(kind){};

    virtual std::string getName() const override;
    
    const EKind &getKind() const{return kind;};
    std::string getKindAsString() const{ return kind == EKind::SD_SOURCE ? "source": "sink";};

    template <EDescriptor D>
    const typename sdDescriptor<D>::type * const getValue() const;
    
    template <EDescriptor D>
    const typename sdDescriptor<D>::type * const getValue(const double &time) const;
    
protected:
    EKind kind;
private:
    std::shared_ptr<sdProtoDataSet> getProtoDataSetPtr(EExtension extension, std::string identifier) const;
    
};


template <EDescriptor D>
inline const typename sdDescriptor<D>::type * const sdEntity::getValue() const{
    auto meta = getMeta<D>();
    if(!meta)return nullptr;
    return &(meta->getValue());
}

template <EDescriptor D>
inline const typename sdDescriptor<D>::type * const sdEntity::getValue(const double &time) const{
    auto event = getEvent<D>(time);
    if(!event){
        try{
            EExtension targetExtension = sdSpec::getExtensionOfDescriptor(D);
            EDescriptor idDescriptor = sdSpec::getIDDescriptorOfExtension(targetExtension);
            std::vector<std::shared_ptr<sdProtoEvent>>  eventsAtTime = getEvents(time);
            auto it = std::find_if(eventsAtTime.begin(), eventsAtTime.end(), [&idDescriptor](std::shared_ptr<sdProtoEvent> &event)->bool{
                return event->getDescriptor() == idDescriptor;
            });
            if(it == eventsAtTime.end()) return nullptr;
            sdProtoEvent* idDescriptorEvent = (*it).get();
            std::string identifier = idDescriptorEvent->getValueAsString();
            std::shared_ptr<sdProtoDataSet> descriptorSetPtr = getProtoDataSetPtr(targetExtension, identifier);
            return &descriptorSetPtr->getValue<D>();
        }catch(std::exception){return nullptr;}
    }
    return &(event->getValue());
}


