
#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "sdScene.h"
#include <iostream>

using namespace std;

TEST_CASE("Test sdScene", "[sdScene]"){
    sdScene scene;
    SECTION("entity control"){
        
        // add-remove
        scene.addEntity("first");
        REQUIRE(scene.addEntity("first") == nullptr); // should fail, due to duplication
        REQUIRE(scene.getNumberOfEntities() == 1);
        REQUIRE(scene.removeEntity("not-registered") != true); // should fail due to non registered entity
        REQUIRE(scene.removeEntity("first") == true); // should success
        REQUIRE(scene.getNumberOfEntities() == 0);
        
        // all clear
        scene.addEntity("first");
        scene.addEntity("second");
        scene.addEntity("third");
        REQUIRE(scene.getNumberOfEntities() == 3);
        scene.removeAllEntities();
        REQUIRE(scene.getNumberOfEntities() == 0);
        
        // get keys
        scene.addEntity("first");
        scene.addEntity("second");
        auto nameVector = scene.getEntityNames();
        REQUIRE(nameVector[0] == "first");
        REQUIRE(nameVector[1] == "second");

        // get entity by key
        REQUIRE(scene.getEntity("first"));
        REQUIRE(!scene.getEntity("dummy")); // should fail because no entity with key "dummy" was registered
        
    }
    
    SECTION("ordering"){
        
        REQUIRE(scene.getOrdering() == EOrdering::SD_TIME); // defualt should be time
        REQUIRE(scene.getOrderingAsString() == "time");
        scene.setOrdering(EOrdering::SD_TRACK);
        REQUIRE(scene.getOrdering() == EOrdering::SD_TRACK);
        REQUIRE(scene.getOrderingAsString() == "track");
        REQUIRE(!scene.setOrdering("dummy")); // unknown
    }
}

TEST_CASE("addExtension() removeExtension() isExtensionActivated() getNumberOfActivatedExtensions() "){
    
    sdScene scene;
    scene.addExtension(EExtension::SD_SOURCE_WIDTH);
    REQUIRE(scene.getNumberOfActivatedExtensions() == 1);
    REQUIRE(scene.removeExtension(EExtension::SD_SOURCE_WIDTH) == 1);
    scene.addExtension(EExtension::SD_SOURCE_WIDTH);
    REQUIRE(!scene.addExtension(EExtension::SD_SOURCE_WIDTH)); // already exists
    scene.removeAllExtensions();
    REQUIRE(scene.getNumberOfActivatedExtensions() == 0);
    scene.addExtension(EExtension::SD_SOURCE_WIDTH);
    REQUIRE(scene.isExtensionActivated(EExtension::SD_SOURCE_WIDTH));
    REQUIRE(scene.removeExtension(EExtension::SD_SOURCE_WIDTH) == 1);
    REQUIRE(scene.removeExtension(EExtension::SD_SOURCE_WIDTH) == 0);
    REQUIRE(!scene.isExtensionActivated(EExtension::SD_SOURCE_WIDTH));

}

TEST_CASE("Test enable descriptor", "[sdScene]"){
    
    
    
    

    
    
}