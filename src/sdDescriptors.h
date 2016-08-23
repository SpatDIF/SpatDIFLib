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
#include "sdUtils.h"



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
    SD_LOOP,
    SD_INTERPOLATION,

    //5.2 general
    SD_POINTSET,
    SD_GEOMETRY,
    SD_AUTTOMATION,
    SD_SHAPE,
    
    //5.3 layer-related
    SD_TRAJECTORY,
    SD_SOURCE_SPREAD,
    SD_DISTANCE_CUES,
    SD_SINK_ENTITY,
    SD_DIRECT_TO_ONE,
    SD_HARDWARE_OUT,
    SD_GROUP,
    
    //5.4 private
    SD_PRIVATE,
    
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
    /// 4.4.1 media
    SD_MEDIA_ID,
    SD_MEDIA_TYPE,
    SD_MEDIA_LOCATION,
    SD_MEDIA_CHANNEL,
    SD_MEDIA_TIME_OFFSET,
    SD_MEDIA_GAIN,
    
    /// 4.4.2 loop
    SD_LOOP_TYPE,
    SD_LOOP_POINTS,
    SD_LOOP_WAIT_TIME,
    
    /// 4.4.3 interpolation
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
    
    /// 5.3.1.1 trjactory generator
    SD_TRAJECTORY_POINTSET,
    SD_TRAJECTORY_INTERPOLATION,
    SD_TRAJECTORY_GEOMETRY,
    SD_TRAJECTORY_AUTOMATION,
    SD_TRAJECTORY_SHAPE,
    
    // 5.3.1.2 group
    SD_GROUP_ID,
    SD_GROUP_INTERPOLATION,
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
 The traits (properties or characters) of each descriptor.
 implemented with explicit specialization template technique
 */

template <EDescriptor D>
struct sdDescriptor {};

/*** core ***/
/// 4.3 core
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
    static type stringTo(const std::string &str){return sdUtils::stringToByTable<type,4>(str, table());}
    static std::string toString(const type &value){return sdUtils::toStringByTable<type,4>(value, table());}
};

template <>
struct sdDescriptor<EDescriptor::SD_PRESENT>{
    typedef bool type;
    const static bool interpolable = false;
    
    static type stringTo(const std::string &str){return sdUtils::stringTo<type>(str);}
    static std::string toString(const type &value){return sdUtils::toString(value);}

};

template <>
struct sdDescriptor<EDescriptor::SD_POSITION>{
    typedef std::array<double, 3> type;
    const static bool interpolable = true;
    
    static type stringTo(const std::string &str){return sdUtils::stringToArray<double, 3>(str);}
    static std::string toString(const type &value){return sdUtils::toString(value);}
};

template <>
struct sdDescriptor<EDescriptor::SD_ORIENTATION>{
    typedef std::array<double, 3> type;
    const static bool interpolable = true;
    
    static type stringTo(const std::string &str){return sdUtils::stringToArray<double, 3>(str);}
    static std::string toString(const type &value){return sdUtils::toString(value);}

};

template <>
struct sdDescriptor<EDescriptor::SD_GROUP_MEMBERSHIP>{
    typedef std::string type;
    const static bool interpolable = false;
    
    static type stringTo(const std::string &str){return str;}
    static std::string toString(const type &value){return value;}

};

/** core functionalities **/
/// 4.4.1 media

template <>
struct sdDescriptor<EDescriptor::SD_MEDIA_ID>{
    typedef std::string type;
    const static bool interpolable = false;

    static type stringTo(const std::string &str){return str;}
    static std::string toString(const type &value){return value;}
};

template <>
struct sdDescriptor<EDescriptor::SD_MEDIA_TYPE>{
    typedef std::string type;
    const static bool interpolable = false;
    
    static type stringTo(const std::string &str){return str;}
    static std::string toString(const type &value){return value;}
};

