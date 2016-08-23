
#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "sdMain.h"
#include <array>
using namespace std;

TEST_CASE("Test all descriptors types"){
    sdScene scene;
    scene.addExtension(EExtension::SD_SOURCE_SPREAD);
    sdEntity * entity = scene.addEntity("MyEntity");
    
    /*** core ***/
    /// 4.3 core
    
    // SD_TYPE
    entity->addEvent<SD_TYPE>(0.0, sdDescriptor<SD_TYPE>::EType::SD_LISTENER);
    REQUIRE(entity->getValueAsString<SD_TYPE>(0.0) == "listener");
    REQUIRE(*(entity->getValue<SD_TYPE>(0.0)) == sdDescriptor<SD_TYPE>::SD_LISTENER);

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
    
    /** core functionalities **/
    /// 4.4.1 media

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
    
    /// 4.4.2 loop
    
    // SD_LOOP_TYPE
    entity->addEvent<SD_LOOP_TYPE>(0.0, std::make_pair(sdDescriptor<SD_LOOP_TYPE>::SD_REPEAT, 3));
    REQUIRE( entity->getValueAsString<SD_LOOP_TYPE>(0.0) == "repeat 3");
    REQUIRE( entity->getValue<SD_LOOP_TYPE>(0.0)->first == sdDescriptor<SD_LOOP_TYPE>::SD_REPEAT);
    REQUIRE( entity->getValue<SD_LOOP_TYPE>(0.0)->second == 3);
            
    // SD_LOOP_POINTS
    entity->addEvent<SD_LOOP_POINTS>(0.0, {3,5});
    REQUIRE( entity->getValueAsString<SD_LOOP_POINTS>(0.0) == "3 5");
    REQUIRE( entity->getValue<SD_LOOP_POINTS>(0.0)->at(0) == 3);
    REQUIRE( entity->getValue<SD_LOOP_POINTS>(0.0)->at(1) == 5);
    
    // SD_LOOP_WAIT_TIME
    entity->addEvent<SD_LOOP_WAIT_TIME>(0.0, 1.0);
    REQUIRE( entity->getValueAsString<SD_LOOP_WAIT_TIME>(0.0) == "1");
    REQUIRE( *entity->getValue<SD_LOOP_WAIT_TIME>(0.0) == 1);

    /// 4.4.3 interpolation
    entity->addEvent<SD_INTERPOLATION_TYPE>(0.0, sdDescriptor<SD_INTERPOLATION_TYPE>::SD_NONE);
    REQUIRE( entity->getValueAsString<SD_INTERPOLATION_TYPE>(0.0) == "none");
    REQUIRE( *entity->getValue<SD_INTERPOLATION_TYPE>(0.0) == sdDescriptor<SD_INTERPOLATION_TYPE>::SD_NONE);
}


TEST_CASE("source spread"){
    sdScene scene;
    scene.addExtension(EExtension::SD_SOURCE_SPREAD);
    
    auto spreadSource = scene.addEntity("spread");
    spreadSource->addEvent<SD_SOURCE_SPREAD_SPREAD>(1.0, 50.0);
    REQUIRE(spreadSource->getValueAsString<SD_SOURCE_SPREAD_SPREAD>(1.0) == "50" );
    REQUIRE(*spreadSource->getValue<SD_SOURCE_SPREAD_SPREAD>(1.0) == 50.0);
    
}

