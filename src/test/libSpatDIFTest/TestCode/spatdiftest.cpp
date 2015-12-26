
#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "sdMain.h"
#include <array>
using namespace std;

#pragma mark sdDate

TEST_CASE("sdDate member function test", "[sdDate]"){
    
    sdDate date = sdDate(1979,12,4);
    REQUIRE(date.getYear() == 1979);
    REQUIRE(date.getMonth() == 12);
    REQUIRE(date.getDay() == 4);
    REQUIRE(date.getDateAsString() == "1979-12-4");
    SECTION("reference passing"){
        unsigned short placeHolder[3];
        date.getDate(placeHolder);
        REQUIRE(placeHolder[0] == 1979);
        REQUIRE(placeHolder[1] == 12);
        REQUIRE(placeHolder[2] == 4);
        
        std::vector<unsigned short> vector = date.getDate();
        REQUIRE(vector[0] == 1979);
        REQUIRE(vector[1] == 12);
        REQUIRE(vector[2] == 4);
    }
    
    SECTION("setter test", "[sdDate]"){
        date.setYear(1945);
        date.setMonth(8);
        date.setDay(10);
        REQUIRE(date.getDateAsString() == "1945-8-10");

        date.setDate(1950, 10, 10);
        REQUIRE(date.getDateAsString() == "1950-10-10");
    }
    
    SECTION("invalid input", "[sdDate]"){
        date.setDate("invalid"); // due to format error, filled with current date
        sdDate date2; // current date
        REQUIRE(date.getDateAsString() == date2.getDateAsString()); // supposed to be same
    }
    SECTION("custom operator", "[sdDate]"){
        std::string dateString = date;
        REQUIRE(dateString == "1979-12-4");
    }
}



#pragma mark sdEvent


TEST_CASE("Test sdEvent add get remove", "[sdEvent]"){
    
    sdScene scene;
    sdEntity * entity = scene.addEntity("MyEntity");
    entity->addEvent<SD_PRESENT>(0.0, false); // 1
    REQUIRE(entity->getNumberOfEvents() == 1);
    
    auto addedEvent = entity->addEvent<SD_ORIENTATION>(1.234, {0.3,0.4,0.5}); // 2
    auto gotEvent =  entity->getEvent<SD_ORIENTATION>(1.234);
    REQUIRE(gotEvent);
    REQUIRE(gotEvent == addedEvent);
    REQUIRE(gotEvent->getValue()[0] == 0.3);
    REQUIRE(gotEvent->getValue()[1] == 0.4);
    REQUIRE(gotEvent->getValue()[2] == 0.5);

    auto event =  entity->getEvent<SD_ORIENTATION>(1.23400001); // a little bit different
    REQUIRE(!event );
    
    // overriding
    auto overridEvent = entity->addEvent<SD_PRESENT>(0.0, true); // 3 but override
    REQUIRE(entity->getNumberOfEvents() == 2);
    
    entity->removeEvent(1.234, SD_ORIENTATION);
    REQUIRE(entity->getNumberOfEvents() == 1);
    
    entity->removeAllEvents();
    REQUIRE(entity->getNumberOfEvents() == 0);
    
}

TEST_CASE("Test exceptions", "[sdEvent]"){
    sdScene scene;
    sdEntity * entity = scene.addEntity("MyEntity");
    
    try {
        entity->addEvent<SD_PRESENT>(-10.0, false); // invalid time, exception thrown and returns nullptr
    } catch (const InvalidTimeException &exception) {
        std::cerr << exception.what() << std::endl;
    }
    
    
}

TEST_CASE("Test all descriptors types"){
    sdScene scene;
    scene.addExtension(EExtension::SD_MEDIA);
    scene.addExtension(EExtension::SD_SOURCE_WIDTH);
    sdEntity * entity = scene.addEntity("MyEntity");
    
    // SD_PRESENT
    entity->addEvent<SD_PRESENT>(0.0, false);
    REQUIRE(entity->getValueAsString<SD_PRESENT>(0.0) == "false");
    REQUIRE(*(entity->getValue<SD_PRESENT>(0.0)) == false);
    
    entity->addEvent<SD_PRESENT>(0.0, true);
    REQUIRE(entity->getValueAsString<SD_PRESENT>(0.0) == "true");
    REQUIRE(*(entity->getValue<SD_PRESENT>(0.0)) == true);
    
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
    
    // SD_SOURCE_WIDTH_WIDTH
    entity->addEvent<SD_SOURCE_WIDTH_WIDTH>(0.0, 0.5);
    REQUIRE( entity->getValueAsString<SD_SOURCE_WIDTH_WIDTH>(0.0) == "0.5");
    REQUIRE( *entity->getValue<SD_SOURCE_WIDTH_WIDTH>(0.0) == 0.5);
    
    
}

