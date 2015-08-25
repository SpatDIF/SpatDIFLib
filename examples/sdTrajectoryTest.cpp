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
    std::string type = traj->getType();
    std::cout << "type of trajectory:" << type << std::endl;
    
    if(!type.compare("bezier")){
        sdTypedTrajectory<sdPoint2D> * trajectory = static_cast<sdTypedTrajectory<sdPoint2D> * >(traj);
        std::cout << "number of point sets:" << trajectory->getNumberOfPointSets() << std::endl;
        
        for(int i = 0; i < trajectory->getNumberOfPointSets(); i++){
            const std::map<std::string, sdPoint2D> * ps = trajectory->getPointSetAt(i);
            sdPoint2D p = ps->at("anchor");
            std::cout <<"anchor " << "x:" << p.x << "  y:" << p.y << std::endl;
            p = ps->at("forwardHandle");
            std::cout <<"forwardHandle " << "x:" << p.x << "  y:" << p.y << std::endl;
            
            p = ps->at("backwardHandle");
            std::cout <<"backwardHandle " << "x:" << p.x << "  y:" << p.y << std::endl;
        }
    }else if(!type.compare("exponential"))
    {
        sdTypedTrajectory<sdPoint2DCurve> * trajectory = static_cast<sdTypedTrajectory<sdPoint2DCurve> * >(traj);

        for(int i = 0; i < trajectory->getNumberOfPointSets(); i++){
            const std::map<std::string, sdPoint2DCurve> * ps = trajectory->getPointSetAt(i);
            sdPoint2DCurve p = ps->at("node");
            std::cout << "x:" << p.x << "  y:" << p.y  <<" cf: " << p.curveFactor << std::endl;
        }
    }
    
}

int main(void){

    sdScene scene;
    scene.addTrajectory<sdPoint2D>("bezierCurve", "bezier");
    scene.addTrajectory<sdPoint2DCurve>("exponentialCurve", "exponential");

    size_t numTraj = scene.getNumberOfTrajectories();
    std::cout << "number of trajectories:" << numTraj << std::endl;

    sdTrajectory* undefined = scene.getTrajectory("undefined");
    if(!undefined){
        std::cout << "not found. trajectory undefined" << std::endl;
    }
    
    sdTypedTrajectory<sdPoint2D> * bezierCurve = static_cast<sdTypedTrajectory<sdPoint2D> *>(scene.getTrajectory("bezierCurve"));
    sdTypedTrajectory<sdPoint2DCurve> * exponentialCurve = static_cast<sdTypedTrajectory<sdPoint2DCurve> *>(scene.getTrajectory("exponentialCurve"));

    // add two bezier points
    {
        std::map<std::string, sdPoint2D> pointSet;
        pointSet["anchor"] = sdPoint2D(0 ,1);
        pointSet["forwardHandle"] = sdPoint2D(1 ,1);
        pointSet["backwardHandle"] = sdPoint2D(-1 ,-1);
        bezierCurve->addPointSet(pointSet);
    }
    {
        std::map<std::string, sdPoint2D> pointSet;
        pointSet["anchor"] = sdPoint2D(5 ,5);
        pointSet["forwardHandle"] = sdPoint2D(6 ,6);
        pointSet["backwardHandle"] = sdPoint2D(-2 ,-3);
        bezierCurve->addPointSet(pointSet);
    }
    
    // add two exponential points
    {
        std::map<std::string, sdPoint2DCurve > pointSet;
        pointSet["node"] = sdPoint2DCurve(0 ,0, 2);
        exponentialCurve->addPointSet(pointSet);
    }
    {
        std::map<std::string, sdPoint2DCurve > pointSet;
        pointSet["node"] = sdPoint2DCurve(1 ,1, 4);
        exponentialCurve->addPointSet(pointSet);
    }
    
    std::cout << "------current content" << std::endl;
    dumpTrajectory(bezierCurve);
    
    std::cout << "------trajectory as XML" << std::endl;
    std::string xmlString = sdSaver::XMLFromScene(&scene);
    std::cout << xmlString << std::endl;

    // remove
    std::cout << "------one point removed" << std::endl;
    bezierCurve->removePointSetAt(1);
    dumpTrajectory(bezierCurve);
    
    std::cout << "------clear all points" << std::endl;
    bezierCurve->clear();
    dumpTrajectory(bezierCurve);

    return 0;
}

