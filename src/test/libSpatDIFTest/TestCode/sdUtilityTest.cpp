
#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "sdConst.h"
#include <iostream>


TEST_CASE("Test Utilities", "[sdConst]"){
    REQUIRE(extensionToString(SD_CORE) == "core");
    REQUIRE(extensionToString(SD_DIRECT_TO_ONE) == "direct-to-one");
    REQUIRE(extensionToString(SD_MEDIA) == "media");
    REQUIRE(extensionToString(SD_SOURCE_WIDTH) == "source-width");

}