TEST_CASE("distance cues"){
    sdScene scene;
    scene.addExtension(EExtension::SD_DISTANCE_CUES);
    
    auto distantSource = scene.addEntity("distance");
    distantSource->addMeta<SD_DISTANCE_CUES_MAXIMUM_DISTANCE>(10.0);
    distantSource->addMeta<SD_DISTANCE_CUES_REFERENCE_DISTANCE>(5.0);
    distantSource->addMeta<SD_DISTANCE_CUES_MAXIMUM_ATTENUATION>(5.0);
    distantSource->addMeta<SD_DISTANCE_CUES_ATTENUATION_MODEL>(1);
    distantSource->addMeta<SD_DISTANCE_CUES_ABSORPTION_MODEL>(1);

    distantSource->addEvent("3.0", "distance-cues", "reference-distance", "2.0");
    distantSource->addEvent("3.0", "distance-cues", "maximum-distance", "2.0");
    distantSource->addEvent("3.0", "distance-cues", "maximum-attenuation", "2.0");
    distantSource->addEvent("3.0", "distance-cues", "attenuation-model", "1");
    distantSource->addEvent("3.0", "distance-cues", "absorption-model", "1");

    REQUIRE(*distantSource->getValue<SD_DISTANCE_CUES_MAXIMUM_DISTANCE>() == 10.0);
    REQUIRE(*distantSource->getValue<SD_DISTANCE_CUES_REFERENCE_DISTANCE>() == 5.0);
    REQUIRE(*distantSource->getValue<SD_DISTANCE_CUES_MAXIMUM_ATTENUATION>() == 5.0);
    REQUIRE(*distantSource->getValue<SD_DISTANCE_CUES_ATTENUATION_MODEL>() == 1);
    REQUIRE(*distantSource->getValue<SD_DISTANCE_CUES_ABSORPTION_MODEL>() == 1);
    
    REQUIRE(*distantSource->getValue<SD_DISTANCE_CUES_REFERENCE_DISTANCE>(3.0) == 2);
    REQUIRE(*distantSource->getValue<SD_DISTANCE_CUES_MAXIMUM_DISTANCE>(3.0) == 2);
    REQUIRE(*distantSource->getValue<SD_DISTANCE_CUES_MAXIMUM_ATTENUATION>(3.0) == 2);
    REQUIRE(*distantSource->getValue<SD_DISTANCE_CUES_ATTENUATION_MODEL>(3.0) == 1);
    REQUIRE(*distantSource->getValue<SD_DISTANCE_CUES_ABSORPTION_MODEL>(3.0) == 1);
}

TEST_CASE("direct to one sink"){
    sdScene scene;
    scene.addExtension(EExtension::SD_DIRECT_TO_ONE);
    
    auto stickySource = scene.addEntity("sticky");
    stickySource->addMeta<SD_DIRECT_TO_ONE_DIRECT_TO_ONE>(true);
    REQUIRE(stickySource->getValueAsString<SD_DIRECT_TO_ONE_DIRECT_TO_ONE>() == "true");

    stickySource->addEvent("4.0", "direct-to-one", "direct-to-one", "true");
    REQUIRE(*stickySource->getValue<SD_DIRECT_TO_ONE_DIRECT_TO_ONE>(4.0) == true);
}

TEST_CASE("hardware out extension"){
    sdScene scene;
    scene.addExtension(EExtension::SD_SINK_ENTITY);
    scene.addExtension(EExtension::SD_HARDWARE_OUT);
    
    auto leftSpeaker = scene.addEntity("JBL-Left", EKind::SD_SINK);
    auto rightSpeaker = scene.addEntity("JBL-Right", EKind::SD_SINK);

    // physical channel
    leftSpeaker->addMeta<SD_HARDWARE_OUT_PHYSICAL_CHANNEL>(10);
    REQUIRE(leftSpeaker->getValueAsString<SD_HARDWARE_OUT_PHYSICAL_CHANNEL>() == "10");
    REQUIRE(*leftSpeaker->getValue<SD_HARDWARE_OUT_PHYSICAL_CHANNEL>() == 10);
    
    rightSpeaker->addEvent("1.0", "hardware-out", "physical-channel",  "11");
    REQUIRE(rightSpeaker->getValueAsString<SD_HARDWARE_OUT_PHYSICAL_CHANNEL>(1.0) == "11");
    REQUIRE(*rightSpeaker->getValue<SD_HARDWARE_OUT_PHYSICAL_CHANNEL>(1.0) == 11);

    // gain
    leftSpeaker->addMeta<SD_HARDWARE_OUT_GAIN>(0.5);
    REQUIRE(leftSpeaker->getValueAsString<SD_HARDWARE_OUT_GAIN>() == "0.5");
    REQUIRE(*leftSpeaker->getValue<SD_HARDWARE_OUT_GAIN>() == 0.5);

    rightSpeaker->addEvent("1.0", "hardware-out", "gain",  "0.6");
    REQUIRE(rightSpeaker->getValueAsString<SD_HARDWARE_OUT_GAIN>(1.0) == "0.6");
    REQUIRE(*rightSpeaker->getValue<SD_HARDWARE_OUT_GAIN>(1.0) == 0.6);

    // exception
    try{
        leftSpeaker->addEvent<SD_HARDWARE_OUT_PHYSICAL_CHANNEL>(20, 0);
    }catch(InvalidValueDomainException &exception){
        // catch
    }
}
