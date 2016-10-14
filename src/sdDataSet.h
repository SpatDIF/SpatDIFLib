//
//  sdDataSet.h
//  libSpatDIFTest
//
//  Created by Chikashi Miyama on 30/08/16.
//  Copyright © 2016 ICST. All rights reserved.
//

#pragma once
#include <unordered_set>
#include <unordered_map>
#include "sdHolder.h"
#include "sdSpec.h"
#include "sdDescriptorSpec.h"
/*!
 reusable set of multiple data in an extension, which can be stored in std::unordered_map and referred by name.
 */

class sdProtoDataSet{
public:
    
    template<EDescriptor D>
    typename sdDescriptorSpec<D>::type &getValue() const{
        std::shared_ptr<sdProtoHolder> holder = dataSetHolders.at(D);
        sdHolder<typename sdDescriptorSpec<D>::type> *holderPtr = static_cast<sdHolder<typename sdDescriptorSpec<D>::type> *>(holder.get());
        return holderPtr->item;
    }
    
    template<EDescriptor D>
    std::string getValueAsString() const{
        return sdDescriptorSpec<D>::toString(getValue<D>());
    }
    
    std::string getValueAsString(EDescriptor descriptor) const{
        std::shared_ptr<sdProtoHolder> protoHolder = dataSetHolders.at(descriptor);
        auto getDataAsStringFunc = sdSpec::getDataAsStringFunc(descriptor);
        return getDataAsStringFunc(protoHolder);
        
    }
    
    template<EDescriptor D>
    void setValue(typename sdDescriptorSpec<D>::type value) {
        std::shared_ptr<sdProtoHolder> holder = dataSetHolders.at(D);
        sdHolder<typename sdDescriptorSpec<D>::type> *holderPtr = static_cast<sdHolder<typename sdDescriptorSpec<D>::type> *>(holder.get());
        holderPtr->item = value;
    }
        
    std::unordered_set<EDescriptor> getKeys() const{
        std::unordered_set<EDescriptor> set;
        for(auto &dataSetHolder:dataSetHolders){ set.insert(dataSetHolder.first);}
        return set;
    }
    
protected:
    
    // type-erased dataset map
    std::unordered_map<EDescriptor, std::shared_ptr<sdProtoHolder>> dataSetHolders;
};


#pragma specifications of dataset


template <EExtension E>
class sdDataSet : public sdProtoDataSet{} ;

template <>
class sdDataSet<EExtension::SD_INFO>: public sdProtoDataSet{
public:
    sdDataSet<EExtension::SD_INFO>(
                                   sdDescriptorSpec<SD_INFO_AUTHOR>::type author = "unknown",
                                   sdDescriptorSpec<SD_INFO_HOST>::type host = "",
                                   sdDescriptorSpec<SD_INFO_DATE>::type date = "",
                                   sdDescriptorSpec<SD_INFO_SESSION>::type session = "",
                                   sdDescriptorSpec<SD_INFO_LOCATION>::type location = "",
                                   sdDescriptorSpec<SD_INFO_ANNOTATION>::type annotation = "",
                                   sdDescriptorSpec<SD_INFO_TITLE>::type title = "unknown",
                                   sdDescriptorSpec<SD_INFO_DURATION>::type duration = 0.0){
        
        dataSetHolders.emplace(SD_INFO_AUTHOR, std::shared_ptr<sdProtoHolder>(new sdHolder<sdDescriptorSpec<SD_INFO_AUTHOR>::type>(author)));
        dataSetHolders.emplace(SD_INFO_HOST, std::shared_ptr<sdProtoHolder>(new sdHolder<sdDescriptorSpec<SD_INFO_HOST>::type>(host)));
        dataSetHolders.emplace(SD_INFO_DATE, std::shared_ptr<sdProtoHolder>(new sdHolder<sdDescriptorSpec<SD_INFO_DATE>::type>(date)));
        dataSetHolders.emplace(SD_INFO_SESSION, std::shared_ptr<sdProtoHolder>(new sdHolder<sdDescriptorSpec<SD_INFO_SESSION>::type>(session)));
        dataSetHolders.emplace(SD_INFO_LOCATION, std::shared_ptr<sdProtoHolder>(new sdHolder<sdDescriptorSpec<SD_INFO_LOCATION>::type>(location)));
        dataSetHolders.emplace(SD_INFO_ANNOTATION, std::shared_ptr<sdProtoHolder>(new sdHolder<sdDescriptorSpec<SD_INFO_ANNOTATION>::type>(annotation)));
        dataSetHolders.emplace(SD_INFO_TITLE, std::shared_ptr<sdProtoHolder>(new sdHolder<sdDescriptorSpec<SD_INFO_TITLE>::type>(title)));
        dataSetHolders.emplace(SD_INFO_DURATION, std::shared_ptr<sdProtoHolder>(new sdHolder<sdDescriptorSpec<SD_INFO_DURATION>::type>(duration)));
    }
    

};

