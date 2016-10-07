
#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "sdMain.h"
#include <array>
using namespace std;

TEST_CASE("core"){
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
}

TEST_CASE("media"){

    /// 4.4.1 media
    sdScene scene;
    
    sdEntity * entity = scene.addEntity("core_function");
    
    // SD_MEDIA_ID
    entity->addEvent<SD_MEDIA_ID>(0.0, "mymedia");
    REQUIRE( entity->getValueAsString<SD_MEDIA_ID>(0.0) == "mymedia");
    REQUIRE( *entity->getValue<SD_MEDIA_ID>(0.0) == "mymedia");
    
    // SD_MEDIA_TYPE
    entity->addEvent<SD_MEDIA_TYPE>(0.0, sdDescriptor<SD_MEDIA_TYPE>::SD_LIVE );
    REQUIRE( entity->getValueAsString<SD_MEDIA_TYPE>(0.0) == "live");
    REQUIRE( *entity->getValue<SD_MEDIA_TYPE>(0.0) == sdDescriptor<SD_MEDIA_TYPE>::SD_LIVE);
    
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
    
    //exception
    try{
        entity->addEvent("9.0", "media", "type", "not defined");
        REQUIRE(false);
    }catch(CanNotConvertStringException){}
    
    try{
        entity->addEvent<SD_MEDIA_CHANNEL>(3.0, 0);
        REQUIRE(false);
    }catch(InvalidValueDomainException){}
    
    try{
        entity->addEvent<SD_MEDIA_TIME_OFFSET>(3.0, -1.0);
        REQUIRE(false);
    }catch(InvalidValueDomainException){}
    
    try{
        entity->addEvent<SD_MEDIA_GAIN>(3.0, -2.0);
        REQUIRE(false);
    }catch(InvalidValueDomainException){}
    
 
}

TEST_CASE("loop"){
    
    /// 4.4.2 loop
    sdScene scene;
    sdEntity * entity = scene.addEntity("looped");
    
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
}

TEST_CASE("interpolation"){
    /// 4.4.3 interpolation
    sdScene scene;
    sdEntity * entity = scene.addEntity("interpolatedEntity");
    entity->addEvent<SD_INTERPOLATION_TYPE>(0.0, sdDescriptor<SD_INTERPOLATION_TYPE>::SD_NONE);
    REQUIRE( entity->getValueAsString<SD_INTERPOLATION_TYPE>(0.0) == "none");
    REQUIRE( *entity->getValue<SD_INTERPOLATION_TYPE>(0.0) == sdDescriptor<SD_INTERPOLATION_TYPE>::SD_NONE);
}

TEST_CASE("pointset"){
    sdScene scene;
    scene.addExtension(EExtension::SD_POINTSET);
    sdDataSet<EExtension::SD_POINTSET> pointset("mypointset");
    pointset.addPoint({0,0,0});
    pointset.addHandle({1,1,1});
    pointset.addHandle({2,2,2});
    pointset.addPoint({3,3,3});
    pointset.addHandle({4,4,4});
    pointset.addHandle({5,5,5});
    pointset.addPoint({6,6,6});

    scene.addDataSet(EExtension::SD_POINTSET, pointset);
    auto ps = scene.getDataSet<EExtension::SD_POINTSET>("mypointset");
    auto vector = ps.getValue<SD_POINTSET_POINT_OR_HANDLE>();
    
}

TEST_CASE("geometry"){
    sdScene scene;
    scene.addExtension(EExtension::SD_GEOMETRY);
    sdEntity * entity = scene.addEntity("geometry");
    entity->addEvent<SD_GEOMETRY_TRANSLATE>(3.0, {0.3,0.4,0.2});
    entity->addEvent<SD_GEOMETRY_ROTATE>(3.0, {1.0, 0.2, 0.4});
    entity->addEvent<SD_GEOMETRY_SCALE>(3.0, {1.0, 0.2, 0.4});
}

