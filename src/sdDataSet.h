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
#include "sdSpec.h"
#include "sdDescriptor.h"
/*!
 reusable set of multiple data in an extension, which can be stored in std::unordered_map and referred by name.
 */

struct sdProtoHolder{};

template<typename T>
struct sdHolder : public sdProtoHolder {
    sdHolder(const T& item) : item(item) {}
    T item;
};


class sdProtoDataSet{
public:
    
    template<EDescriptor D>
    typename sdDescriptor<D>::type &getValue() const{
        std::shared_ptr<sdProtoHolder> holder = dataSetHolders.at(D);
        sdHolder<typename sdDescriptor<D>::type> *holderPtr = static_cast<sdHolder<typename sdDescriptor<D>::type> *>(holder.get());
        return holderPtr->item;
    }
    
    template<EDescriptor D>
    std::string getValueAsString() const{
        return sdDescriptor<D>::toString(getValue<D>());
    }
    
    std::string getValueAsString(EDescriptor descriptor) const{
        std::shared_ptr<sdProtoHolder> protoHolder = dataSetHolders.at(descriptor);
        auto getDataAsStringFunc = sdSpec::getDataAsStringFunc(descriptor);
        return getDataAsStringFunc(protoHolder);
        
    }
    
    template<EDescriptor D>
    void setValue(typename sdDescriptor<D>::type value) {
        std::shared_ptr<sdProtoHolder> holder = dataSetHolders.at(D);
        sdHolder<typename sdDescriptor<D>::type> *holderPtr = static_cast<sdHolder<typename sdDescriptor<D>::type> *>(holder.get());
        holderPtr->item = value;
    }
        
    std::unordered_set<EDescriptor> getKeys() const{
        std::unordered_set<EDescriptor> set;
        for(auto &dataSetHolder:dataSetHolders){ set.insert(dataSetHolder.first);}
        return set;
    }
    
protected:
    
    /// this holder is used for type
    
    std::unordered_map<EDescriptor, std::shared_ptr<sdProtoHolder>> dataSetHolders;
};


#pragma specifications of dataset


template <EExtension E>
class sdDataSet : public sdProtoDataSet{} ;

template <>
class sdDataSet<EExtension::SD_INFO>: public sdProtoDataSet{
public:
    sdDataSet<EExtension::SD_INFO>(
                                   sdDescriptor<SD_INFO_AUTHOR>::type author = "unknown",
                                   sdDescriptor<SD_INFO_HOST>::type host = "",
                                   sdDescriptor<SD_INFO_DATE>::type date = "",
                                   sdDescriptor<SD_INFO_SESSION>::type session = "",
                                   sdDescriptor<SD_INFO_LOCATION>::type location = "",
                                   sdDescriptor<SD_INFO_ANNOTATION>::type annotation = "",
                                   sdDescriptor<SD_INFO_TITLE>::type title = "unknown",
                                   sdDescriptor<SD_INFO_DURATION>::type duration = 0.0){
        
        dataSetHolders.emplace(SD_INFO_AUTHOR, std::shared_ptr<sdProtoHolder>(new sdHolder<sdDescriptor<SD_INFO_AUTHOR>::type>(author)));
        dataSetHolders.emplace(SD_INFO_HOST, std::shared_ptr<sdProtoHolder>(new sdHolder<sdDescriptor<SD_INFO_HOST>::type>(host)));
        dataSetHolders.emplace(SD_INFO_DATE, std::shared_ptr<sdProtoHolder>(new sdHolder<sdDescriptor<SD_INFO_DATE>::type>(date)));
        dataSetHolders.emplace(SD_INFO_SESSION, std::shared_ptr<sdProtoHolder>(new sdHolder<sdDescriptor<SD_INFO_SESSION>::type>(session)));
        dataSetHolders.emplace(SD_INFO_LOCATION, std::shared_ptr<sdProtoHolder>(new sdHolder<sdDescriptor<SD_INFO_LOCATION>::type>(location)));
        dataSetHolders.emplace(SD_INFO_ANNOTATION, std::shared_ptr<sdProtoHolder>(new sdHolder<sdDescriptor<SD_INFO_ANNOTATION>::type>(annotation)));
        dataSetHolders.emplace(SD_INFO_TITLE, std::shared_ptr<sdProtoHolder>(new sdHolder<sdDescriptor<SD_INFO_TITLE>::type>(title)));
        dataSetHolders.emplace(SD_INFO_DURATION, std::shared_ptr<sdProtoHolder>(new sdHolder<sdDescriptor<SD_INFO_DURATION>::type>(duration)));
    }
    

};

