/** @file
 *
 * @ingroup spatdiflib
 *
 * @brief
 *
 * @details
 *
 * @authors Chikashi Miyama, Trond Lossius
 *
 * @copyright Copyright (C) 2013 - 2014 Zurich University of the Arts, Institute for Computer Music and Sound Technology @n
 * This code is licensed under the terms of the "New BSD License" @n
 * http://creativecommons.org/licenses/BSD/
 */

#ifndef ____sdConst__
#define ____sdConst__

#include <string>
#include <iostream>
#include <sstream>

/*!
    enum for descriptor. internally all descriptors are handled with this Enum
 */
typedef enum {
    SD_TYPE,
    SD_PRESENT,
    SD_POSITION,
    SD_ORIENTATION,
    
    // descriptor for media extension
    SD_MEDIA_ID,
    SD_MEDIA_TYPE,
    SD_MEDIA_LOCATION,
    SD_MEDIA_CHANNEL,
    SD_MEDIA_TIME_OFFSET,
    SD_MEDIA_GAIN,
    
    // descriptor for source width extension
    SD_SOURCE_WIDTH_WIDTH,
    
    // descriptor for point set extension
    SD_POINT_SET_NAME,
    
    
    
    SD_ERROR,
    SD_ALL
} EDescriptor;

/*!
 enum for "type" descriptor.
 Currently only "point" is declared in the specification.
 */
typedef enum{
    SD_POINT
} EType;

/*!
 enum for ordering
 */
typedef enum {
    SD_TIME,
    SD_TRACK
} EOrdering;

/*!
 enum for kind. All sdEntityCore must define their kind when instantiated.
 */
typedef enum {
    SD_SOURCE,
    SD_SINK
    
} EKind;

/*!
 enum for extension. all sdEntityExtension must have one of these enum as a static variable in order to identify themselves
 */
typedef enum {
    SD_CORE,
    SD_MEDIA,
    SD_INTERPOLATION,
    SD_SOURCE_WIDTH,
    SD_DIRECT_TO_ONE,
    SD_EXTENSION_ERROR
} EExtension;

/*!
 enum for interpolation types
 */
typedef enum {
    SD_NO_INTERPOLATION,
    SD_LINEAR_INTERPOLATION,
    SD_CUBIC_INTERPOLATION
} EInterpolation;

/*!
 The Descriptor traits. implemented with template explicit specialization technique
 */

template <EDescriptor D>
struct sdDescriptor{};

template <>
struct sdDescriptor<EDescriptor::SD_TYPE>{
    typedef bool type;
    const static bool interpolable = true;
};

template <>
struct sdDescriptor<EDescriptor::SD_PRESENT>{
    typedef bool type;
    const static bool interpolable = false;
};

template <>
struct sdDescriptor<EDescriptor::SD_POSITION>{
    typedef std::array<double, 3> type;
    const static bool interpolable = true;
};

template <>
struct sdDescriptor<EDescriptor::SD_ORIENTATION>{
    typedef std::array<double, 3> type;
    const static bool interpolable = true;
};



/*!
 utility function that convert ints to a string
 */
inline std::string intsToString(const int *it, const int num){
    std::ostringstream os;
    for(int i=0;i<num;i++){
        os << it[i];
        if(i != (num-1))
            os << ' ';
    }
    return os.str();
}

/*!
 utility function that convert a int to a string
 */
inline std::string intToString(const int it){
    return intsToString(&it, 1);
}

/*!
 utility function that convert ints to a string
 */
inline std::string floatsToString(const float *ft, const int num){
    std::ostringstream os;
    for(int i=0;i<num;i++){
        os << ft[i];
        if(i != (num-1))
            os << ' ';
    }
    return os.str();
}

/*!
 utility function that convert a int to a string
 */
inline std::string floatToString(const float ft){
    return floatsToString(&ft, 1);
}

/*!
 utility function that convert floats to double. buffer must be allocated properly beforehand
 */