TEST_CASE("automation"){
    sdScene scene;
    scene.addExtension(EExtension::SD_AUTOMATION);
    sdEntity * entity = scene.addEntity("automation");
    
    entity->addEvent<SD_AUTOMATION_DURATION>(0.0, 1.0);
    REQUIRE( entity->getValueAsString<SD_AUTOMATION_DURATION>(0.0) == "1");
    REQUIRE( *entity->getValue<SD_AUTOMATION_DURATION>(0.0) == 1);
    
    entity->addEvent<SD_AUTOMATION_DELAY>(0.0, 2.0);
    REQUIRE( entity->getValueAsString<SD_AUTOMATION_DELAY>(0.0) == "2");
    REQUIRE( *entity->getValue<SD_AUTOMATION_DELAY>(0.0) == 2);
    
    entity->addEvent<SD_AUTOMATION_FUNCTION>(3.0, sdDescriptor<SD_AUTOMATION_FUNCTION>::SD_EASE_OUT);
    REQUIRE( entity->getValueAsString<SD_AUTOMATION_FUNCTION>(3.0) == "ease-out");
    REQUIRE( *entity->getValue<SD_AUTOMATION_FUNCTION>(3.0) == sdDescriptor<SD_AUTOMATION_FUNCTION>::SD_EASE_OUT);

    //exception
    try{
        entity->addEvent<SD_AUTOMATION_DURATION>(3.0,-2.0); // negative duration
        REQUIRE(false);
    }catch(InvalidValueDomainException){}
    
    try{
        entity->addEvent<SD_AUTOMATION_DELAY>(3.0,-2.0); // negative delay
        REQUIRE(false);
    }catch(InvalidValueDomainException){}
    
    try{
        entity->addEvent("5.0", "automation", "function", "difficult-in");
        REQUIRE(false);
    }catch(CanNotConvertStringException){}
    
}

TEST_CASE("shape"){
    sdScene scene;
    sdEntity * entity = scene.addEntity("shape");
    
    scene.addExtension(EExtension::SD_SHAPE);
    
    entity->addEvent<SD_SHAPE_DIRECTION>(3.0, false);
    REQUIRE(entity->getValueAsString<SD_SHAPE_DIRECTION>(3.0) == "false" );
    REQUIRE(*entity->getValue<SD_SHAPE_DIRECTION>(3.0) == false);

    entity->addEvent<SD_SHAPE_CLOSED>(3.0, true);
    REQUIRE(entity->getValueAsString<SD_SHAPE_CLOSED>(3.0) == "true" );
    REQUIRE(*entity->getValue<SD_SHAPE_CLOSED>(3.0) == true);
    
    entity->addEvent<SD_SHAPE_TYPE>(3.0, sdDescriptor<SD_SHAPE_TYPE>::SD_CIRCLE);
    REQUIRE(entity->getValueAsString<SD_SHAPE_TYPE>(3.0) == "circle" );
    REQUIRE(*entity->getValue<SD_SHAPE_TYPE>(3.0) == sdDescriptor<SD_SHAPE_TYPE>::SD_CIRCLE);
    
    // exception
    try{
        entity->addEvent("5.0", "shape", "direction", "dummy"); // should be true or false
        REQUIRE(false);
    }catch(CanNotConvertStringException){}

    try{
        entity->addEvent("5.0", "shape", "closed", "dummy"); // should be true or false
        REQUIRE(false);
    }catch(CanNotConvertStringException){}

    try{
        entity->addEvent("5.0", "shape", "type", "oval"); // should be true or false
        REQUIRE(false);
    }catch(CanNotConvertStringException){}
    
   
}

TEST_CASE("group"){
    sdScene scene;
    sdEntity* entity = scene.addEntity("group-member");
    
    // SD_GROUP_MEMBERSHIP
    try{
        entity->addEvent<SD_GROUP_MEMBERSHIP>(0.0, "alpha_group"); // we need to activate extension first
        REQUIRE(false); // this line should not be reached
    }catch(InvalidDescriptorException){}
    
    scene.addExtension(EExtension::SD_GROUP);
    entity->addEvent<SD_GROUP_MEMBERSHIP>(3.0, "alpha_group");
    
    REQUIRE( entity->getValueAsString<SD_GROUP_MEMBERSHIP>(3.0) == "alpha_group");
    REQUIRE( *entity->getValue<SD_GROUP_MEMBERSHIP>(3.0) == "alpha_group");
    
}

TEST_CASE("source spread"){
    sdScene scene;
    scene.addExtension(EExtension::SD_SOURCE_SPREAD);
    auto spreadSource = scene.addEntity("spread");
    spreadSource->addEvent<SD_SOURCE_SPREAD_SPREAD>(1.0, 50.0);
    REQUIRE(spreadSource->getValueAsString<SD_SOURCE_SPREAD_SPREAD>(1.0) == "50" );
    REQUIRE(*spreadSource->getValue<SD_SOURCE_SPREAD_SPREAD>(1.0) == 50.0);
    try{
        spreadSource->addEvent<SD_SOURCE_SPREAD_SPREAD>(3.0,240);
        REQUIRE(false);
    }catch(InvalidValueDomainException){}
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
    
    //exception test
    try{
        distantSource->addMeta<SD_DISTANCE_CUES_MAXIMUM_DISTANCE>(-1); // should be positive
        REQUIRE(false); // this line should be not reached because of the invalid value
    }catch(InvalidValueDomainException &exception){}
    
    try{
        distantSource->addMeta<SD_DISTANCE_CUES_REFERENCE_DISTANCE>(-1); // should be positive
        REQUIRE(false); // this line should be not reached because of the invalid value
    }catch(InvalidValueDomainException &exception){}
    
    try{
        distantSource->addMeta<SD_DISTANCE_CUES_ATTENUATION_MODEL>(3); // should be 0, 1 or 2
        REQUIRE(false); // this line should be not reached because of the invalid value
    }catch(InvalidValueDomainException &exception){}
    
    try{
        distantSource->addMeta<SD_DISTANCE_CUES_ABSORPTION_MODEL>(2); // should be 0 or 1
        REQUIRE(false); // this line should be not reached because of the invalid value
    }catch(InvalidValueDomainException &exception){}
}


