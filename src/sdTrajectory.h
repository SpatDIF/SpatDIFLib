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

#ifndef ____sdTrajectory__
#define ____sdTrajectory__

#include <map>
#include <string>
#include <vector>

class sdPoint2D{
public:
    sdPoint2D(double x = 0.0, double y = 0.0);
    ~sdPoint2D();
    double x;
    double y;
};

class sdPoint2DCurve{
public:
    sdPoint2DCurve(double x = 0.0, double y = 0.0, double curveFactor = 0.0);
    ~sdPoint2DCurve();
    double x;
    double y;
    double curveFactor;
};


inline sdPoint2D::sdPoint2D(double x, double y):x(x),y(y){};
inline sdPoint2D::~sdPoint2D(){};

inline sdPoint2DCurve::sdPoint2DCurve(double x, double y, double curveFactor):x(x),y(y),curveFactor(curveFactor){};
inline sdPoint2DCurve::~sdPoint2DCurve(){};


class sdTrajectory{
public:
    sdTrajectory(std::string tp = "none");
    const std::string & getType();

protected:
    std::string type;
    virtual void clear() = 0;
};

inline sdTrajectory::sdTrajectory(std::string tp):type(tp){};
inline const std::string & sdTrajectory::getType(){
    return type;
}


template <typename T>
class sdTypedTrajectory : public sdTrajectory{
    //friend class sdScene; // only sdScene can instantiate this class. this guarantees that all trjaectory has affiliations.
    
private:
    std::vector< std::map< std::string, T> > pointArray;
    
public:
    sdTypedTrajectory(std::string tp = "none");
    void addPointSet(std::map<std::string, T> pointSet);
    
    const std::map<std::string, T>* getPointSetAt( int index);
    bool removePointSetAt(int index);
    const size_t getNumberOfPointSets() const;
    void clear();
    
};

#pragma mark implementations

template <typename T>
inline sdTypedTrajectory<T>::sdTypedTrajectory(std::string tp):sdTrajectory(tp){};

template <typename T>
inline void sdTypedTrajectory<T>::addPointSet(std::map<std::string, T> pointSet){
    pointArray.push_back(pointSet);
}

template <typename T>
inline const std::map<std::string, T>* sdTypedTrajectory<T>::getPointSetAt(int index){
    
    if(pointArray.size() <= index)
        return nullptr;
    
    return &pointArray[index];
}

template <typename T>
inline bool sdTypedTrajectory<T>::removePointSetAt(int index){
    if(pointArray.size() <= index)
        return false;
    
    pointArray.erase(pointArray.begin()+index);
    return true;

}

template <typename T>
inline void sdTypedTrajectory<T>::clear(){
    pointArray.clear();
}

template <typename T>
inline const size_t sdTypedTrajectory<T>::getNumberOfPointSets() const{
    return pointArray.size();
}


#endif