using sdInfo = sdDataSet<EExtension::SD_INFO>;

template <>
class sdDataSet<EExtension::SD_MEDIA>: public sdProtoDataSet{
public:
    sdDataSet<EExtension::SD_MEDIA>(sdDescriptor<SD_MEDIA_ID>::type identifier,
                                    sdDescriptor<SD_MEDIA_TYPE>::type type = sdDescriptor<SD_MEDIA_TYPE>::SD_FILE,
                                    sdDescriptor<SD_MEDIA_LOCATION>::type location = "",
                                    sdDescriptor<SD_MEDIA_CHANNEL>::type channel = 1,
                                    sdDescriptor<SD_MEDIA_TIME_OFFSET>::type time_offset = 0,
                                    sdDescriptor<SD_MEDIA_GAIN>::type gain = 1.0){
        
        dataSetHolders.emplace(SD_MEDIA_ID, std::shared_ptr<sdProtoHolder>(new sdHolder<sdDescriptor<SD_MEDIA_ID>::type>(identifier)));
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
    sdDataSet<EExtension::SD_POINTSET>(sdDescriptor<SD_POINTSET_ID>::type identifier,
                                       sdDescriptor<SD_POINTSET_CLOSED>::type present = true,
                                       sdDescriptor<SD_POINTSET_SIZE>::type size =  1,
                                       sdDescriptor<SD_POINTSET_POINT>::type position = std::array<double, 3>{0,0,0}){
        dataSetHolders.emplace(SD_POINTSET_ID, std::shared_ptr<sdProtoHolder>(new sdHolder<sdDescriptor<SD_POINTSET_ID>::type>(identifier)));
        dataSetHolders.emplace(SD_POINTSET_CLOSED, std::shared_ptr<sdProtoHolder>(new sdHolder<sdDescriptor<SD_POINTSET_CLOSED>::type>(present)));
        dataSetHolders.emplace(SD_POINTSET_SIZE, std::shared_ptr<sdProtoHolder>(new sdHolder<sdDescriptor<SD_POINTSET_SIZE>::type>(size)));
        dataSetHolders.emplace(SD_POINTSET_POINT, std::shared_ptr<sdProtoHolder>(new sdHolder<sdDescriptor<SD_POINTSET_POINT>::type>(position)));
    }
};


template <>
class sdDataSet<EExtension::SD_SHAPE>: public sdProtoDataSet{
public:
    sdDataSet<EExtension::SD_SHAPE>(
                                    sdDescriptor<SD_SHAPE_ID>::type identifier,
                                    sdDescriptor<SD_SHAPE_DIRECTION>::type direction = true,
                                    sdDescriptor<SD_SHAPE_CLOSED>::type closed =  false,
                                    sdDescriptor<SD_SHAPE_TYPE>::type type = sdDescriptor<SD_SHAPE_TYPE>::SD_POINT){
        dataSetHolders.emplace(SD_SHAPE_ID, std::shared_ptr<sdProtoHolder>(new sdHolder<sdDescriptor<SD_SHAPE_ID>::type>(identifier)));
        dataSetHolders.emplace(SD_SHAPE_DIRECTION, std::shared_ptr<sdProtoHolder>(new sdHolder<sdDescriptor<SD_SHAPE_DIRECTION>::type>(direction)));
        dataSetHolders.emplace(SD_SHAPE_CLOSED, std::shared_ptr<sdProtoHolder>(new sdHolder<sdDescriptor<SD_SHAPE_CLOSED>::type>(closed)));
        dataSetHolders.emplace(SD_SHAPE_TYPE, std::shared_ptr<sdProtoHolder>(new sdHolder<sdDescriptor<SD_SHAPE_TYPE>::type>(type)));
    }
};