TEST_CASE("getEvent() and getEvents()"){
    
    sdScene scene;
    sdEntity * entity = scene.addEntity("MyEntity");
    auto a = entity->addEvent<SD_POSITION>(0.2, {0.3, 0.1, 0.2});
    auto b = entity->addEvent<SD_PRESENT>(0.2, true);
    
    REQUIRE(entity->getEvent<SD_PRESENT>(0.2));
    REQUIRE(entity->getEvent<SD_POSITION>(0.2));
    REQUIRE(!entity->getEvent<SD_ORIENTATION>(0.2));
    
    REQUIRE(!entity->getEvent<SD_PRESENT>(0.20000001)); // slightly after
    REQUIRE(!entity->getEvent<SD_PRESENT>(0.19999999)); // slightly before
    
    auto eventSet = entity->getEventSet(0.2);
    REQUIRE(eventSet.size() == 2);
    entity->removeEvent(a);
    eventSet = entity->getEventSet(0.2);
    REQUIRE(eventSet.size() == 1);
    entity->removeEvent(b);
    eventSet = entity->getEventSet(0.2);
    REQUIRE(eventSet.size() == 0);
}



TEST_CASE("getFirstEvent() and getLastEvent()"){
    
    sdScene scene;
    sdEntity * entity = scene.addEntity("MyEntity");
    
    // no events in the entity, should return nullptr
    REQUIRE(!entity->getFirstEvent<SD_POSITION>());
    REQUIRE(!entity->getLastEvent<SD_POSITION>());
    
    // three events added
    auto a = entity->addEvent<SD_POSITION>(0.4, {0.5, 0.2, 0.3}); // chronologically third
    auto b = entity->addEvent<SD_POSITION>(0.2, {0.3, 0.1, 0.2}); // chronologically first
    auto c = entity->addEvent<SD_POSITION>(0.3, {0.1, 0.4, 0.6}); // chronologically second
    
    // looking for non-exisiting event
    REQUIRE(!entity->getFirstEvent<SD_PRESENT>());
    REQUIRE(!entity->getLastEvent<SD_PRESENT>());
    
    auto firstEvent = entity->getFirstEvent<SD_POSITION>();
    auto lastEvent = entity->getLastEvent<SD_POSITION>();
    REQUIRE(firstEvent == b);
    REQUIRE(lastEvent == a);
}

TEST_CASE("getFirstEvents() and getLastEvents()"){
    
    sdScene scene;
    sdEntity * entity = scene.addEntity("MyEntity");
    
    REQUIRE(entity->getFirstEventSet().empty());
    REQUIRE(entity->getLastEventSet().empty());
    
    // three events added
    auto a = entity->addEvent<SD_POSITION>(0.4, {0.5, 0.2, 0.3}); // chronologically third
    auto b = entity->addEvent<SD_POSITION>(0.2, {0.3, 0.1, 0.2}); // chronologically first
    auto c = entity->addEvent<SD_POSITION>(0.3, {0.1, 0.4, 0.6}); // chronologically second
    ;
    REQUIRE(entity->getFirstEventSet().size() == 1);
    REQUIRE(entity->getLastEventSet().size() == 1);
    
    // event added at first and last
    auto d = entity->addEvent<SD_PRESENT>(0.2, true);
    auto e = entity->addEvent<SD_PRESENT>(0.4, true);
    REQUIRE(entity->getFirstEventSet().size() == 2);
    REQUIRE(entity->getLastEventSet().size() == 2);
    
    // two event removed
    entity->removeEvent(d);
    entity->removeEvent(e);
    REQUIRE(entity->getFirstEventSet().size() == 1);
    REQUIRE(entity->getLastEventSet().size() == 1);
    REQUIRE(entity->getNumberOfEvents() == 3);
}

