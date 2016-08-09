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

#ifndef ____sdTraits__
#define ____sdTraits__

#include "sdConst.h"

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
    typedef EType type;
    const static bool interpolable = false;
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

// interpolation
template <>
struct sdDescriptor<EDescriptor::SD_INTERPOLATION_TYPE>{
    typedef std::string type;
    const static bool interpolable = true;
};


/********************************************************
5. extensions
*********************************************************/


// source-spread
template <>
struct sdDescriptor<EDescriptor::SD_SOURCE_SPREAD_SPREAD>{
    typedef double type;
    const static bool interpolable = true;
};

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

// 5.3.2
#endif
