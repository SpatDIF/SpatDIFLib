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