TEST_CASE("getFirstEventTime() getLastEventTime()"){
    sdScene scene;
    sdEntity * entity = scene.addEntity("MyEntity");
    
    // no events in the entity, so the set should be {0.0, false}
    REQUIRE(!entity->getFirstEventTime().second);
    REQUIRE(!entity->getLastEventTime().second);
    
    // three events added
    auto a = entity->addEvent<SD_POSITION>(0.4, {0.5, 0.2, 0.3}); // chronologically third
    auto b = entity->addEvent<SD_POSITION>(0.2, {0.3, 0.1, 0.2}); // chronologically first
    auto c = entity->addEvent<SD_POSITION>(0.3, {0.1, 0.4, 0.6}); // chronologically second
    
    REQUIRE(entity->getFirstEventTime().first == b->getTime());
    REQUIRE(entity->getLastEventTime().first == a->getTime());
    entity->removeEvent(b);
    entity->removeEvent(a);
    REQUIRE(entity->getFirstEventTime().first == c->getTime());
    REQUIRE(entity->getLastEventTime().first == c->getTime());
    
}

TEST_CASE("getNextEvent() getPreviousEvent()" ){
    sdScene scene;
    sdEntity * entity = scene.addEntity("MyEntity");
    
    // should return nullptr
    REQUIRE(!entity->getNextEvent<SD_POSITION>(0.35));
    REQUIRE(!entity->getPreviousEvent<SD_POSITION>(0.35));
    
    auto a = entity->addEvent<SD_POSITION>(0.4, {0.5, 0.2, 0.3}); // chronologically third
    auto b = entity->addEvent<SD_POSITION>(0.2, {0.3, 0.1, 0.2}); // chronologically first
    auto c = entity->addEvent<SD_POSITION>(0.3, {0.1, 0.4, 0.6}); // chronologically second
    auto d = entity->addEvent<SD_POSITION>(0.5, {0.2, 0.2, 0.2}); // chronologically fourth
    REQUIRE(entity->getNextEvent<SD_POSITION>(0.35) == a);
    REQUIRE(entity->getPreviousEvent<SD_POSITION>(0.35) == c);
    
    entity->removeEvent(a);
    entity->removeEvent(c);
    
    REQUIRE(entity->getNextEvent<SD_POSITION>(0.35) == d);
    REQUIRE(entity->getPreviousEvent<SD_POSITION>(0.35) == b);
    
    entity->removeEvent(b);
    entity->removeEvent(d);
    
    REQUIRE(!entity->getNextEvent<SD_POSITION>(0.35));
    REQUIRE(!entity->getPreviousEvent<SD_POSITION>(0.35));
}

TEST_CASE("getNextEventTime() getPreviousEventTime()" ){
    sdScene scene;
    sdEntity * entity = scene.addEntity("MyEntity");
    
    REQUIRE(!entity->getNextEventTime(0.0).second);
    REQUIRE(!entity->getPreviousEventTime(10.0).second);
    
    auto a = entity->addEvent<SD_POSITION>(0.4, {0.5, 0.2, 0.3}); // chronologically third
    auto b = entity->addEvent<SD_POSITION>(0.2, {0.3, 0.1, 0.2}); // chronologically first
    auto c = entity->addEvent<SD_POSITION>(0.3, {0.1, 0.4, 0.6}); // chronologically second
    auto d = entity->addEvent<SD_POSITION>(0.5, {0.2, 0.2, 0.2}); // chronologically fourth
    
    REQUIRE(a->getTime() == entity->getNextEventTime(0.35).first);
    REQUIRE(!entity->getNextEventTime(0.5).second);
    
    REQUIRE(c->getTime() == entity->getPreviousEventTime(0.35).first);
    REQUIRE(!entity->getPreviousEventTime(0.2).second);
    
}
TEST_CASE("getValue()"){
    
    sdScene scene;
    sdEntity * entity = scene.addEntity("MyEntity");
    std::array<double, 3> array = {0.5, 0.2, 0.3};
    auto a = entity->addEvent<SD_POSITION>(0.4, array); // chronologically third
    auto value = entity->getValue<SD_POSITION>(0.5); // cannot get value
    REQUIRE(!value);
    value = entity->getValue<SD_POSITION>(0.4);
    REQUIRE(value);
    REQUIRE(array == *value);
    
}

