
#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "sdScene.h"

using namespace std;

TEST_CASE("Test sdEvent add get remove", "[sdEvent]"){
    
    sdScene scene;
    sdEntity * entity = scene.addEntity("MyEntity");
    entity->addEvent<SD_PRESENT>(0.0, false); // 1
    REQUIRE(entity->getNumberOfEvents() == 1);
    
    auto addedEvent = entity->addEvent<SD_MEDIA_GAIN>(1.234, 0.8); // 2
    auto gotEvent =  entity->getEvent<SD_MEDIA_GAIN>(1.234);
    REQUIRE(gotEvent);
    REQUIRE(gotEvent == addedEvent.get());
    REQUIRE(gotEvent->getValue() == 0.8);
    
    auto event =  entity->getEvent<SD_MEDIA_GAIN>(1.23400001); // a little bit different
    REQUIRE(!event );

    // overriding
    auto overridEvent = entity->addEvent<SD_PRESENT>(0.0, true); // 3 but override
    REQUIRE(entity->getNumberOfEvents() == 2);
    
    entity->removeEvent(1.234, SD_MEDIA_GAIN);
    REQUIRE(entity->getNumberOfEvents() == 1);
    
    
    entity->removeAllEvents();
    REQUIRE(entity->getNumberOfEvents() == 0);

}

TEST_CASE("Test all descriptors", "[sdEvent]"){
    
}

TEST_CASE("Test chronological sorting", "[sdEvent]"){
    
    sdScene scene;
    sdEntity * entity = scene.addEntity("MyEntity");

    auto a = entity->addEvent<SD_POSITION>(0.4, {0.5, 0.2, 0.3}); // chronologically third
    auto b = entity->addEvent<SD_POSITION>(0.2, {0.3, 0.1, 0.2}); // chronologically first
    auto c = entity->addEvent<SD_POSITION>(0.3, {0.1, 0.4, 0.6}); // chronologically second

    // the order should be b, c, and a
    auto firstEvent = entity->getFirstEvent(SD_POSITION);
    REQUIRE(firstEvent == b);
    
    auto d = entity->addEvent<SD_PRESENT>(0.2, true);
    auto firstEvents = entity->getFirstEventSet();
    REQUIRE(firstEvents.size() == 2);
    
}