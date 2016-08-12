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
#include <unordered_map>
#include "sdException.h"

/*!
    static constants
 */
static const double kDegreeToRadian = M_PI/180.0;
static const double kRadianToDegree = 180.0/M_PI;



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

    //5.2 general
    SD_POINTSET,
    SD_GEOMETRY,
    SD_AUTTOMATION,
    SD_SHAPE,
    
    //5.3 layer-related
    SD_TRAJECTPRY_GENERATOR,
    SD_SOURCE_SPREAD,
    SD_DISTANCE_CUES,
    SD_SINK_ENTITY,
    SD_DIRECT_TO_ONE,
    SD_HARDWARE_OUT,
    SD_GROUP,
    
    
    SD_EXTENSION_ERROR
};



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
    
    /*** 5 extensions ***/
    /** 5.2 general extensions **/
    
    /// 5.2.1 pointset
    SD_POINTSET_ID,
    SD_POINTSET_UNIT,
    SD_POINTSET_CLOSED,
    SD_POINTSET_SIZE,
    SD_POINTSET_POINT,
    SD_POINTSET_HANDLE,
    
    /// 5.2.2 geometry
    SD_GEOMETRY_TRANSLATE,
    SD_GEOMETRY_SCALE,
    SD_GEOMETRY_ROTATE,
    SD_GEOMETRY_SHEAR,
    SD_GEOMETRY_REFLECT,
    
    /// 5.2.3 automation
    SD_AUTOMATION_DURATION,
    SD_AUTOMATION_DELAY,
    SD_AUTOMATION_FUNCTION,
    SD_AUTOMATION_POINTSET,
    SD_AUTOMATION_LOOP,
    
    /// 5.2.4 shape
    SD_SHAPE_DIRECTION,
    SD_SHAPE_CLOSED,
    SD_SHAPE_TYPE,
    SD_SHAPE_ID,
    
    /** 5.3 layer-related extensions **/
    
    /// 5.3.1 trjactory generator
    
    // 5.3.1.2 group
    SD_GROUP_NAME,
    SD_GROUP_PRESENT,
    SD_GROUP_POSITION,
    SD_GROUP_ORIENTATION,
    
    SD_GROUP_MEMBERSHIP, // descriptor for source entity
    
    /// 5.3.2 Extensions for scene description layer
    // 5.3.2.1 Source Spread
    SD_SOURCE_SPREAD_SPREAD,
    
    /// 5.3.3 Extensions for spatial encoding layer
    // 5.3.3.1 Distance cues
    SD_DISTANCE_CUES_REFERENCE_DISTANCE,
    SD_DISTANCE_CUES_MAXIMUM_DISTANCE,
    SD_DISTANCE_CUES_MAXIMUM_ATTENUATION,
    SD_DISTANCE_CUES_ATTENUATION_MODEL,
    SD_DISTANCE_CUES_ABSORPTION_MODEL,
    
    // 5.3.4 Extensions for Spatial Decoding Layer
    /// 5.3.4.1 Sink Entity (descriptor same as core)
    
    /// 5.3.4.2 Direct-to-One Sink
    SD_DIRECT_TO_ONE_DIRECT_TO_ONE,
    
    /// 5.3.5 Extensions for Hardware Abstractions Layer
    // 5.3.5.1 Hareware out

    SD_HARDWARE_OUT_PHYSICAL_CHANNEL,
    SD_HARDWARE_OUT_GAIN,
    
    // misc
    SD_ERROR,
    SD_ALL
} EDescriptor;



/*!
 string to from conversion template utility
 */

template <typename T, int N>
std::string toStringUtil(const T &value, std::array<std::pair<T, std::string>, N> &table){
    auto type = std::find_if(table.begin(), table.end(), [&value](std::pair<T, std::string> &typePair){return value == typePair.first;});
    return type == table.end() ? "undefined" : (*type).second;
}

template <typename T, int N>
T stringToUtil(const std::string &str, std::array<std::pair<T, std::string>, N> &table){
    auto type = std::find_if(table.begin(), table.end(), [&str](std::pair<T, std::string> &typePair){return str == typePair.second;});
    return type == table.end() ? T::SD_UNDEFINED : (*type).first;
}

/*!
 The traits (properties or characters) of each descriptor.
 implemented with explicit specialization template technique
 */

template <EDescriptor D>
struct sdDescriptor {};