TEST_CASE("getValueAsString()"){
    
    sdScene scene;
    sdEntity * entity = scene.addEntity("MyEntity");
    auto event = entity->addEvent<SD_POSITION>(5.0, {0.2, 0.3, 0.4});
    REQUIRE(event->getValueAsString() == "0.2 0.3 0.4");
}

TEST_CASE("getNextValue() getPreviousValue()"){
    sdScene scene;
    sdEntity * entity = scene.addEntity("MyEntity");
    
    REQUIRE(!entity->getNextValue<SD_POSITION>(0.3));
    REQUIRE(!entity->getPreviousValue<SD_POSITION>(0.7));
    
    auto event = entity->addEvent<SD_POSITION>(0.5, {0.5, 0.2, 0.3});
    REQUIRE(entity->getNextValue<SD_POSITION>(0.3) == &event->getValue() );
    REQUIRE(entity->getPreviousValue<SD_POSITION>(0.7) == &event->getValue() );
    
    auto eventB = entity->addEvent<SD_POSITION>(0.4, {0.0, 0.0, 0.0});
    auto eventC = entity->addEvent<SD_POSITION>(0.6, {0.0, 0.0, 0.0});
    REQUIRE(entity->getNextValue<SD_POSITION>(0.3) == &eventB->getValue() );
    REQUIRE(entity->getPreviousValue<SD_POSITION>(0.7) == &eventC->getValue() );
    
    REQUIRE(!entity->getNextValue<SD_POSITION>(0.7));
    REQUIRE(!entity->getPreviousValue<SD_POSITION>(0.3));
}


#pragma mark sdEntity

TEST_CASE("Test sdEntity", "[sdEntity]"){
    sdScene scene;
    sdEntity * firstEntity = scene.addEntity("FirstEntity");
    sdEntity * secondEntity = scene.addEntity("SecondEntity", EKind::SD_SINK);
    REQUIRE(!scene.removeEntity("Nothing"));
    
    REQUIRE(firstEntity->getKind() == EKind::SD_SOURCE);
    REQUIRE(secondEntity->getKind() == EKind::SD_SINK);
    REQUIRE(firstEntity->getKindAsString() == "source");
    REQUIRE(secondEntity->getKindAsString() ==  "sink");

    REQUIRE(scene.removeEntity("FirstEntity"));
    REQUIRE(scene.getNumberOfEntities() == 1);
    scene.removeAllEntities();

}

#pragma mark sdScene

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
    
    SECTION("ordering", "[sdScene]"){
        
        REQUIRE(scene.getOrdering() == EOrdering::SD_TIME); // defualt should be time
        REQUIRE(scene.getOrderingAsString() == "time");
        scene.setOrdering(EOrdering::SD_TRACK);
        REQUIRE(scene.getOrdering() == EOrdering::SD_TRACK);
        REQUIRE(scene.getOrderingAsString() == "track");
        REQUIRE(!scene.setOrdering("dummy")); // unknown
    }
}

TEST_CASE("addEvent() getEvent()"){
    sdScene scene;
    scene.addEntity("myEntity");
    auto event = scene.addEvent<SD_POSITION>("myEntity", 1.0, {0.35, 0.1,0.2});
    auto value = scene.getValue<SD_POSITION>("myEntity", 1.0);
    REQUIRE(event->getValue()[0] == 0.35);
    REQUIRE(event->getValue()[1] == 0.1);
    REQUIRE(event->getValue()[2] == 0.2 );
}

