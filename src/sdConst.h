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
#include <vector>
#include "sdException.h"

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


class sdEntity;
class sdProtoEvent;
class sdExtension {
protected:
    struct sdDSpec{
        sdDSpec(EDescriptor descriptor, std::string descriptorString, std::function< std::shared_ptr<sdProtoEvent>(sdEntity* entity,double, std::string)> addEventFromStringFunc): descriptor(descriptor), descriptorString(descriptorString), addEventFromStringFunc(addEventFromStringFunc){};
        EDescriptor descriptor;
        std::string descriptorString;
        std::function< std::shared_ptr<sdProtoEvent>(sdEntity * entity, double, std::string)> addEventFromStringFunc;
    };

    struct sdESpec{
        sdESpec(EExtension extension, std::string extensionString, std::vector<sdDSpec> descriptorSpecs):extension(extension), extensionString(extensionString), descriptorSpecs(descriptorSpecs){};
        EExtension extension;
        std::string extensionString;
        std::vector<sdDSpec> descriptorSpecs;
    };


    const static std::vector<sdESpec> spatDIFSpec;

public:
    
#pragma mark extension
    static EExtension getExtensionOfDescriptor(EDescriptor descriptor){
        EExtension extension = EExtension::SD_EXTENSION_ERROR;
        auto it = std::find_if(spatDIFSpec.begin(), spatDIFSpec.end(), [&descriptor](sdESpec eSpec){
            auto itr = std::find_if(eSpec.descriptorSpecs.begin(), eSpec.descriptorSpecs.end(), [&descriptor](sdDSpec dSpec){
                return dSpec.descriptor == descriptor;
            });
            return itr != eSpec.descriptorSpecs.end();
        });
        if(it != spatDIFSpec.end()) extension = (*it).extension;
        return extension;
    }

    static std::string extensionToString(const EExtension &extension){
        auto it = std::find_if(spatDIFSpec.begin(), spatDIFSpec.end(), [&extension](sdESpec eSpec){return eSpec.extension == extension;});
        if (it == spatDIFSpec.end()) return std::string();
        return (*it).extensionString;
    }
    
    static EExtension stringToExtension(std::string string){
        auto it = std::find_if(spatDIFSpec.begin(), spatDIFSpec.end(),[&string](sdESpec eSpec){return eSpec.extensionString == string;});
        if(it != spatDIFSpec.end()) return (*it).extension;
        return EExtension::SD_EXTENSION_ERROR;
    }

#pragma mark descriptor

    static const std::vector<sdDSpec> &getDescriptorsForExtension(EExtension extension){
        auto itr = std::find_if(spatDIFSpec.begin(), spatDIFSpec.end(), [&extension](sdESpec eSpec){return eSpec.extension == extension;});
        if(itr == spatDIFSpec.end()){throw InvalidDescriptorException(); }
        return (*itr).descriptorSpecs;
    }

    static std::string descriptorToString(const EDescriptor &descriptor) {
        std::string str;
        std::find_if(spatDIFSpec.begin(), spatDIFSpec.end(), [&descriptor, &str](sdESpec eSpec){
            auto itr = std::find_if(eSpec.descriptorSpecs.begin(), eSpec.descriptorSpecs.end(), [&descriptor, &str](sdDSpec dSpec){
                if(dSpec.descriptor == descriptor){
                    str = dSpec.descriptorString;
                    return true;
                }else return false;
            });
            return itr != eSpec.descriptorSpecs.end();
        });
        return str;
    }


    static EDescriptor stringToDescriptor(EExtension extension, std::string string){
        auto it = std::find_if(spatDIFSpec.begin(), spatDIFSpec.end(),[&extension](sdESpec eSpec){return eSpec.extension == extension;});
        if(it == spatDIFSpec.end()) return SD_ERROR;
        auto dSpecs = (*it).descriptorSpecs;
        auto iit = std::find_if(dSpecs.begin(), dSpecs.end(), [&string](sdDSpec dSpec){return dSpec.descriptorString == string;});
        if( iit == dSpecs.end()) return SD_ERROR;
        return (*iit).descriptor;
    }

    static EDescriptor stringToDescriptor(std::string string){
        EDescriptor descriptor = SD_ERROR;
        std::find_if(spatDIFSpec.begin(), spatDIFSpec.end(), [&descriptor, &string](sdESpec eSpec){
            auto itr = std::find_if(eSpec.descriptorSpecs.begin(), eSpec.descriptorSpecs.end(), [&descriptor, &string](sdDSpec dSpec){
                if(dSpec.descriptorString == string){
                    descriptor  = dSpec.descriptor;
                    return true;
                }else return false;
            });
            return itr != eSpec.descriptorSpecs.end();
        });
        return descriptor;
    }
    
    static std::function<std::shared_ptr<sdProtoEvent>(sdEntity* entity, double,std::string)> getAddEventFunc(EDescriptor descriptor){
        auto ext = getExtensionOfDescriptor(descriptor);
        auto descriptors = getDescriptorsForExtension(ext);
        auto it = std::find_if(descriptors.begin(), descriptors.end(), [&descriptor](sdDSpec dSpec){ return dSpec.descriptor == descriptor;});
        if(it == descriptors.end())return nullptr;
        return (*it).addEventFromStringFunc;
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

inline std::string toString(const EType &type){
    return "point";
}

inline std::string toString(const std::string &str){
    return str;
}

template <typename T>
inline std::string toString(const T &i){
    return toString(std::array<T, 1>({{i}}));
}

template <typename T>
inline T stringTo(const std::string &str){
    
}

template <>
inline bool stringTo(const std::string &str){
    return str == "true";
};

template <>
inline std::array<double, 3> stringTo(const std::string &str){
    std::istringstream iss(str);
    std::vector<double> strVect;
    do {std::string string;
        iss >> string;
        strVect.push_back(std::stod(string));
    } while(iss);
    std::array<double, 3> array;
    std::copy_n(std::make_move_iterator(strVect.begin()), 3, array.begin());
    return std::move(array);
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