TEST_CASE("direct to one sink"){
    sdScene scene;
    scene.addExtension(EExtension::SD_DIRECT_TO_ONE);
    
    auto stickySource = scene.addEntity("sticky");
    stickySource->addMeta<SD_DIRECT_TO_ONE_DIRECT_TO_ONE>(true);
    REQUIRE(stickySource->getValueAsString<SD_DIRECT_TO_ONE_DIRECT_TO_ONE>() == "true");

    stickySource->addEvent("4.0", "direct-to-one", "direct-to-one", "true");
    REQUIRE(*stickySource->getValue<SD_DIRECT_TO_ONE_DIRECT_TO_ONE>(4.0) == true);
    
    //exception test
    try{
        stickySource->addEvent("5.0", "direct-to-one", "direct-to-one", "invalid input");
        REQUIRE(false); // this line should be not reached because of the invalid value
    }catch(CanNotConvertStringException &exception){}
    
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

    // exception test
    try{
        leftSpeaker->addEvent<SD_HARDWARE_OUT_PHYSICAL_CHANNEL>(20, 0);
        REQUIRE(false); // this line should be not reached because of the invalid value
    }catch(InvalidValueDomainException &exception){}

    // gain
    leftSpeaker->addMeta<SD_HARDWARE_OUT_GAIN>(0.5);
    REQUIRE(leftSpeaker->getValueAsString<SD_HARDWARE_OUT_GAIN>() == "0.5");
    REQUIRE(*leftSpeaker->getValue<SD_HARDWARE_OUT_GAIN>() == 0.5);

    rightSpeaker->addEvent("1.0", "hardware-out", "gain",  "0.6");
    REQUIRE(rightSpeaker->getValueAsString<SD_HARDWARE_OUT_GAIN>(1.0) == "0.6");
    REQUIRE(*rightSpeaker->getValue<SD_HARDWARE_OUT_GAIN>(1.0) == 0.6);
}


TEST_CASE("Dataset"){

    sdScene scene;
    sdInfo info("Max Musterman", "testapp", "2016-9-12",  "testses", "chengdu", "this is test", "dataset test", 22);
    scene.setInfo(info);
    scene.addExtension(EExtension::SD_POINTSET);
    
    sdDataSet<EExtension::SD_MEDIA> mediadata1 = sdDataSet<EExtension::SD_MEDIA>(
                                                                            "mymedia",sdDescriptor<SD_MEDIA_TYPE>::SD_FILE,"/tmp/test.wav", 1, 0, 1.0 );
    sdDataSet<EExtension::SD_MEDIA> mediadata2 = sdDataSet<EExtension::SD_MEDIA>(
                                                                                 "yourmedia", sdDescriptor<SD_MEDIA_TYPE>::SD_FILE,
                                                                                 "example.wav", 4, 0, 0.5);

    sdDataSet<EExtension::SD_POINTSET> pointset("face", false, {{SD_POINTSET_POINT, {0,1,2}}});
    scene.addDataSet(EExtension::SD_MEDIA, mediadata1);
    scene.addDataSet(EExtension::SD_MEDIA, mediadata2);
    scene.addDataSet(EExtension::SD_POINTSET, pointset);
    
    auto dataset1 = scene.getDataSet<EExtension::SD_MEDIA>("mymedia");
    REQUIRE(dataset1.getValue<SD_MEDIA_TYPE>() == sdDescriptor<SD_MEDIA_TYPE>::SD_FILE);
    REQUIRE(dataset1.getValue<SD_MEDIA_ID>() == "mymedia");
    REQUIRE(dataset1.getValue<SD_MEDIA_LOCATION>() == "/tmp/test.wav");
    REQUIRE(dataset1.getValue<SD_MEDIA_CHANNEL>() == 1);
    REQUIRE(dataset1.getValue<SD_MEDIA_GAIN>() == 1.0);
    REQUIRE(dataset1.getValue<SD_MEDIA_TIME_OFFSET>() == 0);
    
    scene.setData("mymedia", SD_MEDIA_CHANNEL, "3");
    REQUIRE(scene.getDataSet<EExtension::SD_MEDIA>("mymedia").getValue<SD_MEDIA_CHANNEL>() == 3);
    
}

