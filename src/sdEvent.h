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

#ifndef __sdEvent__
#define __sdEvent__

#include <string>
#include "sdConst.h"

#pragma mark definitions

/**
 * @brief  A pure virtual class of event. This class maintains data of time, descriptor, and value and responsible for converting incoming and outgoing data.
 */

class sdEvent{
    
protected:
    
    double time;/*!< time of the event */
    EDescriptor descriptor;/*!<descriptor of the event*/
    void *value;/*!<value of the event. the subclasses must allocates this void pointer */
    
public:
    
    /*! constructor */
    sdEvent(){};
    
    sdEvent(const double time, const EDescriptor descriptor, void* const value):
    time(time), descriptor(descriptor), value(value){};
    
    /*! destructor should be overrided by the subclasses and they are responsible for releaseing void *value */
    virtual ~sdEvent() = 0;
    
    /*! @name Setter
     @{ */
    
    /*!
     set all three parameters at once
     @param time time of event
     @param desciptor desciprot of event declared in sdConst.h
     @param value value of event
     */
    void set(const double time, const EDescriptor descriptor,  void* const value);
    
    /*! @} */
    
    /*! @name Time handling
     @{ */
    
    /*! set time of the event
     @param time time of the event */
    void setTime(const double time);
    
    /*! set time of the event
     @param time time of the event as String */
    void setTime(const std::string timeString);
    
    /*!returns time as double*/
    double getTime(void) const;
    
    /*!returns time as string*/
    std::string getTimeAsString(void) const;
    
    /*! @} */
    
    /*! @name Value handling
     @{ */
    
    /*! descriptor and value should be set at once. because this function allocates memory to void* value based on the descriptor*/
    virtual bool setValue(const EDescriptor descriptor, void* const value) = 0;
    
    /*!overloaded function*/
    virtual bool setValue(const std::string descriptor, const std::string value) = 0;
    
    /*!returns descriptor as Enum */
    EDescriptor getDescriptor(void) const;
    
    /*!returns descriptor as string. a routine for conversion should be implemented in derived classes*/
    virtual std::string getDescriptorAsString(void) const = 0;
    
    /*!returns value through void pointer. It should be casted to propoer type manually.*/
    void* getValue(void) const;
    
    /*!returns value as string. overrided in the inherited classes */
    virtual std::string getValueAsString(void) const = 0;
    
    /*! @} */
};

#pragma mark implementations

inline void sdEvent::set(const double time, const EDescriptor descriptor, void* const value){
    if(!value){
        std::cout << "Error: sdEvent::set()\n value not allocated. " << std::endl;
        return;
    }
    setTime(time);
    setValue(descriptor, value);
}

inline void sdEvent::setTime(const double time){
    if(time < 0){
        std::cout << "Error: sdEvent::setTime()\n time value less than 0.0. set to 0.0." << std::endl;
        this->time = 0.0;
    }
    this->time = time;
}

inline void sdEvent::setTime(const std::string timeString){
    sdEvent::time = stringToDouble(timeString);
}

inline double sdEvent::getTime(void) const{
    return time;
}

inline std::string sdEvent::getTimeAsString(void) const{
    return doubleToString(time);
}

inline EDescriptor sdEvent::getDescriptor(void) const{
    return descriptor;
}

inline void* sdEvent::getValue(void) const{
    return value;
}

#pragma mark helper class
/*!
 a helper class for sorting events
 */
class sdEventCompare{
public:
    bool operator()(sdEvent* e1, sdEvent* e2){
        if(e1->getTime() < e2->getTime()) // sort by time
            return true;
        else
            return false;
    }
};

#endif