/*** core ***/
/// core
template <>
struct sdDescriptor<EDescriptor::SD_TYPE>{
    enum EType{
        SD_POINT,
        SD_LOUDSPEAKER,
        SD_LISTENER,
        SD_MICROPHONE,
        SD_UNDEFINED
    };
    typedef EType type;
    const static bool interpolable = false;

    // enum to string conversion and vice versa
    static std::array<std::pair<EType, std::string>,4> &table(){
        static std::array<std::pair<EType, std::string>,4> table=
         {std::make_pair(EType::SD_POINT, "point") ,
            std::make_pair(EType::SD_LOUDSPEAKER, "loudspeaker"),
            std::make_pair(EType::SD_LISTENER, "listener"),
             std::make_pair(EType::SD_MICROPHONE, "microphone")};
        return table;
    }
    static EType stringTo(const std::string &str){return stringToUtil<EType,4>(str, table());}
    static std::string toString(const EType &value){return toStringUtil<EType,4>(value, table());}
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
struct sdDescriptor<EDescriptor::SD_GROUP_MEMBERSHIP>{
    typedef std::string type;
    const static bool interpolable = false;
};

/// core functionalities descriptors
// media

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
struct sdDescriptor<EDescriptor::SD_INTERPOLATION_TYPE>{
    
    typedef enum {
        SD_NONE,
        SD_LINEAR,
        SD_CUBIC_BEZIER,
        SD_UNDEFINED
    } EInterpolation;
    