TEST_CASE("get exact first last next previous eventset" "[sdScene]"){
    
    sdScene scene;
    auto myEntity = scene.addEntity("myEntity");
    auto yourEntity = scene.addEntity("yourEntity");
    
    myEntity->addEvent<SD_PRESENT>(1.0, true);
    yourEntity->addEvent<SD_PRESENT>(1.0, true);
    yourEntity->addEvent<SD_POSITION>(1.0, {2.0, 3.0, 0.0});
    myEntity->addEvent<SD_POSITION>(1.3, {3.0, 4.0, 0.0});
    
    
    myEntity->addEvent<SD_POSITION>(2.0, {2.0,3.0,4.0}); // first event
    yourEntity->addEvent<SD_POSITION>(2.0, {3.0,4.0,5.0});
    yourEntity->addEvent<SD_PRESENT>(2.0, false);
    myEntity->addEvent<SD_PRESENT>(2.0, false);

    REQUIRE(scene.getEventsFromAllEntities(2.0).size() == 4);
    REQUIRE(scene.getEventsFromAllEntities(0.5, 1.5).size() == 4);
    REQUIRE(scene.getFirstEventsFromAllEntities().size() == 3);
    REQUIRE(scene.getLastEventsFromAllEntities().size() == 4);
    REQUIRE(scene.getNextEventsFromAllEntities(1.5).size() == 4);
    REQUIRE(scene.getPreviousEventsFromAllEntities(1.5).size()==1);
    REQUIRE(scene.getNextEventTime(1.4).first == 2.0);
    REQUIRE(almostEqual(scene.getDeltaTimeToNextEvent(1.4).first ,0.6));
    REQUIRE(almostEqual(scene.getDeltaTimeFromPreviousEvent(1.4).first ,1.4-1.3 ));

    REQUIRE(scene.getPreviousEventTime(1.4).first == 1.3);

}

TEST_CASE("addExtension() removeExtension() isExtensionActivated() getNumberOfActivatedExtensions()", "[sdScene]"){
    
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

TEST_CASE("same as above but with strings"){
    
    sdScene scene;
    scene.addExtension("source-width");
    REQUIRE(scene.getNumberOfActivatedExtensions() == 1);
    REQUIRE(scene.removeExtension("source-width") == 1);
    REQUIRE(scene.getNumberOfActivatedExtensions() == 0);
    
    REQUIRE(scene.addExtension("source-width"));
    REQUIRE(!scene.addExtension("source-width")); // already exists
    scene.removeAllExtensions();
    REQUIRE(scene.getNumberOfActivatedExtensions() == 0);
    scene.addExtension("source-width");
    REQUIRE(scene.isExtensionActivated("source-width"));
    REQUIRE(!scene.isExtensionActivated("source-height"));
    
    REQUIRE(scene.removeExtension("source-width") == 1);
    REQUIRE(scene.removeExtension("source-width") == 0);
    REQUIRE(scene.removeExtension("source-height") == 0);
    
    REQUIRE(!scene.isExtensionActivated("source-width"));
}

TEST_CASE("valid descriptor test"){
    sdScene scene;
    REQUIRE(scene.getValidDescriptorSet().size() == 4);
    scene.addExtension(EExtension::SD_SOURCE_WIDTH);
    REQUIRE(scene.getValidDescriptorSet().size() == 5);
    
    
}

#pragma mark sdOSCConverter
TEST_CASE("sdOSCConverter", "[sdOSCConverter]"){
    auto intArg = sdOSCConverter::toBlock(32);
    auto floatArg = sdOSCConverter::toBlock(23.42f);
    auto stringArg = sdOSCConverter::toBlock(std::string("chikashi"));
    
    REQUIRE(sdOSCConverter::blockTo<int>(intArg) == 32);
    REQUIRE(sdOSCConverter::blockTo<float>(floatArg) == 23.42f);
    REQUIRE(sdOSCConverter::blockTo<std::string>(stringArg) == std::string("chikashi"));
    
}


#pragma mark sdOSCMessage
TEST_CASE("sdOSCMessage", "[sdOSCMessage]"){
    sdOSCMessage mes;
    // default value in constructor
    REQUIRE(mes.getAddressAsString() == "/spatdif");
    
    // address
    mes.setAddress("/spatdif/position");
    REQUIRE(mes.getAddressAsString() == "/spatdif/position");

    // append arguments
    mes.appendArgument(9.1f);
    mes.appendArgument(3);
    mes.appendArgument(std::string("testing"));
    
    //type tag check
    REQUIRE(mes.getTypetags()[0] == ',');
    REQUIRE(mes.getTypetags()[1] == 'f');
    REQUIRE(mes.getTypetags()[2] == 'i');
    REQUIRE(mes.getTypetags()[3] == 's');
    REQUIRE(mes.getTypetagsAsString() == ",fis");
    
    // argument check
    REQUIRE(mes.getArgument<float>(0) == 9.1f);
    REQUIRE(mes.getArgument<int>(1) == 3);
    REQUIRE(mes.getArgument<std::string>(2) == std::string("testing"));
    REQUIRE(mes.getArguments().size() == 16);
    REQUIRE(mes.getNumberOfArguments() == 3);
    
    // entire message
    REQUIRE(mes.getAllArgumentsAsString() == "9.1 3 testing");
    REQUIRE(mes.getOSCMessage().size() == 44);
    REQUIRE(mes.getMessageAsString() == "/spatdif/position ,fis 9.1 3 testing");

    mes.clear();
    REQUIRE(mes.getNumberOfArguments() == 0);
    REQUIRE(mes.getAddressAsString() == "");
    REQUIRE(mes.getTypetagsAsString() == ",");
}

#pragma mark sdUtility

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
    REQUIRE( toString(true) == "true");
    REQUIRE( toString(false) == "false");
    
    REQUIRE( stringTo<bool>("true") == true);
    REQUIRE( stringTo<bool>("false") == false);
    
    REQUIRE( stringTo<int>("43") == 43);
    REQUIRE( stringTo<double>("423.1") == 423.1);
    SECTION("int array"){
        auto array = stringToArray<int,3>("4 3 2");
        REQUIRE(array.at(0) == 4);
        REQUIRE(array.at(1) == 3);
        REQUIRE(array.at(2) == 2);
    }
    
    SECTION("double array"){
        auto array = stringToArray<double,2>("4.2 3.2");
        REQUIRE(array.at(0) == 4.2);
        REQUIRE(array.at(1) == 3.2);
    }
}


