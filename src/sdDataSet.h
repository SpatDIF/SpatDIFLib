//
//  sdDataSet.h
//  libSpatDIFTest
//
//  Created by Chikashi Miyama on 30/08/16.
//  Copyright © 2016 ICST. All rights reserved.
//

#pragma once

#include "sdDescriptor.h"
/*!
 reusable set of multiple descriptors, which can be stored in std::unordered_map and refered by name.
 See sdDataSetHandler.h sdScene inherits the functionality of sdDataSetHandler and is able to handle sdDataSets
 */

class sdProtoDataSet{
public:
    template<EDescriptor D>
    typename sdDescriptor<D>::type &getValue(){
        std::shared_ptr<sdProtoHolder> holder = descriptorSetHolders.at(D);
        sdHolder<typename sdDescriptor<D>::type> *holderPtr =static_cast<sdHolder<typename sdDescriptor<D>::type> *>(holder.get());
        return holderPtr->item;
    };
    
protected:
    struct sdProtoHolder{
    };
    
    template<typename T>
    struct sdHolder : public sdProtoHolder {
        sdHolder(const T& item) : item(item) {}
        
        T item;
    };
    std::unordered_map<EDescriptor, std::shared_ptr<sdProtoHolder>> descriptorSetHolders;
};


#pragma specifications

template <EExtension E>
class sdDataSet{};

template <>
class sdDataSet<EExtension::SD_MEDIA>: public sdProtoDataSet{
public:
    sdDataSet<EExtension::SD_MEDIA>(
                                          sdDescriptor<SD_MEDIA_TYPE>::type type = sdDescriptor<SD_MEDIA_TYPE>::SD_FILE,
                                          sdDescriptor<SD_MEDIA_LOCATION>::type location = "",
                                          sdDescriptor<SD_MEDIA_CHANNEL>::type channel = 1,
                                          sdDescriptor<SD_MEDIA_TIME_OFFSET>::type time_offset = 0,
                                          sdDescriptor<SD_MEDIA_GAIN>::type gain = 1.0){
        
        descriptorSetHolders.emplace(SD_MEDIA_TYPE, std::shared_ptr<sdProtoHolder>(new sdHolder<sdDescriptor<SD_MEDIA_TYPE>::type>(type)));
        descriptorSetHolders.emplace(SD_MEDIA_LOCATION, std::shared_ptr<sdProtoHolder>(new sdHolder<sdDescriptor<SD_MEDIA_LOCATION>::type>(location)));
        descriptorSetHolders.emplace(SD_MEDIA_CHANNEL, std::shared_ptr<sdProtoHolder>(new sdHolder<sdDescriptor<SD_MEDIA_CHANNEL>::type>(channel)));
        descriptorSetHolders.emplace(SD_MEDIA_TIME_OFFSET, std::shared_ptr<sdProtoHolder>(new sdHolder<sdDescriptor<SD_MEDIA_TIME_OFFSET>::type>(time_offset)));
        descriptorSetHolders.emplace(SD_MEDIA_GAIN, std::shared_ptr<sdProtoHolder>(new sdHolder<sdDescriptor<SD_MEDIA_GAIN>::type>(gain)));
    }
    
};


template <>
class sdDataSet<EExtension::SD_POINTSET>: public sdProtoDataSet {
public:
    sdDataSet<EExtension::SD_POINTSET>(
                                             sdDescriptor<EDescriptor::SD_POINTSET_CLOSED>::type closed,
                                             sdDescriptor<EDescriptor::SD_POINTSET_SIZE>::type size,
                                             sdDescriptor<EDescriptor::SD_POINTSET_POINT>::type point,
                                             sdDescriptor<EDescriptor::SD_POINTSET_HANDLE>::type handle):
    closed(closed),
    size(size),
    point(point),
    handle(handle){}
    
protected:
    sdDescriptor<EDescriptor::SD_POINTSET_CLOSED>::type closed;
    sdDescriptor<EDescriptor::SD_POINTSET_SIZE>::type size;
    sdDescriptor<EDescriptor::SD_POINTSET_POINT>::type point;
    sdDescriptor<EDescriptor::SD_POINTSET_HANDLE>::type handle;
};