    typedef EInterpolation type;
    const static bool interpolable = false;
    static std::array<std::pair<EInterpolation, std::string>,3> &table(){
        static std::array<std::pair<EInterpolation, std::string>,3> table={
            std::make_pair(EInterpolation::SD_NONE, "none"),
            std::make_pair(EInterpolation::SD_LINEAR, "linear"),
            std::make_pair(EInterpolation::SD_CUBIC_BEZIER, "cubic_bezier")};
        return table;
    }
    static EInterpolation stringTo(const std::string &str){return stringToUtil<EInterpolation,3>(str, table());}
    static std::string toString(const EInterpolation &value){return toStringUtil<EInterpolation, 3>(value, table());}
};


/********************************************************
 5. extensions
 *********************************************************/



// hardware-out
template <>
struct sdDescriptor<EDescriptor::SD_HARDWARE_OUT_PHYSICAL_CHANNEL>{
    typedef int type;
    const static bool interpolable = false;
};

template <>
struct sdDescriptor<EDescriptor::SD_HARDWARE_OUT_GAIN>{
    typedef double type;
    const static bool interpolable = true;
};

//***** 5.2. General Extensions *****//

/// 5.2.1 Pointset

template <>
struct sdDescriptor<EDescriptor::SD_POINTSET_ID>{
    typedef std::string type;
    const static bool interpolable = false;
};

template <>
struct sdDescriptor<EDescriptor::SD_POINTSET_UNIT>{
    typedef std::string type;
    const static bool interpolable = false;
};

template <>
struct sdDescriptor<EDescriptor::SD_POINTSET_CLOSED>{
    typedef bool type;
    const static bool interpolable = false;
};

template <>
struct sdDescriptor<EDescriptor::SD_POINTSET_SIZE>{
    typedef int type;
    const static bool interpolable = false;
};

template <>
struct sdDescriptor<EDescriptor::SD_POINTSET_POINT>{
    typedef std::array<double, 3> type;
    const static bool interpolable = false;
};

template <>
struct sdDescriptor<EDescriptor::SD_POINTSET_HANDLE>{
    typedef std::array<double, 3> type;
    const static bool interpolable = false;
};

/// 5.2.2 Geometry
template <>
struct sdDescriptor<EDescriptor::SD_GEOMETRY_TRANSLATE>{
    typedef std::array<double, 3> type;
    const static bool interpolable = true;
};

template <>
struct sdDescriptor<EDescriptor::SD_GEOMETRY_SCALE>{
    typedef std::array<double, 3> type;
    const static bool interpolanle = true;
};

template <>
struct sdDescriptor<EDescriptor::SD_GEOMETRY_ROTATE>{
    typedef std::array<double, 3> type;
    const static bool interpolanle = true;
};

template <>
struct sdDescriptor<EDescriptor::SD_GEOMETRY_SHEAR>{
    typedef std::array<double, 3> type;
    const static bool interpolable = true;
};

template <>
struct sdDescriptor<EDescriptor::SD_GEOMETRY_REFLECT>{
    typedef std::array<int, 3> type;
    const static bool interpolable = true;
};


/// 5.2.3 Automation

template <>
struct sdDescriptor<EDescriptor::SD_AUTOMATION_DURATION>{
    typedef double type;
    const static bool interpolable = false;
};

template <>
struct sdDescriptor<EDescriptor::SD_AUTOMATION_DELAY>{
    typedef double type;
    const static bool interpolable = false;
};

template <>
struct sdDescriptor<EDescriptor::SD_AUTOMATION_FUNCTION>{
    typedef std::string type;
    const static bool interpolable = false;
};

/////// pointset -- extension
/////// loop -- extension

/// 5.2.4 Shape

template <>
struct sdDescriptor<EDescriptor::SD_SHAPE_DIRECTION>{
    typedef bool type; // CCW, CW
    const static bool interpolable = false;
};

template <>
struct sdDescriptor<EDescriptor::SD_SHAPE_CLOSED>{
    typedef bool type;
    const static bool interpolable = false;
};

template <>
struct sdDescriptor<EDescriptor::SD_SHAPE_TYPE>{
    typedef std::string type;
    const static bool interpolable = false;
};

template <>
struct sdDescriptor<EDescriptor::SD_SHAPE_ID>{
    typedef std::string type;
    const static bool interpolable = false;
};

//***** 5.3. Layer-Related Extensions *****//

/// 5.3.1 Extensions for Spatial Authoring Layer

// 5.3.1.1 Trajectory Generator

// 5.3.1.2 Group
template <>
struct sdDescriptor<EDescriptor::SD_GROUP_NAME>{
    typedef std::string type;
    const static bool interpolable = false;
};

template <>
struct sdDescriptor<EDescriptor::SD_GROUP_PRESENT>{
    typedef bool type;
    const static bool interpolable = false;
};

template <>
struct sdDescriptor<EDescriptor::SD_GROUP_POSITION>{
    typedef std::array<double , 3> type;
    const static bool interpolable = true;
};

template <>
struct sdDescriptor<EDescriptor::SD_GROUP_ORIENTATION>{
    typedef std::array<double, 3 > type;
    const static bool interpolable = true;
};

/// 5.3.2 Extensions for Scene Description Layer
// 5.3.2.1 Source Spread

template <>
struct sdDescriptor<EDescriptor::SD_SOURCE_SPREAD_SPREAD>{
    typedef double type;
    const static bool interpolable = true;
};

/// 5.3.3 Extension for Spatial Encoding Layer
//5.3.3.1 Distance-cues

template <>
struct sdDescriptor<EDescriptor::SD_DISTANCE_CUES_REFERENCE_DISTANCE>{
    typedef double type;
    const static bool interpolable = true;
};

template <>
struct sdDescriptor<EDescriptor::SD_DISTANCE_CUES_MAXIMUM_DISTANCE>{
    typedef double type;
    const static bool interpolable = true;
};

template <>
struct sdDescriptor<EDescriptor::SD_DISTANCE_CUES_MAXIMUM_ATTENUATION>{
    typedef double type;
    const static bool interpolable = true;
};

template <>
struct sdDescriptor<EDescriptor::SD_DISTANCE_CUES_ATTENUATION_MODEL>{
    typedef int type;
    const static bool interpolable = false;
};

template <>
struct sdDescriptor<EDescriptor::SD_DISTANCE_CUES_ABSORPTION_MODEL>{
    typedef int type;
    const static bool interpolable = false;
};

// 5.3.4 Extensions for Spatial Decoding Layer
/// 5.3.4.1 Sink Entity (descriptor same as core)


/// 5.3.4.2 Direct-to-One Sink
template <>
struct sdDescriptor<EDescriptor::SD_DIRECT_TO_ONE_DIRECT_TO_ONE>{
    typedef bool type;
    const static bool interpolable = false;
};



#pragma mark toString

/*! template function for to string conversion
 this function reacts differently, when the argument type is bool, enum or string.
 if the argument type is enum, it refers a specific template of the descriptor
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

inline std::string toString(const sdDescriptor<SD_TYPE>::EType &type){
    return sdDescriptor<SD_TYPE>::toString(type);
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
inline sdDescriptor<SD_TYPE>::EType stringTo(const std::string &str){
    return sdDescriptor<SD_TYPE>::stringTo(str);
}

template <>
inline sdDescriptor<SD_INTERPOLATION_TYPE>::EInterpolation stringTo(const std::string &str){
    return stringTo<sdDescriptor<SD_INTERPOLATION_TYPE>::EInterpolation>(str);
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
