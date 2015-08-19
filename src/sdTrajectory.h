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

class sdTrajectory {
    friend class sdScene; // only sdScene can instantiate this class. this guarantees that all trjaectory has affiliations.
    
private:
    std::vector<sdPoint> pointArray;
    std::string type;
    
public:
    sdTrajectory(std::string tp = "none");
    ~sdTrajectory();
    
    void addPoint(sdPoint point);
    void addPoint(double x, double y, double z, std::string tp = "none");
                  
    
    const std::string & getType();
    const sdPoint* getPointAt( int index);
    bool removePointAt(int index);
    const size_t getNumberOfPoints() const;
    void clear();
    
};

#pragma mark implementations
inline sdTrajectory::sdTrajectory(std::string tp):type(tp){};
inline sdTrajectory::~sdTrajectory(){};

inline void sdTrajectory::addPoint(sdPoint point){
    pointArray.push_back(point);
}

inline void sdTrajectory::addPoint(double x, double y, double z, std::string tp){
    addPoint(sdPoint(x,y,z,tp));
}


inline const std::string & sdTrajectory::getType(){
    return type;
}

inline const sdPoint* sdTrajectory::getPointAt(int index){
    
    if(pointArray.size() <= index)
        return nullptr;
    
    return &pointArray[index];
}

inline bool sdTrajectory::removePointAt(int index){
    if(pointArray.size() <= index)
        return false;
    
    pointArray.erase(pointArray.begin()+index);
    return true;

}

inline void sdTrajectory::clear(){
    pointArray.clear();
}

inline const size_t sdTrajectory::getNumberOfPoints() const{
    return pointArray.size();
}


#endif
