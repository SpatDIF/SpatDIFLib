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
    InvalidTimeException(const double &time )
    : std::range_error(std::string("Invalid Time Exception: ") + std::to_string(time) ) {}
};


class InvalidExtensionException : public std::invalid_argument{
public:
    InvalidExtensionException(std::string extensionName)
    : std::invalid_argument(std::string("Invalid Extension: ") + extensionName){}
};

class InvalidDescriptorException : public std::invalid_argument{
public:
    InvalidDescriptorException(std::string descriptorName)
    : std::invalid_argument(std::string("Invalid Descriptor ") + descriptorName){}
};

class CanNotConvertStringException : public std::invalid_argument{
public:
    CanNotConvertStringException(std::string message)
    : std::invalid_argument(std::string("Invalid String. Cannot convert: " + message)){}
};


class InvalidValueDomainException : public std::domain_error{
public:
    InvalidValueDomainException(std::string message)
    : std::domain_error(std::string("Invalid Value Domain: " + message)){}
};


class FileErrorException : public std::ios_base::failure{
public:
    FileErrorException()
    : std::ios_base::failure(std::string("File parse failed")){}
};

#endif /* sdException_h */
