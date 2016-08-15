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

#ifndef ____sdUtils__
#define ____sdUtils__

#pragma mark methmatical conversion

/*!
 static constants
 */
static const double kDegreeToRadian = M_PI/180.0;
static const double kRadianToDegree = 180.0/M_PI;


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

#endif
