//
//  sdOrderingHandler.h
//  libSpatDIFTest
//
//  Created by Chikashi Miyama on 23/09/16.
//  Copyright © 2016 ICST. All rights reserved.
//

#pragma once

class sdOrderingHandler{
public:
    
    sdOrderingHandler(EOrdering ordering = EOrdering::SD_TIME):ordering(ordering){}
    
    const EOrdering &getOrdering(void) const{
        return ordering;
    }
    std::string getOrderingAsString(void) const{
        return ordering == EOrdering::SD_TIME ? std::string("time") : std::string("track"); // invoke move semantics bacause of rvalue
    }
    
    void setOrdering(const EOrdering &ordering ){
        sdOrderingHandler::ordering = ordering;
    }
    
    void setOrdering(const std::string &ordering){
        if(ordering == "time"){
            sdOrderingHandler::ordering = EOrdering::SD_TIME;
        }else if(ordering == "track"){
            sdOrderingHandler::ordering = EOrdering::SD_TRACK;
        }else{
            throw InvalidDescriptorException(ordering);
        }
    }
protected:
    EOrdering ordering; //!< ordering flag
};