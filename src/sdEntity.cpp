//
//  sdEntity.cpp
//  

#include <string>
#include <iostream>
#include <set>
#include "sdEntity.h"

sdEntity::sdEntity(){
}

sdEntity::~sdEntity(){
}

unsigned int sdEntity::getNumberOfEvents(){
    return eventSet.size();
}



void sdEntity::removeAllEvents(){
    eventSet.clear();
}

