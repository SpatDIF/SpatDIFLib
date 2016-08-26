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

#pragma once

#pragma mark methmatical conversion
/*!
 static constants
 */
static const double kDegreeToRadian = M_PI/180.0;
static const double kRadianToDegree = 180.0/M_PI;


class sdUtils{
public:
    /*!
     check almost equal for double precision
     */
     static bool almostEqual(double x, double y){
        const double gap = std::abs(x-y);
        const double epsilon = std::numeric_limits<double>::epsilon() * std::abs(x+y) * 2.0;
        const double min = std::numeric_limits<double>::min();
        return (gap < epsilon) || (gap < min);
    }

    /*!
     convert azimuth, elevation, radius to xyz
     */

    static std::array<double, 3> aedToXyz(const std::array<double,3> &aed){
        std::array<double, 3> xyz;
        const double a = aed[0];
        const double e = aed[1];
        const double d = aed[2];
        xyz[0] = sin(a*kDegreeToRadian) * cos(e*kDegreeToRadian) * d;
        xyz[1] = cos(a*kDegreeToRadian) * cos(e*kDegreeToRadian) * d;
        xyz[2] = sin(e*kDegreeToRadian) * d;
        return std::move(xyz);
    }

     static std::array<double, 3> xyzToAed(const std::array<double,3> &xyz){
        std::array<double, 3> aed;
        const double x = xyz[0];
        const double y = xyz[1];
        const double z = xyz[2];
        aed[0] = atan2(x,y) * kRadianToDegree;
        aed[1] = atan2(z,sqrt(x*x + y*y)) * kRadianToDegree;
        aed[2] = sqrt(x*x+y*y+z*z);
        return std::move(aed);
    }

    /*!
     string to from conversion template utility
     */

    template <typename T, int N>
    static std::string toStringByTable(const T &value, std::array<std::pair<T, std::string>, N> &table){
        auto type = std::find_if(table.begin(), table.end(), [&value](std::pair<T, std::string> &typePair){return value == typePair.first;});
        return type == table.end() ? "undefined" : (*type).second;
    }

    template <typename T, int N>
    static T stringToByTable(const std::string &str, std::array<std::pair<T, std::string>, N> &table){
        auto type = std::find_if(table.begin(), table.end(), [&str](std::pair<T, std::string> &typePair){return str == typePair.second;});
        if(type == table.end()) throw CanNotConvertStringException("cannot convert " + str);
        return  (*type).first;
    }
    
    
#pragma mark toString
    
    /*! template function for to string conversion
     this function reacts differently, when the argument type is bool, enum or string.
     if the argument type is enum, it refers a specific template of the descriptor
     */
    template <typename T, size_t size>
    static std::string toString(const std::array<T, size> &array){
        
        std::ostringstream os;
        std:: for_each(array.begin(), array.end(), [&os](T item){
            os << item << ' ';
        });
        std::string str = os.str();
        str.erase(str.size()-1);
        return std::move(str);
    }
    
    static std::string toString(const bool &bl){
        return bl ? std::string("true") : std::string("false");
    }
    
    static std::string toString(const std::string &str){
        return str;
    }
    
    template <typename T>
    static std::string toString(const T &i){
        return toString(std::array<T, 1>({{i}}));
    }
    
#pragma mark stringTo
    
    /*!
     converstion from string to a specific type of data
     */
    
    template <typename T>
    static T stringTo(const std::string &str);

    
    template <typename T, size_t S>
    static std::array<T, S> stringToArray(const std::string &str){
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
};

template <typename T>
inline T sdUtils::stringTo(const std::string &str){
    return std::stod(str);
}

template <>
inline int sdUtils::stringTo(const std::string &str){
    return std::stoi(str);
}

template <>
inline bool sdUtils::stringTo(const std::string &str){
    if(str != "true" && str != "false"){ throw CanNotConvertStringException(str);}
    return str == "true";
};