using sdInfo = sdDataSet<EExtension::SD_INFO>;

template <>
class sdDataSet<EExtension::SD_MEDIA>: public sdProtoDataSet{
public:
    sdDataSet<EExtension::SD_MEDIA>(sdDescriptorSpec<SD_MEDIA_ID>::type identifier,
                                    sdDescriptorSpec<SD_MEDIA_TYPE>::type type = sdDescriptorSpec<SD_MEDIA_TYPE>::SD_FILE,
                                    sdDescriptorSpec<SD_MEDIA_LOCATION>::type location = "",
                                    sdDescriptorSpec<SD_MEDIA_CHANNEL>::type channel = 1,
                                    sdDescriptorSpec<SD_MEDIA_TIME_OFFSET>::type time_offset = 0,
                                    sdDescriptorSpec<SD_MEDIA_GAIN>::type gain = 1.0){
        
        dataSetHolders.emplace(SD_MEDIA_ID, std::shared_ptr<sdProtoHolder>(new sdHolder<sdDescriptorSpec<SD_MEDIA_ID>::type>(identifier)));
        dataSetHolders.emplace(SD_MEDIA_TYPE, std::shared_ptr<sdProtoHolder>(new sdHolder<sdDescriptorSpec<SD_MEDIA_TYPE>::type>(type)));
        dataSetHolders.emplace(SD_MEDIA_LOCATION, std::shared_ptr<sdProtoHolder>(new sdHolder<sdDescriptorSpec<SD_MEDIA_LOCATION>::type>(location)));
        dataSetHolders.emplace(SD_MEDIA_CHANNEL, std::shared_ptr<sdProtoHolder>(new sdHolder<sdDescriptorSpec<SD_MEDIA_CHANNEL>::type>(channel)));
        dataSetHolders.emplace(SD_MEDIA_TIME_OFFSET, std::shared_ptr<sdProtoHolder>(new sdHolder<sdDescriptorSpec<SD_MEDIA_TIME_OFFSET>::type>(time_offset)));
        dataSetHolders.emplace(SD_MEDIA_GAIN, std::shared_ptr<sdProtoHolder>(new sdHolder<sdDescriptorSpec<SD_MEDIA_GAIN>::type>(gain)));
    }
};

