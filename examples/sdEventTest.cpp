
#include "sdScene.h"
#include <string>
#include <iostream>

int main(void){
    using namespace std;
    
    sdScene scene;
    sdEntityCore * testEntity = scene.addEntity("testEntity");
    double pos[3] = {0.0, 1.0, 2.0};
    sdEvent* event = testEntity->addEvent(5.0, SD_POSITION, pos);
    scene.dump();
    return 0;
}
