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

#ifndef ____sdPointSet__
#define ____sdPointSet__

#include <string>
#include <vector>

class sdPoint{
public:
    sdPoint(double x = 0.0, double y = 0.0, double z = 0.0, std::string tp = "none");
    ~sdPoint();
    
    double x;
    double y;
    double z;
    std::string type;
};

inline sdPoint::sdPoint(double x, double y, double z, std::string tp):x(x),y(y),z(z),type(tp){};
inline sdPoint::~sdPoint(){}

class sdPointSet{
private:
    std::vector<sdPoint> pointArray;
    std::string type;
    
public:
    sdPointSet(std::string tp = "none");
    ~sdPointSet();
    
    void addPoint(sdPoint point);
    void addPoint(double x, double y, double z, std::string tp);
                  
    
    std::string getType();
    bool getPointAt(sdPoint &p, int index);
    bool removePointAt(int index);
    const int numberOfPoints() const;
    void clear();
    
};

#pragma mark implementations
inline sdPointSet::sdPointSet(std::string tp):type(tp){};
inline sdPointSet::~sdPointSet(){};

inline void sdPointSet::addPoint(sdPoint point){
    pointArray.push_back(point);
}

inline void sdPointSet::addPoint(double x, double y, double z, std::string tp){
    addPoint(sdPoint(x,y,z,tp));
}


inline std::string sdPointSet::getType(){
    return type;
}

inline bool sdPointSet::getPointAt(sdPoint &p, int index){
    
    if(pointArray.size() <= index)
        return false;
    
    p = pointArray[index];
    return true;
}

inline bool sdPointSet::removePointAt(int index){
    if(pointArray.size() <= index)
        return false;
    
    pointArray.erase(pointArray.begin()+index);
    return true;

}

inline void sdPointSet::clear(){
    pointArray.clear();
}

inline const int sdPointSet::numberOfPoints() const{
    return pointArray.size();
}


#endif
