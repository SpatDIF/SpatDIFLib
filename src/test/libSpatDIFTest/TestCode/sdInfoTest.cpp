
#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "sdInfo.h"

#include <iostream>
using namespace std;
TEST_CASE("Test sdDate", "[sdInfo]"){
    SECTION("constructor check"){
        sdInfo info1 = sdInfo();
        sdDate date1;
        REQUIRE(info1.getAuthor() == "");
        REQUIRE(info1.getHost() == "");
        REQUIRE(info1.getDate().getDateAsString() == date1.getDateAsString());
        REQUIRE(info1.getSession() == "");
        REQUIRE(info1.getLocation() == "");
        REQUIRE(info1.getAnnotation() == "");
        
        sdDate date2("1979-12-4");
        sdInfo info2 = sdInfo("author", "host", date2, "session", "location", "this is test");
        REQUIRE(info2.getAuthor() == "author");
        REQUIRE(info2.getHost() == "host");
        REQUIRE(info2.getDate().getDateAsString() == date2.getDateAsString());
        REQUIRE(info2.getSession() == "session");
        REQUIRE(info2.getLocation() == "location");
        REQUIRE(info2.getAnnotation() == "this is test");
        
        sdInfo info3 = sdInfo(std::string("author"),
                              std::string("host"),
                              std::string("1979-12-4"),
                              std::string("session"),
                              std::string("location"),
                              std::string("this is test"));
        REQUIRE(info3.getAuthor() == "author");
        REQUIRE(info3.getHost() == "host");
        REQUIRE(info3.getDate().getDateAsString() == date2.getDateAsString());
        REQUIRE(info3.getSession() == "session");
        REQUIRE(info3.getLocation() == "location");
        REQUIRE(info3.getAnnotation() == "this is test");
        
        sdInfo info4 = sdInfo("author", "host", "1979-12-4", "session", "location", "this is test");
        REQUIRE(info4.getAuthor() == "author");
        REQUIRE(info4.getHost() == "host");
        REQUIRE(info4.getDate().getDateAsString() == date2.getDateAsString());
        REQUIRE(info4.getSession() == "session");
        REQUIRE(info4.getLocation() == "location");
        REQUIRE(info4.getAnnotation() == "this is test");
        
    }
    
    SECTION("accessor check"){
        sdInfo info = sdInfo();
        sdDate date;
        info.setAuthor("author");
        REQUIRE(info.getAuthor() == "author");
        
        info.setHost("host");
        REQUIRE(info.getHost() == "host");
        
        info.setSession("session");
        REQUIRE(info.getSession() == "session");

        REQUIRE(info.getDate().getDateAsString() == date.getDateAsString());
        REQUIRE(info.getDateAsString() == date.getDateAsString());
        
        info.setLocation("location");
        REQUIRE(info.getLocation() == "location");
        
        info.setAnnotation("this is test");
        REQUIRE(info.getAnnotation() == "this is test");
        
        info.set("chikashi", "mac", "1979-12-4", "session1", "icst", "last test");
        REQUIRE(info.getAuthor() == "chikashi");
        REQUIRE(info.getHost() == "mac");
        REQUIRE(info.getDate().getDateAsString() == "1979-12-4");
        REQUIRE(info.getSession() == "session1");
        REQUIRE(info.getLocation() == "icst");
        REQUIRE(info.getAnnotation() == "last test");
    }
}