template <>
struct sdDescriptor<EDescriptor::SD_MEDIA_LOCATION>{
    typedef std::string type;
    const static bool interpolable = false;
    
    static type stringTo(const std::string &str){return str;}
    static std::string toString(const type &value){return value;}
};

template <>
struct sdDescriptor<EDescriptor::SD_MEDIA_CHANNEL>{
    typedef int type;
    const static bool interpolable = false;
    
    static type stringTo(const std::string &str){return sdUtils::stringTo<type>(str);}
    static std::string toString(const type &value){return sdUtils::toString(value);}
};

template <>
struct sdDescriptor<EDescriptor::SD_MEDIA_TIME_OFFSET>{
    typedef double type;
    const static bool interpolable = false;
    
    static type stringTo(const std::string &str){return sdUtils::stringTo<type>(str);}
    static std::string toString(const type &value){return sdUtils::toString(value);}
};

template <>
struct sdDescriptor<EDescriptor::SD_MEDIA_GAIN>{
    typedef double type;
    const static bool interpolable = true;
    
    static type stringTo(const std::string &str){return sdUtils::stringTo<type>(str);}
    static std::string toString(const type &value){return sdUtils::toString(value);}
};

/// 4.4.2 loop
template <>
struct sdDescriptor<EDescriptor::SD_LOOP_TYPE>{
    
    typedef enum{
        SD_NONE,
        SD_REPEAT,
        SD_UNDEFINED
    } EType;
    
    typedef std::pair<EType, int> type;
    const static bool interpolable = false;
    static std::array<std::pair<EType, std::string>,2> &table(){
        static std::array<std::pair<EType, std::string>,2> table={
            std::make_pair(EType::SD_NONE, "none"),
            std::make_pair(EType::SD_REPEAT, "repeat")};
        return table;
    }
    static std::pair<EType, int>  stringTo(const std::string &str){
        std::istringstream iss(str);
        std::vector<std::string> items{std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>{}};
        if(items.size() > 2) return std::make_pair(EType::SD_UNDEFINED, 0);
        EType tp =  sdUtils::stringToByTable<EType,2>(items[0], table());
        if(tp == SD_REPEAT){
            return std::make_pair(tp,std::stoi(items[1]));
        }
        return  std::make_pair(tp, 0);
    }
    static std::string toString(const std::pair<EType, int>  &value){
        std::string tp = sdUtils::toStringByTable<EType, 2>(value.first, table());
        tp += " " + std::to_string(value.second);
        return tp;
    }
    
};

template <>
struct sdDescriptor<EDescriptor::SD_LOOP_POINTS>{
    typedef std::array<double, 2> type;
    const static bool interpolable = false;
    
    static type stringTo(const std::string &str){return sdUtils::stringToArray<double, 2>(str);}
    static std::string toString(const type &value){return sdUtils::toString(value);}
};

template <>
struct sdDescriptor<EDescriptor::SD_LOOP_WAIT_TIME>{
    typedef double type;
    const static bool interpolable = true;
    
    static type stringTo(const std::string &str){return sdUtils::stringTo<type>(str);}
    static std::string toString(const type &value){return sdUtils::toString(value);}
};



/// 4.4.3 interpolation
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
    
    static type stringTo(const std::string &str){return sdUtils::stringToByTable<type,3>(str, table());}
    static std::string toString(const type &value){return sdUtils::toStringByTable<type, 3>(value, table());}
};


/********************************************************
 5. extensions
 *********************************************************/



//***** 5.2. General Extensions *****//

/// 5.2.1 Pointset

template <>
struct sdDescriptor<EDescriptor::SD_POINTSET_ID>{
    typedef std::string type;
    const static bool interpolable = false;
    
    static type stringTo(const std::string &str){return str;}
    static std::string toString(const type &value){return value;}
};

template <>
struct sdDescriptor<EDescriptor::SD_POINTSET_UNIT>{
    typedef std::string type;
    const static bool interpolable = false;
    
