/** @file
 *
 * @ingroup spatdiflib-examples
 *
 * @brief Example: Test Source Width extentions
 *
 * @details
 *
 * @authors Chikashi Miyama, Trond Lossius
 *
 * @copyright Copyright (C) 2013 - 2014 Zurich University of the Arts, Institute for Computer Music and Sound Technology @n
 * This code is licensed under the terms of the "New BSD License" @n
 * http://creativecommons.org/licenses/BSD/
 */


#include <string>
#include <set>

#include "sdScene.h"
#include "sdSaver.h"

void dumpTrajectory(sdTrajectory * traj){
    std::cout << "type of trajectory:" << traj->getType() << std::endl;
    std::cout << "number of points:" << traj->getNumberOfPoints() << std::endl;
    for(int i = 0; i < traj->getNumberOfPoints(); i++){
        const sdPoint * p = traj->getPointAt(i);
        if(!p)
            continue;
        std::cout << "x:" << p->x << "  y:" << p->y << "  z: " << p->z <<std::endl;
    }
}

int main(void){

    // check type
    sdScene scene;

    scene.addTrajectory("myFirstTrajectory" , "bezier");
    size_t numTraj = scene.getNumberOfTrajectories();
    std::cout << "number of trajectories:" << numTraj << std::endl;

    sdTrajectory* undefined = scene.getTrajectory("undefined");
    if(!undefined){
        std::cout << "not found. trajectory undefined" << std::endl;
    }
    
    sdTrajectory* myFirstTrajectory = scene.getTrajectory("myFirstTrajectory");


    // triangle
    myFirstTrajectory->addPoint(0, 1, 0 , "anchorPoint");
    myFirstTrajectory->addPoint(-1, 0, 0, "anchorPoint");
    sdPoint lastPoint = sdPoint(1,0,0, "anchorPoint");
    myFirstTrajectory->addPoint(lastPoint);
    
    std::cout << "------current content" << std::endl;
    dumpTrajectory(myFirstTrajectory);
    
    std::cout << "------trajectory as XML" << std::endl;
    std::string xmlString = sdSaver::XMLFromScene(&scene);
    std::cout << xmlString << std::endl;

    // remove
    std::cout << "------one point removed" << std::endl;
    myFirstTrajectory->removePointAt(1);
    dumpTrajectory(myFirstTrajectory);
    
    std::cout << "------clear all points" << std::endl;
    myFirstTrajectory->clear();
    dumpTrajectory(myFirstTrajectory);
    



    return 0;
}

