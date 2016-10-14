//
//  sdKindHandler.h
//  libSpatDIFTest
//
//  Created by Chikashi Miyama on 14/10/2016.
//  Copyright © 2016 ICST. All rights reserved.
//

#pragma once
class sdKindHandler{
public:
    sdKindHandler(EKind kind):kind(kind){}
    const EKind &getKind() const{return kind;};
    std::string getKindAsString() const{ return kind == EKind::SD_SOURCE ? "source": "sink";};
    
protected:
    EKind kind;
};
