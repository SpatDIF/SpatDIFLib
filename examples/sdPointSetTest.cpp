/** @file
 *
 * @ingroup spatdiflib-examples
 *
 * @brief Example: Test #sdPointSet
 * @details
 *
 * @authors Chikashi Miyama, Trond Lossius
 *
 * @copyright Copyright (C) 2013 - 2014 Zurich University of the Arts, Institute for Computer Music and Sound Technology @n
 * This code is licensed under the terms of the "New BSD License" @n
 * http://creativecommons.org/licenses/BSD/
 */

#include "sdPointSet.h"
#include <iostream>

void dumpPointSet(sdPointSet &pSet) {
    int size = pSet.numberOfPoints();
    
    std::cout << "======" << std::endl;
    std::cout<< "point set type: " << pSet.getType() << std::endl;
    std::cout << "number of points:" << size << std::endl;
    for(int i = 0; i < size ; i++){
        sdPoint p(3,4,5,"aaa");
        if(pSet.getPointAt(p, i)){
            std::cout << "index:" << i << " x:" << p.x << " y:" << p.y << " z:" << p.z << " type:" << p.type << std::endl;
        }
    }
    std::cout << "======" << std::endl;
}


int main(void){

    // check type
    sdPointSet pointSet("bezier");
    
    // add point in two different ways
    
    sdPoint p = sdPoint(1, 1, 0, "anchor");
    pointSet.addPoint(p);
    pointSet.addPoint(-1, -1, 0, "anchor");
    pointSet.addPoint(0, 0, 0, "anchor");
    dumpPointSet(pointSet);
    
    
    // remove one point
    
    std::cout << "remove point at index : 2" << std::endl;
    if(!pointSet.removePointAt(2)){
        std::cout << "deletion unsuccessful" << std::endl;
    }
    dumpPointSet(pointSet);
    
    
    // clear all point set
    std::cout << "clear all points" << std::endl;
    pointSet.clear();
    dumpPointSet(pointSet);
    

    return 0;
}