    static type stringTo(const std::string &str){return str;}
    static std::string toString(const type &value){return value;}
};

template <>
struct sdDescriptor<EDescriptor::SD_POINTSET_CLOSED>{
    typedef bool type;
    const static bool interpolable = false;
    
    static type stringTo(const std::string &str){return sdUtils::stringTo<type>(str);}
    static std::string toString(const type &value){return sdUtils::toString(value);}
};

template <>
struct sdDescriptor<EDescriptor::SD_POINTSET_SIZE>{
    typedef int type;
    const static bool interpolable = false;
    
    static type stringTo(const std::string &str){return sdUtils::stringTo<type>(str);}
    static std::string toString(const type &value){return sdUtils::toString(value);}
};

template <>
struct sdDescriptor<EDescriptor::SD_POINTSET_POINT>{
    typedef std::array<double, 3> type;
    const static bool interpolable = false;
    
    static type stringTo(const std::string &str){return sdUtils::stringToArray<double, 3>(str);}
    static std::string toString(const type &value){return sdUtils::toString(value);}
};

template <>
struct sdDescriptor<EDescriptor::SD_POINTSET_HANDLE>{
    typedef std::array<double, 3> type;
    const static bool interpolable = false;
    
    static type stringTo(const std::string &str){return sdUtils::stringToArray<double, 3>(str);}
    static std::string toString(const type &value){return sdUtils::toString(value);}
};

/// 5.2.2 Geometry
template <>
struct sdDescriptor<EDescriptor::SD_GEOMETRY_TRANSLATE>{
    typedef std::array<double, 3> type;
    const static bool interpolable = true;
    
    static type stringTo(const std::string &str){return sdUtils::stringToArray<double, 3>(str);}
    static std::string toString(const type &value){return sdUtils::toString(value);}
};

template <>
struct sdDescriptor<EDescriptor::SD_GEOMETRY_SCALE>{
    typedef std::array<double, 3> type;
    const static bool interpolanle = true;
    
    static type stringTo(const std::string &str){return sdUtils::stringToArray<double, 3>(str);}
    static std::string toString(const type &value){return sdUtils::toString(value);}
};

template <>
struct sdDescriptor<EDescriptor::SD_GEOMETRY_ROTATE>{
    typedef std::array<double, 3> type;
    const static bool interpolanle = true;
    
    static type stringTo(const std::string &str){return sdUtils::stringToArray<double, 3>(str);}
    static std::string toString(const type &value){return sdUtils::toString(value);}
};

template <>
struct sdDescriptor<EDescriptor::SD_GEOMETRY_SHEAR>{
    typedef std::array<double, 3> type;
    const static bool interpolable = true;
    
    static type stringTo(const std::string &str){return sdUtils::stringToArray<double, 3>(str);}
    static std::string toString(const type &value){return sdUtils::toString(value);}
};

template <>
struct sdDescriptor<EDescriptor::SD_GEOMETRY_REFLECT>{
    typedef std::array<int, 3> type;
    const static bool interpolable = true;
    
    static type stringTo(const std::string &str){return sdUtils::stringToArray<int, 3>(str);}
    static std::string toString(const type &value){return sdUtils::toString(value);}
};


/// 5.2.3 Automation

template <>
struct sdDescriptor<EDescriptor::SD_AUTOMATION_DURATION>{
    typedef double type;
    const static bool interpolable = false;
    
    static type stringTo(const std::string &str){return sdUtils::stringTo<type>(str);}
    static std::string toString(const type &value){return sdUtils::toString(value);}

};

template <>
struct sdDescriptor<EDescriptor::SD_AUTOMATION_DELAY>{
    typedef double type;
    const static bool interpolable = false;
    
    static type stringTo(const std::string &str){return sdUtils::stringTo<type>(str);}
    static std::string toString(const type &value){return sdUtils::toString(value);}

};

