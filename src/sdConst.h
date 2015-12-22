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
#include <map>
#include <array>
#include <algorithm>
#include <cmath>
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
enum class EType{
    SD_POINT
};

/*!
 enum for ordering
 */
enum class EOrdering{
    SD_TIME,
    SD_TRACK
};

/*!
 enum for kind. All sdEntityCore must define their kind when instantiated.
 */
enum class EKind {
    SD_SOURCE,
    SD_SINK
};

/*!
 enum for extension. all sdEntityExtension must have one of these enum as a static variable in order to identify themselves
 */
enum class EExtension {
    SD_CORE,
    SD_MEDIA,
    SD_SOURCE_WIDTH,
    SD_DIRECT_TO_ONE,
    SD_EXTENSION_ERROR
};

/*!
 enum for interpolation types
 */
typedef enum {
    SD_NO_INTERPOLATION,
    SD_LINEAR_INTERPOLATION,
    SD_CUBIC_INTERPOLATION
} EInterpolation;


class sdExtension {
protected:
    
    struct sdExtensionSpec{
        sdExtensionSpec(const std::string &name, const std::map<EDescriptor, std::string> &members):
        name(name), members(members){};
        
        std::string name;
        const std::map<EDescriptor, std::string> members;
    };
    
    static const std::map<EExtension, sdExtensionSpec> extensionDict;
public:
    static const std::map<EDescriptor, std::string> &getDescriptorsForExtension(EExtension extension){
        return extensionDict.at(extension).members;
    }
    static EExtension getExtensionOfDescriptor(EDescriptor descriptor){
        for(auto it = extensionDict.begin(); it != extensionDict.end(); it++) {
            auto members = (*it).second.members;
            for (auto rit = members.begin(); rit != members.end(); rit++) {
                if( (*rit).first == descriptor){
                    return (*it).first;
                }
            };
        }
        return EExtension::SD_EXTENSION_ERROR;
    }
    
    static std::string extensionToString(const EExtension &descriptor){
        auto pair = extensionDict.find(descriptor);
        if (pair == extensionDict.end()) return std::string();
        return pair->second.name;
    }
    
    static std::string descriptorToString(const EDescriptor &descriptor) {
        for(auto it = extensionDict.begin(); it != extensionDict.end();it++){
            auto item =  it->second.members.find(descriptor);
            if(item != it->second.members.end()) return item->second;
        }
        return std::string();
    }

    static EExtension stringToExtension(std::string string){
        for(auto it = extensionDict.begin(); it != extensionDict.end();it++){
            if((*it).second.name == string) return (*it).first;
        }
        return EExtension::SD_EXTENSION_ERROR;
    }
    
    static EDescriptor stringToDescriptor(EExtension extension, std::string string){
        auto ext = extensionDict.find(extension);
        if(ext == extensionDict.end()) return EDescriptor::SD_ERROR;

        auto map = ext->second.members;
        for(auto jt = map.begin(); jt != map.end(); jt++){
            if((*jt).second == string) return (*jt).first;
        }
        return EDescriptor::SD_ERROR;
    }
    
    static EDescriptor stringToDescriptor(std::string string){
        for(auto it = extensionDict.begin(); it != extensionDict.end(); it++){
            auto desc = stringToDescriptor((*it).first , string);
            if(desc != EDescriptor::SD_ERROR) return desc;
        }
        return EDescriptor::SD_ERROR;
    }
};



/*!
 utility function that converts a single number of vector of number to a string
 */
template <typename T, size_t size>
inline std::string toString(const std::array<T, size> &array){
    
    std::ostringstream os;
    std:: for_each(array.begin(), array.end(), [&os](T item){
        os << item << ' ';
    });
    std::string str = os.str();
    str.erase(str.size()-1);
    return std::move(str);
}

inline std::string toString(const bool &bl){
    return bl ? std::string("true") : std::string("false");
}

inline std::string toString(const std::string &str){
    return str;
}

template <typename T>
inline std::string toString(const T &i){
    return toString(std::array<T, 1>({{i}}));
}



/*!
 The Descriptor traits. implemented with template explicit specialization technique
 */


template <EDescriptor D>
struct sdDescriptor {};

template <>
struct sdDescriptor<EDescriptor::SD_TYPE>{
    typedef EType type;
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

template <>
struct sdDescriptor<EDescriptor::SD_MEDIA_ID>{
    typedef std::string type;
    const static bool interpolable = false;
};

template <>
struct sdDescriptor<EDescriptor::SD_MEDIA_TYPE>{
    typedef std::string type;
    const static bool interpolable = false;
};

template <>
struct sdDescriptor<EDescriptor::SD_MEDIA_LOCATION>{
    typedef std::string type;
    const static bool interpolable = false;
};

template <>
struct sdDescriptor<EDescriptor::SD_MEDIA_CHANNEL>{
    typedef int type;
    const static bool interpolable = false;
};

template <>
struct sdDescriptor<EDescriptor::SD_MEDIA_TIME_OFFSET>{
    typedef double type;
    const static bool interpolable = false;
};

template <>
struct sdDescriptor<EDescriptor::SD_MEDIA_GAIN>{
    typedef double type;
    const static bool interpolable = true;
};

template <>
struct sdDescriptor<EDescriptor::SD_SOURCE_WIDTH_WIDTH>{
    typedef double type;
    const static bool interpolable = true;
};

/*!
 check almost equal for double precision
 */
inline bool almostEqual(double x, double y){
    const double gap = std::abs(x-y);
    const double epsilon = std::numeric_limits<double>::epsilon() * std::abs(x+y) * 2.0;
    const double min = std::numeric_limits<double>::min();
    return (gap < epsilon) || (gap < min);
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


#endif
