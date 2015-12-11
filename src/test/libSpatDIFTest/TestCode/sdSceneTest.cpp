
#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "sdScene.h"
#include <iostream>

using namespace std;

TEST_CASE("Test sdScene", "[sdScene]"){
    sdScene scene;
    REQUIRE(scene.getOrdering() == SD_TIME);
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
        
        REQUIRE(scene.getOrdering() == SD_TIME); // defualt should be time
        REQUIRE(scene.getOrderingAsString() == "time");

        scene.setOrdering(SD_TRACK);
        REQUIRE(scene.getOrdering() == SD_TRACK);
        REQUIRE(scene.getOrderingAsString() == "track");

        scene.setOrdering("dummy");
        
    }
    
}

TEST_CASE("Test extension", "[sdScene]"){
    sdScene scene;
    scene.addExtension(SD_MEDIA)


}