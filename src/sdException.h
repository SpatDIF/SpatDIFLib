//
//  sdException.h
//  libSpatDIFTest
//
//  Created by Chikashi Miyama on 12/12/15.
//  Copyright © 2015 ICST. All rights reserved.
//

#ifndef sdException_h
#define sdException_h

#include <exception>


class InvalidTimeException : public std::range_error{
public: 
    InvalidTimeException(const double time )
    : std::range_error(std::string("Invalid Time Exception: ") + std::to_string(time) ) {}
};

#endif /* sdException_h */
