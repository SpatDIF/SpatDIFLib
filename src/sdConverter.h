//
//  sdConverter.h
//


#ifndef ____sdConverter__
#define ____sdConverter__

#include "sdConst.h"
#include "sdEntity.h"
/*!
 an utility class for conveting strings to enums and enums to strings
 */
class sdConverter{
public:
    static string convert(EDescriptor descriptor);
    static string convert(EKind kind);
    static string convert(EType type);
    static string convert(EExtension extension);
    
    /*! this function convert the value of event based on its descriptor*/
    static string convert(sdEvent* event);
    
    static int convert(std::string str);
};


#endif