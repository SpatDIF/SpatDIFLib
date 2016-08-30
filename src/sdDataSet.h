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
 reusable set of multiple data in an extension, which can be stored in std::unordered_map and referred by name.
 */

class sdProtoDataSet{
public:
    template<EDescriptor D>
    typename sdDescriptor<D>::type &getValue(){
        std::shared_ptr<sdProtoHolder> holder = dataSetHolders.at(D);
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
    
    std::unordered_map<EDescriptor, std::shared_ptr<sdProtoHolder>> dataSetHolders;
};


#pragma specifications of dataset

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
        
        dataSetHolders.emplace(SD_MEDIA_TYPE, std::shared_ptr<sdProtoHolder>(new sdHolder<sdDescriptor<SD_MEDIA_TYPE>::type>(type)));
        dataSetHolders.emplace(SD_MEDIA_LOCATION, std::shared_ptr<sdProtoHolder>(new sdHolder<sdDescriptor<SD_MEDIA_LOCATION>::type>(location)));
        dataSetHolders.emplace(SD_MEDIA_CHANNEL, std::shared_ptr<sdProtoHolder>(new sdHolder<sdDescriptor<SD_MEDIA_CHANNEL>::type>(channel)));
        dataSetHolders.emplace(SD_MEDIA_TIME_OFFSET, std::shared_ptr<sdProtoHolder>(new sdHolder<sdDescriptor<SD_MEDIA_TIME_OFFSET>::type>(time_offset)));
        dataSetHolders.emplace(SD_MEDIA_GAIN, std::shared_ptr<sdProtoHolder>(new sdHolder<sdDescriptor<SD_MEDIA_GAIN>::type>(gain)));
    }
};

template <>
class sdDataSet<EExtension::SD_POINTSET>: public sdProtoDataSet{
public:
    sdDataSet<EExtension::SD_POINTSET>(
                                       sdDescriptor<SD_POINTSET_CLOSED>::type present = true,
                                       sdDescriptor<SD_POINTSET_SIZE>::type size =  1,
                                       sdDescriptor<SD_POINTSET_POINT>::type position = std::array<double, 3>{0,0,0}){
        dataSetHolders.emplace(SD_POINTSET_CLOSED, std::shared_ptr<sdProtoHolder>(new sdHolder<sdDescriptor<SD_POINTSET_CLOSED>::type>(present)));
        dataSetHolders.emplace(SD_POINTSET_SIZE, std::shared_ptr<sdProtoHolder>(new sdHolder<sdDescriptor<SD_POINTSET_SIZE>::type>(size)));
        dataSetHolders.emplace(SD_POINTSET_POINT, std::shared_ptr<sdProtoHolder>(new sdHolder<sdDescriptor<SD_POINTSET_POINT>::type>(position)));
    }
};

template <>
class sdDataSet<EExtension::SD_GROUP>: public sdProtoDataSet{
public:
    sdDataSet<EExtension::SD_GROUP>(
                                    sdDescriptor<SD_GROUP_PRESENT>::type present = true,
                                    sdDescriptor<SD_GROUP_POSITION>::type position = std::array<double, 3>{0,0,0},
                                    sdDescriptor<SD_GROUP_ORIENTATION>::type orientation = std::array<double, 3>{0,0,0}){
        dataSetHolders.emplace(SD_GROUP_PRESENT, std::shared_ptr<sdProtoHolder>(new sdHolder<sdDescriptor<SD_GROUP_PRESENT>::type>(present)));
        dataSetHolders.emplace(SD_GROUP_POSITION, std::shared_ptr<sdProtoHolder>(new sdHolder<sdDescriptor<SD_GROUP_POSITION>::type>(position)));
        dataSetHolders.emplace(SD_GROUP_ORIENTATION, std::shared_ptr<sdProtoHolder>(new sdHolder<sdDescriptor<SD_GROUP_ORIENTATION>::type>(orientation)));
    }
};


template <>
class sdDataSet<EExtension::SD_SHAPE>: public sdProtoDataSet{
public:
    sdDataSet<EExtension::SD_SHAPE>(
                                    sdDescriptor<SD_SHAPE_DIRECTION>::type direction = true,
                                    sdDescriptor<SD_SHAPE_CLOSED>::type closed =  false,
                                    sdDescriptor<SD_SHAPE_TYPE>::type type = sdDescriptor<SD_SHAPE_TYPE>::SD_POINT){
        dataSetHolders.emplace(SD_SHAPE_DIRECTION, std::shared_ptr<sdProtoHolder>(new sdHolder<sdDescriptor<SD_SHAPE_DIRECTION>::type>(direction)));
        dataSetHolders.emplace(SD_SHAPE_CLOSED, std::shared_ptr<sdProtoHolder>(new sdHolder<sdDescriptor<SD_SHAPE_CLOSED>::type>(closed)));
        dataSetHolders.emplace(SD_SHAPE_TYPE, std::shared_ptr<sdProtoHolder>(new sdHolder<sdDescriptor<SD_SHAPE_TYPE>::type>(type)));
    }
};