inline double *floatsToDoubles(const float *floats, double *doubles, const int num){
    for (int i = 0; i < num; i++){
        doubles[i] = static_cast<double>(floats[i]);
    }
    return doubles;
}

/*!
 utility function that convert floats to double. buffer must be allocated properly beforehand
 */
inline float *doublesToFloats(const double *doubles, float *floats, const int num){
    for (int i = 0; i < num; i++){
        floats[i] = static_cast<float>(doubles[i]);
    }
    return floats;
}

/*! 
 utility function that convert doubles to a string
 */
inline std::string doublesToString(const double *db, const int num){
    std::ostringstream os;
    for(int i=0;i<num;i++){
        os << db[i];
        if(i != (num-1))
            os << ' ';
    }
    return os.str();
}

/*!
 utility function that convert a double to a string
 */
inline std::string doubleToString(const double db){
    return doublesToString(&db, 1);
}

/*!
 utility function that convert a bool to a string
 */
inline std::string boolToString(const bool bl){
    return bl ? std::string("true") : std::string("false");
}

/*!
 utility function that convert a string to a bool
 */
inline bool stringToBool(const std::string str){
    return str == "true" ? true : false;
}



/*!
 utility function that convert a string to doubles
 */
inline double *stringToDoubles(std::string str, double *db, int num){
    std::istringstream is(str);
    for(int i = 0; i < num; i++) {
        is >> db[i];
    }
    return db;
}

/*!
 utility function that convert a string to a double
 */
inline double stringToDouble(std::string str){
    double db;
    stringToDoubles(str, &db, 1);
    return db;
}

/*!
 utility function that converts a string to a int
 */
inline int *stringToInts(std::string str, int *it, int num){
    std::istringstream is(str);
    for(int i = 0; i < num; i++) {
        is >> it[i];
    }
    return it;
}

/*!
 utility function that convert int to a string
 */
inline int stringToInt(std::string str){
    int it;
    stringToInts(str, &it, 1);
    return it;
}

/**
 * @brief converts enum EExtension to string
 * @param extension [description]
 * @return [description]
 */
inline const std::string extensionToString(EExtension extension){
    std::string extensionStr;
    if(extension == SD_MEDIA){
        extensionStr = "media";
    }else if(extension == SD_INTERPOLATION){
        extensionStr = "interpolation";
    }else if(extension == SD_DIRECT_TO_ONE){
        extensionStr = "directo-to-one";
    }else if(extension == SD_SOURCE_WIDTH){
        extensionStr = "source-width";
    }else if(extension == SD_CORE){
        extensionStr = "core";
    }else{
        extensionStr = "error";
    }
    return extensionStr;
}

/**
 * @brief converts string to enum EExtension
 * @param extensionStr the name of extension in string 
 * @return enum EExtension that represents the type of extension
 */
inline  EExtension stringToExtension(std::string extensionStr){
    EExtension ext;
    if(extensionStr == "media"){
        ext = SD_MEDIA;
    }else if(extensionStr == "interpolation"){
        ext = SD_INTERPOLATION;
    }else if(extensionStr == "direct-to-one"){
        ext = SD_DIRECT_TO_ONE;
    }else if(extensionStr == "source-width"){
        ext = SD_SOURCE_WIDTH;
    }else{
        ext = SD_EXTENSION_ERROR;
    }
    return ext;
}

/**
 * @brief returns type of extension associated to the given descriptor. if no extension found, returns SD_EXTENSION_ERROR.
 * @param descriptor an extension which includes this descriptor will be returned
 * @return extension associated to the given descriptor
 */
inline EExtension getRelevantExtension(EDescriptor descriptor){
    if(EDescriptor::SD_TYPE <= descriptor && descriptor <= EDescriptor::SD_ORIENTATION){
        return SD_CORE;
    }else if(EDescriptor::SD_MEDIA_ID <= descriptor && descriptor <= EDescriptor::SD_MEDIA_GAIN){
        return SD_MEDIA;
    }else{
        return SD_EXTENSION_ERROR;
    }
}

#endif
