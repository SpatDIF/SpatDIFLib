
#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "sdConst.h"
#include <iostream>
#include <array>

TEST_CASE("Test Utilities", "[sdConst]"){
    REQUIRE(sdExtension::extensionToString(EExtension::SD_CORE) == "core");
    REQUIRE(sdExtension::extensionToString(EExtension::SD_MEDIA) == "media");
    REQUIRE(sdExtension::extensionToString(EExtension::SD_SOURCE_WIDTH) == "source-width");

    REQUIRE(sdExtension::stringToExtension("core") == EExtension::SD_CORE);
    REQUIRE(sdExtension::stringToExtension("media") == EExtension::SD_MEDIA);
    REQUIRE(sdExtension::stringToExtension("source-width") == EExtension::SD_SOURCE_WIDTH);

    REQUIRE(sdExtension::descriptorToString(EDescriptor::SD_TYPE) == "type");
    REQUIRE(sdExtension::descriptorToString(EDescriptor::SD_PRESENT) == "present");
    REQUIRE(sdExtension::descriptorToString(EDescriptor::SD_POSITION) == "position");
    REQUIRE(sdExtension::descriptorToString(EDescriptor::SD_ORIENTATION) == "orientation");
    
    REQUIRE(sdExtension::descriptorToString(EDescriptor::SD_MEDIA_ID) == "id");
    REQUIRE(sdExtension::descriptorToString(EDescriptor::SD_MEDIA_TYPE) == "type");
    REQUIRE(sdExtension::descriptorToString(EDescriptor::SD_MEDIA_LOCATION) == "location");
    REQUIRE(sdExtension::descriptorToString(EDescriptor::SD_MEDIA_CHANNEL) == "channel");
    REQUIRE(sdExtension::descriptorToString(EDescriptor::SD_MEDIA_TIME_OFFSET) == "time-offset");
    
    REQUIRE(sdExtension::descriptorToString(EDescriptor::SD_SOURCE_WIDTH_WIDTH) == "width");

    REQUIRE(sdExtension::stringToDescriptor(EExtension::SD_CORE,"type") == EDescriptor::SD_TYPE);
    REQUIRE(sdExtension::stringToDescriptor(EExtension::SD_CORE,"present") == EDescriptor::SD_PRESENT);
    REQUIRE(sdExtension::stringToDescriptor(EExtension::SD_CORE,"position") == EDescriptor::SD_POSITION);
    REQUIRE(sdExtension::stringToDescriptor(EExtension::SD_CORE,"orientation") == EDescriptor::SD_ORIENTATION);

    REQUIRE(sdExtension::stringToDescriptor(EExtension::SD_MEDIA, "id") == SD_MEDIA_ID);
    REQUIRE(sdExtension::stringToDescriptor(EExtension::SD_MEDIA, "type") == SD_MEDIA_TYPE);
    REQUIRE(sdExtension::stringToDescriptor(EExtension::SD_MEDIA, "location") == SD_MEDIA_LOCATION);
    REQUIRE(sdExtension::stringToDescriptor(EExtension::SD_MEDIA, "channel") == SD_MEDIA_CHANNEL);
    REQUIRE(sdExtension::stringToDescriptor(EExtension::SD_MEDIA, "time-offset") == SD_MEDIA_TIME_OFFSET);
    
    REQUIRE(sdExtension::stringToDescriptor(EExtension::SD_SOURCE_WIDTH, "width") == SD_SOURCE_WIDTH_WIDTH);
    


    
    // std::array to string
    REQUIRE( toString(std::array<int, 3>({{3,4,5}})) == "3 4 5");
    REQUIRE( toString(std::array<double, 2>({{3.3,4.4}})) == "3.3 4.4");

    // single arithmatic to string
    REQUIRE( toString(32) == "32");
    REQUIRE( toString(42.31) == "42.31");
    REQUIRE( toString(42.31f) == "42.31");

    // bool to string
    REQUIRE(toString(true) == "true");
    REQUIRE(toString(false) == "false");

}