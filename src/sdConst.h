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
    static constants
 */
static const double kDegreeToRadian = M_PI/180.0;
static const double kRadianToDegree = 180.0/M_PI;


/*!
    enum for descriptor. internally all descriptors are handled with this Enum
 */
typedef enum {
    
    /*** core ***/
    // core
    SD_TYPE,
    SD_PRESENT,
    SD_POSITION,
    SD_ORIENTATION,
    
    /** core functionalities **/
    // media
    SD_MEDIA_ID,
    SD_MEDIA_TYPE,
    SD_MEDIA_LOCATION,
    SD_MEDIA_CHANNEL,
    SD_MEDIA_TIME_OFFSET,
    SD_MEDIA_GAIN,
    
    // loop
    SD_LOOP_TYPE,
    SD_LOOP_POINTS,
    SD_LOOP_WAIT_TIME,
    
    // interpolation
    SD_INTERPOLATION_TYPE,
    
    /*** extensions ***/
    /** general extensions **/

    // pointset
    SD_POINTSET_ID,
    SD_POINTSET_UNIT,
    SD_POINTSET_CLOSED,
    SD_POINTSET_SIZE,
    SD_POINTSET_POINT,
    SD_POINTSET_HANDLE,
    
    // geometry
    SD_GEOMETRY_TRANSLATE,
    SD_GEOMETRY_SCALE,
    SD_GEOMETRY_ROTATE,
    SD_GEOMETRY_SHEAR,
    SD_GEOMETRY_REFLECT,
    
    // automation
    SD_AUTOMATION_DURATION,
    SD_AUTOMATION_DELAY,
    SD_AUTOMATION_FUNCTION,
    SD_AUTOMATION_POINTSET,
    SD_AUTOMATION_LOOP,
    
    // shape
    SD_SHAPE_DIRECTION,
    SD_SHAPE_CLOSED,
    SD_SHAPE_TYPE,
    SD_SHAPE_ID,
    
    /** layer-related extensions **/

    // descriptor for source spread extension
    SD_SOURCE_SPREAD_SPREAD,

    // descriptor for hardware out
    SD_HARDWARE_OUT_PHYSICAL_CHANNEL,
    SD_HARDWARE_OUT_GAIN,
    
    // misc
    SD_ERROR,
    SD_ALL
} EDescriptor;


/*!
 enum for "type" descriptor.
 */
enum class EType{
    SD_POINT,
    SD_LOUDSPEAKER,
    SD_LISTENER,
    SD_MICROPHONE,
    SD_UNDEFINED
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
    //core
    SD_CORE,
    SD_MEDIA,
    SD_INTERPOLATION,

    //general
    SD_POINTSET,
    SD_GEOMETRY,
    SD_AUTTOMATION,
    SD_SHAPE,
    
    //layer-related
    SD_TRAJECTPRY_GENERATOR,
    SD_SOURCE_SPREAD,
    SD_DISTANCE_CUE,
    SD_SINK_ENTITY,
    SD_DIRECT_TO_ONE,
    SD_HARDWARE_OUT,
    
    SD_EXTENSION_ERROR
};

/*!
 enum for interpolation types
 */
typedef enum {
    SD_NONE,
    SD_LINEAR,
    SD_CUBIC_BEZIER
} EInterpolation;


class sdEntity;
class sdProtoEvent;
class sdProtoMeta;
class sdExtension {
protected:
    struct sdDSpec{
        sdDSpec(EDescriptor descriptor, std::string descriptorString, std::function< std::shared_ptr<sdProtoEvent>(sdEntity* entity,double, std::string)> addEventFromStringFunc): descriptor(descriptor), descriptorString(descriptorString), addEventFromStringFunc(addEventFromStringFunc){};
        EDescriptor descriptor;
        std::string descriptorString;
        std::function< std::shared_ptr<sdProtoEvent>(sdEntity * entity, double, std::string)> addEventFromStringFunc;
        std::function< std::shared_ptr<sdProtoMeta>(sdEntity * entity, std::string)> addMetaFromStringFunc;
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
    
    static std::function<std::shared_ptr<sdProtoMeta>(sdEntity* entity, std::string)> getAddMetaFunc(EDescriptor descriptor){
        auto ext = getExtensionOfDescriptor(descriptor);
        auto descriptors = getDescriptorsForExtension(ext);
        auto it = std::find_if(descriptors.begin(), descriptors.end(), [&descriptor](sdDSpec dSpec){ return dSpec.descriptor == descriptor;});
        if(it == descriptors.end())return nullptr;
        return (*it).addMetaFromStringFunc;
    }
};
#pragma mark methmatical conversion

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
    convert azimuth, elevation, radius to xyz
 */

inline std::array<double, 3> aedToXyz(const std::array<double,3> &aed){
    std::array<double, 3> xyz;
    const double a = aed[0];
    const double e = aed[1];
    const double d = aed[2];
    xyz[0] = sin(a*kDegreeToRadian) * cos(e*kDegreeToRadian) * d;
    xyz[1] = cos(a*kDegreeToRadian) * cos(e*kDegreeToRadian) * d;
    xyz[2] = sin(e*kDegreeToRadian) * d;
    return std::move(xyz);
}

inline std::array<double, 3> xyzToAed(const std::array<double,3> &xyz){
    std::array<double, 3> aed;
    const double x = xyz[0];
    const double y = xyz[1];
    const double z = xyz[2];
    aed[0] = atan2(x,y) * kRadianToDegree;
    aed[1] = atan2(z,sqrt(x*x + y*y)) * kRadianToDegree;
    aed[2] = sqrt(x*x+y*y+z*z);
    return std::move(aed);
}

#pragma mark toString

/*! template function for to string conversion
    this function reacts differently, when the argument type is bool, EType or string.
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

#pragma mark stringTo

/*! 
 converstion from string to a specific type of data 
 */

template <typename T>
inline T stringTo(const std::string &str){
    return std::stod(str);
}

template <>
inline int stringTo(const std::string &str){
    return std::stoi(str);
}

template <>
inline bool stringTo(const std::string &str){
    return str == "true";
};

template <>
inline EType stringTo(const std::string &str){
    if(str == "point")return EType::SD_POINT;
    if(str == "loudspeaker") return EType::SD_LOUDSPEAKER;
    if(str == "listner") return EType::SD_LISTENER;
    if(str == "microphone") return EType::SD_MICROPHONE;
    return EType::SD_UNDEFINED;
}

template <>
inline EInterpolation stringTo(const std::string &str){
    if(str == "none")return EInterpolation::SD_NONE;
    if(str == "linear") return EInterpolation::SD_LINEAR;
    if(str == "cubic_bezier") return EInterpolation::SD_CUBIC_BEZIER;
    return EInterpolation::SD_LINEAR;
}

template <typename T, size_t S>
inline std::array<T, S> stringToArray(const std::string &str){
    std::istringstream iss(str);
    std::vector<T> strVect;
    for(int i = 0; i < S; i++){
        std::string string;
        iss >> string;
        strVect.push_back(std::stod(string));
    }
    std::array<T, S> array;
    std::copy_n(std::make_move_iterator(strVect.begin()), S, array.begin());
    return std::move(array);
}




#endif
