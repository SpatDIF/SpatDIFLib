//
//  sdProtoEvent.h
//  libSpatDIFTest
//
//  Created by Chikashi Miyama on 14/10/2016.
//  Copyright © 2016 ICST. All rights reserved.
//

#pragma once
/**
 * @brief  An abstract class of event inherited from sdProtoMeta. This class maintains additionaly time value.
 * @detail This class can only be used by sdEntity. All member variables are declared as const. Thus, all parameter must be set by initializer list. Modification of events are not possible in SpatDIF Library. Discard the instance, create new one, and move the unchanged data to new one, if modification of member data is required.
 */
class sdEntity;
class sdProtoEvent : public sdProtoMeta{
    
public:
    /*!
     return event time
     @returns the time of event in second
     */
    const double &getTime() const {return time;}
    
    /*!
     returns event time as std::string
     @returns the time of event as a std::string
     */
    std::string getTimeAsString() const {return std::to_string(time);};
    virtual std::string getValueAsString() const = 0;
    
protected:
    /*! protected constructor. only friend of this class can invoke */
    sdProtoEvent(const double time, const EDescriptor descriptor, const sdEntity * parent):
    time(time),
    sdProtoMeta(descriptor, parent)
    {}
    
    const double time; /*< the absolute time of event. unmutable */
};
