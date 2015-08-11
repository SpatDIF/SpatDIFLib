//
//  sdTrajectoryHandler.h
//  libspatdif
//
//  Created by Chikashi Miyama on 11/08/15.
//  Copyright (c) 2015 org.spatdif. All rights reserved.
//

#ifndef libspatdif_sdTrajectoryHandler_h
#define libspatdif_sdTrajectoryHandler_h

#include "sdTrajectory.h"

class sdTrajectoryHandler{
    
protected:
    /*! a map of trajectories */
    std::map <std::string, sdTrajectory*> trajectoryMap;
    
public:
    /*! @name Trajectory handling
     @{
     */
    
    /**
     * @brief add trajectory and store it in the map.
     * @return a pointer to the newly instantiated trajectory
     * @detail this is the only function that can instantiate sdTrajectory.
     */
    
    sdTrajectory* addTrajectory(std::string name);
    
    /**
     * @brief retrieve trjectory by name
     * @return if correnspoinding trjectory exists, returns a pointer to the trajectory. if not nullptr.
     */
    
    sdTrajectory* getTrajectory(std::string name);
    
    /**
     * @brief modify existing trajectory. if the corresponding trjectory does not exist, does nothing.
     * @param name name for the trajectory
     * @param trjectory pointer to an instance of sdTrajectory
     */
    void setTrajectory(std::string name, sdTrajectory *trajectory);
    
    /**
     * @brief get number of registered trajectories
     */
    size_t getNumberOfTrajectories(void);
    
    /**
     * @brief remove specified trjaectory
     */
    void removeTrajectory(std::string name);
    
    /**
     * @brief discard all registerd trajectories
     */
    void removeAllTrajectories();
    /*!
     @}
     */

};

sdTrajectory* sdTrajectoryHandler::addTrajectory(std::string name){
    if(trajectoryMap.find(name) != trajectoryMap.end()){
        // trajectory exists already
        return trajectoryMap[name];
    }
    return nullptr;
}

inline sdTrajectory* sdTrajectoryHandler::getTrajectory(std::string name){
    if(trajectoryMap.find(name) != trajectoryMap.end()){
        return trajectoryMap[name];
    }
    return nullptr;
}

inline void sdTrajectoryHandler::setTrajectory(std::string name, sdTrajectory *trajectory){
    if(getTrajectory(name)){
        trajectoryMap[name] = trajectory;
    }
}

inline size_t sdTrajectoryHandler::getNumberOfTrajectories(void){
    return trajectoryMap.size();
}

inline void sdTrajectoryHandler::removeTrajectory(std::string name){
    std::map<std::string, sdTrajectory*>::iterator it;
    it = trajectoryMap.find(name);
    if(it != trajectoryMap.end()){
        trajectoryMap.erase(it);
    }
}


inline void sdTrajectoryHandler::removeAllTrajectories(void){
    trajectoryMap.clear();
}

#endif
