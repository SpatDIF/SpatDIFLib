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


/*** extensions ***/
/// general extensions

// pointset
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

#endif