// old tests from old API
TEST_CASE("source width test"){
    sdScene scene;
    scene.addExtension(EExtension::SD_SOURCE_WIDTH);

    REQUIRE(scene.getNumberOfActivatedExtensions() == 1);

    auto extensionStringVector = scene.getActivatedExtensionsAsStrings();
    

    sdEntity* entityOne = scene.addEntity("voice1");


    auto eventOne = entityOne->addEvent<SD_SOURCE_WIDTH_WIDTH>(5.0, 105.2);
    auto eventTwo = entityOne->addEvent("10.0", "width", "99");

    REQUIRE(eventOne == entityOne->getNextEvent<SD_SOURCE_WIDTH_WIDTH>(2.0));
    REQUIRE(eventTwo.get() == entityOne->getPreviousEvent<SD_SOURCE_WIDTH_WIDTH>(12.0));

    scene.removeExtension(EExtension::SD_SOURCE_WIDTH);
}



TEST_CASE("info test"){
    {
        sdInfo info; // empty
        info.setAuthor(string("John"));
        info.setHost(string("sdInfoTest"));
        info.setDate(sdDate("2000-01-01"));
        info.setSession(string("1.1"));
        info.setLocation(string("ZHDK"));
        info.setAnnotation(string("this is a test"));
        
        REQUIRE( info.getAuthor() == "John");
        REQUIRE( info.getHost() == "sdInfoTest");
        REQUIRE( info.getDateAsString() == "2000-1-1");
        REQUIRE( info.getSession() == "1.1");
        REQUIRE( info.getLocation() == "ZHDK");
        REQUIRE( info.getAnnotation() == "this is a test");
    }
    {
        // set at once with strings and sdDate
        sdInfo info(string("Tom"), string("sdInfoTest"), sdDate(string("2012-04-03")), string("1.2"), string("ESB"), string("this is second test"));
        
        REQUIRE( info.getAuthor() == "Tom");
        REQUIRE( info.getHost() == "sdInfoTest");
        REQUIRE( info.getDateAsString() == "2012-4-3");
        REQUIRE( info.getSession() == "1.2");
        REQUIRE( info.getLocation() == "ESB");
        REQUIRE( info.getAnnotation() == "this is second test");
    }
    {
        // initialization also possible with c-strings
        sdInfo info("Kevin", "sdInfoTest", "2012-05-01", "1.3", "SFEM", "this is third test");
        
        REQUIRE( info.getAuthor() == "Kevin");
        REQUIRE( info.getHost() == "sdInfoTest");
        REQUIRE( info.getDateAsString() == "2012-5-1");
        REQUIRE( info.getSession() == "1.3");
        REQUIRE( info.getLocation() == "SFEM");
        REQUIRE( info.getAnnotation() == "this is third test");
    }
}