template <>
struct sdDescriptor<EDescriptor::SD_AUTOMATION_FUNCTION>{
    typedef std::string type;
    const static bool interpolable = false;
    
    static type stringTo(const std::string &str){return str;}
    static std::string toString(const type &value){return value;}
};

/////// pointset -- extension
/////// loop -- extension

/// 5.2.4 Shape

template <>
struct sdDescriptor<EDescriptor::SD_SHAPE_DIRECTION>{
    typedef bool type; // CCW, CW
    const static bool interpolable = false;
    
    static type stringTo(const std::string &str){return sdUtils::stringTo<type>(str);}
    static std::string toString(const type &value){return sdUtils::toString(value);}
};

template <>
struct sdDescriptor<EDescriptor::SD_SHAPE_CLOSED>{
    typedef bool type;
    const static bool interpolable = false;
    
    static type stringTo(const std::string &str){return sdUtils::stringTo<type>(str);}
    static std::string toString(const type &value){return sdUtils::toString(value);}
};

template <>
struct sdDescriptor<EDescriptor::SD_SHAPE_TYPE>{
    typedef std::string type;
    const static bool interpolable = false;
    
    static type stringTo(const std::string &str){return str;}
    static std::string toString(const type &value){return value;}
};

template <>
struct sdDescriptor<EDescriptor::SD_SHAPE_ID>{
    typedef std::string type;
    const static bool interpolable = false;
    
    static type stringTo(const std::string &str){return str;}
    static std::string toString(const type &value){return value;}
};

//***** 5.3. Layer-Related Extensions *****//

/// 5.3.1 Extensions for Spatial Authoring Layer

// 5.3.1.1 Trajectory Generator
template <>
struct sdDescriptor<EDescriptor::SD_TRAJECTORY_POINTSET>{
    typedef std::string type;
    const static bool interpolable = false;
    
    static type stringTo(const std::string &str){return str;}
    static std::string toString(const type &value){return value;}
};

template <>
struct sdDescriptor<EDescriptor::SD_TRAJECTORY_INTERPOLATION>{
    typedef std::string type;
    const static bool interpolable = false;
    static type stringTo(const std::string &str){return str;}
    static std::string toString(const type &value){return value;}

};

template <>
struct sdDescriptor<EDescriptor::SD_TRAJECTORY_GEOMETRY>{
    typedef std::string type;
    const static bool interpolable = false;
    
    static type stringTo(const std::string &str){return str;}
    static std::string toString(const type &value){return value;}
};

template <>
struct sdDescriptor<EDescriptor::SD_TRAJECTORY_SHAPE>{
    typedef std::string type;
    const static bool interpolable = false;
    
    static type stringTo(const std::string &str){return str;}
    static std::string toString(const type &value){return value;}
};

template <>
struct sdDescriptor<EDescriptor::SD_TRAJECTORY_AUTOMATION>{
    typedef std::string type;
    const static bool interpolable = false;
    
    static type stringTo(const std::string &str){return str;}
    static std::string toString(const type &value){return value;}
};

// 5.3.1.2 Group
template <>
struct sdDescriptor<EDescriptor::SD_GROUP_ID>{
    typedef std::string type;
    const static bool interpolable = false;
    
    static type stringTo(const std::string &str){return str;}
    static std::string toString(const type &value){return value;}
};

template <>
struct sdDescriptor<EDescriptor::SD_GROUP_PRESENT>{
    typedef bool type;
    const static bool interpolable = false;
    
    static type stringTo(const std::string &str){return sdUtils::stringTo<type>(str);}
    static std::string toString(const type &value){return sdUtils::toString(value);}
};

template <>
struct sdDescriptor<EDescriptor::SD_GROUP_POSITION>{
    typedef std::array<double , 3> type;
    const static bool interpolable = true;
    
    static type stringTo(const std::string &str){return sdUtils::stringToArray<double, 3>(str);}
    static std::string toString(const type &value){return sdUtils::toString(value);}
};

