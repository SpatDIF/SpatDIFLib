
#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <array>
#include "sdScene.h"

TEST_CASE("Test sdEntity", "[sdEntity]"){
    sdScene scene;
    sdEntity * firstEntity = scene.addEntity("FirstEntity");
    sdEntity * secondEntity = scene.addEntity("SecondEntity");
    REQUIRE(!scene.removeEntity("Nothing"));
    REQUIRE(scene.removeEntity("FirstEntity"));
    REQUIRE(scene.getNumberOfEntities() == 1);
    scene.removeAllEntities();
}