TEST_CASE("How to Query loaded file"){
    ifstream ifs("/Users/chikashi/Development/spatdiflib/src/test/libSpatDIFTest/TestCode/simple_scene.xml");
    string xmlString;
    if (ifs.is_open()){
        while ( ifs.good() ){
            string str;
            getline(ifs,str);
            xmlString.append(str);
        }
        ifs.close();
    }
    REQUIRE(xmlString != "");
    
    sdScene myScene = sdLoader::sceneFromXML(xmlString);
    sdEntity *voice1 = myScene.getEntity("voice1");
    if(!voice1) {abort();}
        
    REQUIRE(voice1->getName() == "voice1");
    auto event = voice1->getEvent<SD_POSITION>(2.0);
    auto position = event->getValue();
    REQUIRE(position[0] == 0.0);
    REQUIRE(position[1] == 1.0);
    REQUIRE(position[2] == 0.0);
    
    
    double currentTime = -1.0;
    int count = 0;
    double time[] = {0.0, 2.0, 3.0};
    double value[][3] = {{0.0, 0.0, 0.0},{0.0,1.0,0.0},{1.0,1.0,2.0}};
    std::string valueString[3] = {"0 0 0","0 1 0","1 1 2"};
    while (true) {
        auto sEvent = voice1->getNextEvent<SD_POSITION>(currentTime);
        if(sEvent == NULL) break;
        currentTime = sEvent->getTime();
        auto val = sEvent->getValue();
        auto valString = sEvent->getValueAsString();
        REQUIRE(currentTime == time[count]);
        REQUIRE(value[count][0] == val[0]);
        REQUIRE(value[count][1] == val[1]);
        REQUIRE(value[count][2] == val[2]);
        REQUIRE(valString == valueString[count]);
        count++;
    }
    
    auto firstEvent = voice1->getFirstEvent<SD_POSITION>();
    REQUIRE(firstEvent->getTime() == 0.0);
    REQUIRE(firstEvent->getValueAsString() == "0 0 0");
    
    auto lastEvent = voice1->getLastEvent<SD_POSITION>();
    REQUIRE(lastEvent->getTime() == 3.0);
    REQUIRE(lastEvent->getValueAsString() == "1 1 2");
    
    // just get last and first time tag
    REQUIRE(voice1->getFirstEventTime().first == 0.0);
    REQUIRE(voice1->getLastEventTime().first == 3.0);

    auto voice2 = myScene.getEntity(string("voice2"));
    auto voice2Events = voice2->getEvents(0.0, 8.0);
    REQUIRE(voice2->getName() == "voice2");
    REQUIRE(voice2Events.size() == 1);
    
    auto voice2EventsEmpty = voice2->getEvents(8.0, 10.0);
    REQUIRE(voice2EventsEmpty.empty());
    
    auto pos = myScene.getValue<SD_POSITION>("voice1", 3.0);
    REQUIRE( (*pos)[0] == 1);
    REQUIRE( (*pos)[1] == 1);
    REQUIRE( (*pos)[2] == 2);
}


TEST_CASE("sdOSCConverter Test"){
    auto dumpBytes = [](vector<unsigned char> bytes){
        cout << "number of bytes:" << std::dec << bytes.size() << endl;
        cout << "content:" << endl;
        
        vector<unsigned char>::iterator it = bytes.begin();
        int count = 0;
        while (it != bytes.end()) {
            unsigned char c = *it;
            cout << hex <<  setw(2) << setfill('0') << static_cast<int>(c) << ' ';
            it++;
            count++;
            if(count % 4 == 0){
                cout << endl;
            }
        }
    };
    
    sdOSCConverter converter;
    int ioriginal = 1000;
    vector<unsigned char> ic = converter.toBlock(ioriginal);
    dumpBytes(ic);
    REQUIRE(ioriginal == converter.blockTo<int>(ic));
    
    float foriginal = 1.234;
    vector<unsigned char> fc = converter.toBlock(foriginal);
    dumpBytes(fc);
    REQUIRE(foriginal == converter.blockTo<float>(fc));

    string soriginal = "hello";
    vector<unsigned char> sc = converter.toBlock(soriginal); // here call it directly
    dumpBytes(sc);
    REQUIRE(soriginal == converter.blockTo<string>(sc));

}

TEST_CASE("sdSaverTest"){
    
}

TEST_CASE("sdOSCMessageTest"){
    
}
