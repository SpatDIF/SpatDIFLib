//
//  sdHolder.h
//  libSpatDIFTest
//
//  Created by Chikashi Miyama on 14/10/2016.
//  Copyright © 2016 ICST. All rights reserved.
//

#pragma once


struct sdProtoHolder{};

template<typename T>
struct sdHolder : public sdProtoHolder {
    sdHolder(const T& item) : item(item) {}
    T item;
};

