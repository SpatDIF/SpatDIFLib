
#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "sdScene.h"

using namespace std;

TEST_CASE("Test sdEvent add get remove", "[sdEvent]"){
    
    sdScene scene;
    sdEntity * entity = scene.addEntity("MyEntity");
    entity->addEvent<SD_PRESENT>(0.0, false);
    REQUIRE(entity->getNumberOfEvents() == 1);
    
    entity->addEvent<SD_MEDIA_GAIN>(1.234, 0.8);
    auto * event =  entity->getEvent<SD_MEDIA_GAIN>(1.234);
    REQUIRE(event);
    REQUIRE(event->getValue() == 0.8);
    
    event =  entity->getEvent<SD_MEDIA_GAIN>(1.23400001); // a little bit different
    REQUIRE(!event);
    
    entity->removeEvent(1.234, SD_MEDIA_GAIN);
    REQUIRE(entity->getNumberOfEvents() == 1);
    
    
    entity->removeAllEvents();
    REQUIRE(entity->getNumberOfEvents() == 0);

}

TEST_CASE("Test all descriptors", "[sdEvent]"){
    
}