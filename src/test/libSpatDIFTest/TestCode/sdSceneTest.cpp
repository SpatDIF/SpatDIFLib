
#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "sdScene.h"

using namespace std;

TEST_CASE("Test sdScene", "[sdScene]"){
    sdScene scene;
    REQUIRE(scene.getOrdering() == SD_TIME);
    SECTION("entity control"){
        scene.addEntity("first");
        REQUIRE(scene.getNumberOfEntities() == 1);
        
        
        
    }
    
} 