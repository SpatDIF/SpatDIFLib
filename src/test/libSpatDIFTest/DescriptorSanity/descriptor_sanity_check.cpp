
#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "sdMain.h"
#include <array>
using namespace std;

TEST_CASE("Test all descriptors types"){
    sdScene scene;
    scene.addExtension(EExtension::SD_MEDIA);
    scene.addExtension(EExtension::SD_SOURCE_SPREAD);
    sdEntity * entity = scene.addEntity("MyEntity");
    
    ///// CORE
    
    // SD_TYPE
    entity->addEvent<SD_TYPE>(0.0, EType::SD_LISTENER);
    REQUIRE(entity->getValueAsString<SD_TYPE>(0.0) == "listener");
    REQUIRE(*(entity->getValue<SD_TYPE>(0.0)) == EType::SD_LISTENER);

    // SD_PRESENT
    entity->addEvent<SD_PRESENT>(0.0, false);
    REQUIRE(entity->getValueAsString<SD_PRESENT>(0.0) == "false");
    REQUIRE(*(entity->getValue<SD_PRESENT>(0.0)) == false);
    
    // SD_POSITION
    entity->addEvent<SD_POSITION>(0.0, {0.2,0.3,0.4});
    REQUIRE( entity->getValueAsString<SD_POSITION>(0.0) == "0.2 0.3 0.4");
    REQUIRE( entity->getValue<SD_POSITION>(0.0)->at(0) == 0.2);
    REQUIRE( entity->getValue<SD_POSITION>(0.0)->at(1) == 0.3);
    REQUIRE( entity->getValue<SD_POSITION>(0.0)->at(2) == 0.4);
    
    // SD_ORIENTATION
    entity->addEvent<SD_ORIENTATION>(0.0, {0.5,0.6,0.7});
    REQUIRE( entity->getValueAsString<SD_ORIENTATION>(0.0) == "0.5 0.6 0.7");
    REQUIRE( entity->getValue<SD_ORIENTATION>(0.0)->at(0) == 0.5);
    REQUIRE( entity->getValue<SD_ORIENTATION>(0.0)->at(1) == 0.6);
    REQUIRE( entity->getValue<SD_ORIENTATION>(0.0)->at(2) == 0.7);
    

    
    
    
    
    // SD_MEDIA_ID
    entity->addEvent<SD_MEDIA_ID>(0.0, "mymedia");
    REQUIRE( entity->getValueAsString<SD_MEDIA_ID>(0.0) == "mymedia");
    REQUIRE( *entity->getValue<SD_MEDIA_ID>(0.0) == "mymedia");
    
    // SD_MEDIA_TYPE
    entity->addEvent<SD_MEDIA_TYPE>(0.0, "live" );
    REQUIRE( entity->getValueAsString<SD_MEDIA_TYPE>(0.0) == "live");
    REQUIRE( *entity->getValue<SD_MEDIA_TYPE>(0.0) == "live");
    
    // SD_MEDIA_LOCATION
    entity->addEvent<SD_MEDIA_LOCATION>(0.0, "/path/to/my/file");
    REQUIRE( entity->getValueAsString<SD_MEDIA_LOCATION>(0.0) == "/path/to/my/file");
    REQUIRE( *entity->getValue<SD_MEDIA_LOCATION>(0.0) == "/path/to/my/file");
    
    // SD_MEDIA_CHANNEL
    entity->addEvent<SD_MEDIA_CHANNEL>(0.0, 2);
    REQUIRE( entity->getValueAsString<SD_MEDIA_CHANNEL>(0.0) == "2");
    REQUIRE( *entity->getValue<SD_MEDIA_CHANNEL>(0.0) == 2);
    
    // SD_MEDIA_TIME_OFFSET
    entity->addEvent<SD_MEDIA_TIME_OFFSET>(0.0, 20.2);
    REQUIRE( entity->getValueAsString<SD_MEDIA_TIME_OFFSET>(0.0) == "20.2");
    REQUIRE( *entity->getValue<SD_MEDIA_TIME_OFFSET>(0.0) == 20.2);
    
    // SD_MEDIA_GAIN
    entity->addEvent<SD_MEDIA_GAIN>(0.0, 0.4);
    REQUIRE( entity->getValueAsString<SD_MEDIA_GAIN>(0.0) == "0.4");
    REQUIRE( *entity->getValue<SD_MEDIA_GAIN>(0.0) == 0.4);
    
    // SD_SOURCE_SPREAD_SPREAD
    entity->addEvent<SD_SOURCE_SPREAD_SPREAD>(0.0, 0.5);
    REQUIRE( entity->getValueAsString<SD_SOURCE_SPREAD_SPREAD>(0.0) == "0.5");
    REQUIRE( *entity->getValue<SD_SOURCE_SPREAD_SPREAD>(0.0) == 0.5);
    
    // SD_HARDWARE_OUT_PHYSICAL_CHANNEL
    entity->addEvent<SD_HARDWARE_OUT_PHYSICAL_CHANNEL>(0.5,3);
    REQUIRE(entity->getValue<SD_HARDWARE_OUT_PHYSICAL_CHANNEL>(0.5) == nullptr);
    
    scene.addExtension(EExtension::SD_HARDWARE_OUT);
    entity->addEvent<SD_HARDWARE_OUT_PHYSICAL_CHANNEL>(0.5,3);
    REQUIRE(*entity->getValue<SD_HARDWARE_OUT_PHYSICAL_CHANNEL>(0.5) == 3);
    
    
}
