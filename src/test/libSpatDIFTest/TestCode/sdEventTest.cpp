
#define CATCH_CONFIG_MAIN
#include <array>
#include "catch.hpp"
#include "sdScene.h"

using namespace std;

TEST_CASE("Test sdEvent add get remove"){
    
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

TEST_CASE("Test exceptions", "[sdEvent]"){
    sdScene scene;
    sdEntity * entity = scene.addEntity("MyEntity");
    
    try {
        entity->addEvent<SD_PRESENT>(-10.0, false); // invalid time, exception thrown and returns nullptr
    } catch (const InvalidTimeException &exception) {
        std::cerr << exception.what() << std::endl;
    }
    
   
}

TEST_CASE("Test all descriptors types", "[sdEvent]"){
    sdScene scene;
    sdEntity * entity = scene.addEntity("MyEntity");
    entity->addEvent<SD_PRESENT>(0.0, false);
}

TEST_CASE("getFirstEvent() and getLastEvent()"){
    
    sdScene scene;
    sdEntity * entity = scene.addEntity("MyEntity");

    // no events in the entity, should return nullptr
    REQUIRE(!entity->getFirstEvent<SD_POSITION>());
    REQUIRE(!entity->getLastEvent<SD_POSITION>());

    // three events added
    auto a = entity->addEvent<SD_POSITION>(0.4, {0.5, 0.2, 0.3}); // chronologically third
    auto b = entity->addEvent<SD_POSITION>(0.2, {0.3, 0.1, 0.2}); // chronologically first
    auto c = entity->addEvent<SD_POSITION>(0.3, {0.1, 0.4, 0.6}); // chronologically second

    // looking for non-exisiting event
    REQUIRE(!entity->getFirstEvent<SD_PRESENT>());
    REQUIRE(!entity->getLastEvent<SD_PRESENT>());

    auto firstEvent = entity->getFirstEvent<SD_POSITION>();
    auto lastEvent = entity->getLastEvent<SD_POSITION>();
    REQUIRE(firstEvent == b.get());
    REQUIRE(lastEvent == a.get());
}

TEST_CASE("getFirstEvents() and getLastEvents()"){

    sdScene scene;
    sdEntity * entity = scene.addEntity("MyEntity");

    REQUIRE(entity->getFirstEventSet().empty());
    REQUIRE(entity->getLastEventSet().empty());

    // three events added
    auto a = entity->addEvent<SD_POSITION>(0.4, {0.5, 0.2, 0.3}); // chronologically third
    auto b = entity->addEvent<SD_POSITION>(0.2, {0.3, 0.1, 0.2}); // chronologically first
    auto c = entity->addEvent<SD_POSITION>(0.3, {0.1, 0.4, 0.6}); // chronologically second
    ;
    REQUIRE(entity->getFirstEventSet().size() == 1);
    REQUIRE(entity->getLastEventSet().size() == 1);

    // event added at first and last
    auto d = entity->addEvent<SD_PRESENT>(0.2, true);
    auto e = entity->addEvent<SD_PRESENT>(0.4, true);
    REQUIRE(entity->getFirstEventSet().size() == 2);
    REQUIRE(entity->getLastEventSet().size() == 2);

    // two event removed
    entity->removeEvent(d);
    entity->removeEvent(e);
    REQUIRE(entity->getFirstEventSet().size() == 1);
    REQUIRE(entity->getLastEventSet().size() == 1);

}

TEST_CASE("getFirstEventTime() getLastEventTime()"){
    sdScene scene;
    sdEntity * entity = scene.addEntity("MyEntity");
    
    // no events in the entity, so the set should be {0.0, false}
    REQUIRE(!entity->getFirstEventTime().second);
    REQUIRE(!entity->getLastEventTime().second);
    
    // three events adde
    auto a = entity->addEvent<SD_POSITION>(0.4, {0.5, 0.2, 0.3}); // chronologically third
    auto b = entity->addEvent<SD_POSITION>(0.2, {0.3, 0.1, 0.2}); // chronologically first
    auto c = entity->addEvent<SD_POSITION>(0.3, {0.1, 0.4, 0.6}); // chronologically second

    REQUIRE(entity->getFirstEventTime().first == b->getTime());
    REQUIRE(entity->getLastEventTime().first == a->getTime());
    entity->removeEvent(b);
    entity->removeEvent(a);
    REQUIRE(entity->getFirstEventTime().first == c->getTime());
    REQUIRE(entity->getLastEventTime().first == c->getTime());
    
}

TEST_CASE("getValue()"){
    
    sdScene scene;
    sdEntity * entity = scene.addEntity("MyEntity");
    std::array<double, 3> array = {0.5, 0.2, 0.3};
    auto a = entity->addEvent<SD_POSITION>(0.4, array); // chronologically third
    auto value = entity->getValue<SD_POSITION>(0.5); // cannot get value
    REQUIRE(!value);
    value = entity->getValue<SD_POSITION>(0.4);
    REQUIRE(value);
    REQUIRE(array == *value);
    
    
}