template <>
class sdDataSet<EExtension::SD_GROUP>: public sdProtoDataSet{
public:
    sdDataSet<EExtension::SD_MEDIA>(sdDescriptorSpec<SD_GROUP_ID>::type identifier,
                                    sdDescriptorSpec<SD_GROUP_PRESENT>::type present = true,
                                    sdDescriptorSpec<SD_GROUP_POSITION>::type position = {0,0,0},
                                    sdDescriptorSpec<SD_GROUP_ORIENTATION>::type orientation = {0,0,0}){
        
        dataSetHolders.emplace(SD_GROUP_ID, std::shared_ptr<sdProtoHolder>(new sdHolder<sdDescriptorSpec<SD_GROUP_ID>::type>(identifier)));
        dataSetHolders.emplace(SD_GROUP_PRESENT, std::shared_ptr<sdProtoHolder>(new sdHolder<sdDescriptorSpec<SD_GROUP_PRESENT>::type>(present)));
        dataSetHolders.emplace(SD_GROUP_POSITION, std::shared_ptr<sdProtoHolder>(new sdHolder<sdDescriptorSpec<SD_GROUP_POSITION>::type>(position)));
        dataSetHolders.emplace(SD_GROUP_ORIENTATION, std::shared_ptr<sdProtoHolder>(new sdHolder<sdDescriptorSpec<SD_GROUP_ORIENTATION>::type>(orientation)));
    }
};

template <>
class sdDataSet<EExtension::SD_POINTSET>: public sdProtoDataSet{
public:
    sdDataSet<EExtension::SD_POINTSET>(sdDescriptorSpec<SD_POINTSET_ID>::type identifier,
                                       sdDescriptorSpec<SD_POINTSET_CLOSED>::type closed = true,
                                       sdDescriptorSpec<SD_POINTSET_POINT_OR_HANDLE>::type pointOrHandle = std::vector<std::pair<EDescriptor, std::array<double, 3>>>()){
        dataSetHolders.emplace(SD_POINTSET_ID, std::shared_ptr<sdProtoHolder>(new sdHolder<sdDescriptorSpec<SD_POINTSET_ID>::type>(identifier)));
        dataSetHolders.emplace(SD_POINTSET_CLOSED, std::shared_ptr<sdProtoHolder>(new sdHolder<sdDescriptorSpec<SD_POINTSET_CLOSED>::type>(closed)));
        dataSetHolders.emplace(SD_POINTSET_POINT_OR_HANDLE, std::shared_ptr<sdProtoHolder>(new sdHolder<sdDescriptorSpec<SD_POINTSET_POINT_OR_HANDLE>::type>(pointOrHandle)));
    }
    
    void addPoint(const std::array<double, 3> &point){
        getValue<SD_POINTSET_POINT_OR_HANDLE>().push_back(std::make_pair(SD_POINTSET_POINT, point));
    }
    void addHandle(const std::array<double, 3> &handle){
        getValue<SD_POINTSET_POINT_OR_HANDLE>().push_back(std::make_pair(SD_POINTSET_HANDLE, handle));
    }
};


template <>
class sdDataSet<EExtension::SD_SHAPE>: public sdProtoDataSet{
public:
    sdDataSet<EExtension::SD_SHAPE>(
                                    sdDescriptorSpec<SD_SHAPE_ID>::type identifier,
                                    sdDescriptorSpec<SD_SHAPE_DIRECTION>::type direction = true,
                                    sdDescriptorSpec<SD_SHAPE_CLOSED>::type closed =  false,
                                    sdDescriptorSpec<SD_SHAPE_TYPE>::type type = sdDescriptorSpec<SD_SHAPE_TYPE>::SD_POINT){
        dataSetHolders.emplace(SD_SHAPE_ID, std::shared_ptr<sdProtoHolder>(new sdHolder<sdDescriptorSpec<SD_SHAPE_ID>::type>(identifier)));
        dataSetHolders.emplace(SD_SHAPE_DIRECTION, std::shared_ptr<sdProtoHolder>(new sdHolder<sdDescriptorSpec<SD_SHAPE_DIRECTION>::type>(direction)));
        dataSetHolders.emplace(SD_SHAPE_CLOSED, std::shared_ptr<sdProtoHolder>(new sdHolder<sdDescriptorSpec<SD_SHAPE_CLOSED>::type>(closed)));
        dataSetHolders.emplace(SD_SHAPE_TYPE, std::shared_ptr<sdProtoHolder>(new sdHolder<sdDescriptorSpec<SD_SHAPE_TYPE>::type>(type)));
    }
};
