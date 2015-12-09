
#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "sdScene.h"

using namespace std;

TEST_CASE("Test sdEvent", "[sdEvent]"){
    
    sdScene scene;
    sdEntity * entity = scene.addEntity("MyEntity");
    entity->addEvent<SD_PRESENT>(0.0, false);
    REQUIRE(entity->getNumberOfEvents() == 1);
    entity->removeAllEvents();
    REQUIRE(entity->getNumberOfEvents() == 0);
    
    entity->addEvent<SD_PRESENT>(1.0, true);
    auto * event =  entity->getEvent<SD_PRESENT>(1.0);
    REQUIRE(event);
    REQUIRE(event->getValue() == true);
    
}