template <>
struct sdDescriptor<EDescriptor::SD_GROUP_ORIENTATION>{
    typedef std::array<double, 3 > type;
    const static bool interpolable = true;
    
    static type stringTo(const std::string &str){return sdUtils::stringToArray<double, 3>(str);}
    static std::string toString(const type &value){return sdUtils::toString(value);}

};

/// 5.3.2 Extensions for Scene Description Layer
// 5.3.2.1 Source Spread

template <>
struct sdDescriptor<EDescriptor::SD_SOURCE_SPREAD_SPREAD>{
    typedef double type;
    const static bool interpolable = true;

    static type stringTo(const std::string &str){return sdUtils::stringTo<type>(str);}
    static std::string toString(const type &value){return sdUtils::toString(value);}
};

/// 5.3.3 Extension for Spatial Encoding Layer
//5.3.3.1 Distance-cues

template <>
struct sdDescriptor<EDescriptor::SD_DISTANCE_CUES_REFERENCE_DISTANCE>{
    typedef double type;
    const static bool interpolable = true;
    
    static type stringTo(const std::string &str){return sdUtils::stringTo<type>(str);}
    static std::string toString(const type &value){return sdUtils::toString(value);}
};

template <>
struct sdDescriptor<EDescriptor::SD_DISTANCE_CUES_MAXIMUM_DISTANCE>{
    typedef double type;
    const static bool interpolable = true;
    
    static type stringTo(const std::string &str){return sdUtils::stringTo<type>(str);}
    static std::string toString(const type &value){return sdUtils::toString(value);}

};

template <>
struct sdDescriptor<EDescriptor::SD_DISTANCE_CUES_MAXIMUM_ATTENUATION>{
    typedef double type;
    const static bool interpolable = true;

    static type stringTo(const std::string &str){return sdUtils::stringTo<type>(str);}
    static std::string toString(const type &value){return sdUtils::toString(value);}
};

template <>
struct sdDescriptor<EDescriptor::SD_DISTANCE_CUES_ATTENUATION_MODEL>{
    typedef int type;
    const static bool interpolable = false;
    
    static type stringTo(const std::string &str){return sdUtils::stringTo<type>(str);}
    static std::string toString(const type &value){return sdUtils::toString(value);}
};

template <>
struct sdDescriptor<EDescriptor::SD_DISTANCE_CUES_ABSORPTION_MODEL>{
    typedef int type;
    const static bool interpolable = false;
    
    static type stringTo(const std::string &str){return sdUtils::stringTo<type>(str);}
    static std::string toString(const type &value){return sdUtils::toString(value);}

};

// 5.3.4 Extensions for Spatial Decoding Layer
/// 5.3.4.1 Sink Entity (descriptor same as core)


/// 5.3.4.2 Direct-to-One Sink
template <>
struct sdDescriptor<EDescriptor::SD_DIRECT_TO_ONE_DIRECT_TO_ONE>{
    typedef bool type;
    const static bool interpolable = false;
    
    static type stringTo(const std::string &str){return sdUtils::stringTo<type>(str);}
    static std::string toString(const type &value){return sdUtils::toString(value);}

};

/// 5.3.5.1 Hardware-out
template <>
struct sdDescriptor<EDescriptor::SD_HARDWARE_OUT_PHYSICAL_CHANNEL>{
    typedef int type;
    const static bool interpolable = false;
    
    static type stringTo(const std::string &str){return sdUtils::stringTo<type>(str);}
    static std::string toString(const type &value){return sdUtils::toString(value);}
};

template <>
struct sdDescriptor<EDescriptor::SD_HARDWARE_OUT_GAIN>{
    typedef double type;
    const static bool interpolable = true;
    
    static type stringTo(const std::string &str){return sdUtils::stringTo<type>(str);}
    static std::string toString(const type &value){return sdUtils::toString(value);}
};


#endif