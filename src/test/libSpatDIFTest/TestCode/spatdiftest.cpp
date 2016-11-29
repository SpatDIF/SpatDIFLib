
#include "catch.hpp"
#include "sdMain.h"
#include <array>
using namespace std;
#define CATCH_CONFIG_MAIN

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
#pragma mark sdMeta

TEST_CASE("Test sdMeta add get remove", "[sdMeta]"){
    sdScene scene;
    sdEntity * entity = scene.addEntity("MyEntity");
    entity->addMeta<SD_PRESENT>(false);
    REQUIRE(entity->getNumberOfMetas() == 1);
    
    auto addedMeta = entity->addMeta<SD_ORIENTATION>({0.3,0.4,0.5});
    REQUIRE(entity->getNumberOfMetas() == 2);
    
    auto gotValue =  entity->getMeta<SD_ORIENTATION>()->getValue();
    REQUIRE(gotValue[0] == 0.3);
    REQUIRE(gotValue[1] == 0.4);
    REQUIRE(gotValue[2] == 0.5);

    auto modifiedMeta = entity->addMeta<SD_ORIENTATION>({0.1,0.2,0.3});
    REQUIRE(entity->getNumberOfMetas() == 2);

    auto modifiedVal = modifiedMeta->getValue();
    REQUIRE(modifiedVal[0] == 0.1);
    REQUIRE(modifiedVal[1] == 0.2);
    REQUIRE(modifiedVal[2] == 0.3);
    
    entity->removeMeta(modifiedMeta); 
    REQUIRE(entity->getNumberOfMetas() == 1);
    entity->addMeta<SD_POSITION>({2,3,4});
    REQUIRE(entity->getNumberOfMetas() == 2);
    entity->removeAllMetas();
    REQUIRE(entity->getNumberOfMetas() == 0);
    
    
    auto shouldBeNull = entity->addMeta<SD_HARDWARE_OUT_PHYSICAL_CHANNEL>(10);
    REQUIRE(shouldBeNull == nullptr);
    
}

TEST_CASE("Test sink entity", "[sdMeta]"){
    sdScene scene;
    scene.addExtension(EExtension::SD_HARDWARE_OUT);
    sdEntity * speaker = scene.addEntity("mySpeaker");
    speaker->addMeta<SD_POSITION>({0.3, 0.5, 0.1});
    speaker->addMeta<SD_HARDWARE_OUT_PHYSICAL_CHANNEL>(1);
    speaker->addMeta<SD_HARDWARE_OUT_GAIN>(0.65);
    
    auto position = speaker->getMeta<SD_POSITION>()->getValue();
    REQUIRE(position[0] == 0.3);
    REQUIRE(position[1] == 0.5);
    REQUIRE(position[2] == 0.1);
    
    auto physicalout = speaker->getMeta<SD_HARDWARE_OUT_PHYSICAL_CHANNEL>()->getValue();
    auto gain = speaker->getMeta<SD_HARDWARE_OUT_GAIN>()->getValue();
    
    REQUIRE(physicalout == 1);
    REQUIRE(gain == 0.65);
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
    
    auto eventSet = entity->getEvents(0.2);
    REQUIRE(eventSet.size() == 2);
    entity->removeEvent(a);
    eventSet = entity->getEvents(0.2);
    REQUIRE(eventSet.size() == 1);
    entity->removeEvent(b);
    eventSet = entity->getEvents(0.2);
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
    
    REQUIRE(entity->getFirstEvents().empty());
    REQUIRE(entity->getLastEvents().empty());
    
    // three events added
    auto a = entity->addEvent<SD_POSITION>(0.4, {0.5, 0.2, 0.3}); // chronologically third
    auto b = entity->addEvent<SD_POSITION>(0.2, {0.3, 0.1, 0.2}); // chronologically first
    auto c = entity->addEvent<SD_POSITION>(0.3, {0.1, 0.4, 0.6}); // chronologically second
    REQUIRE(entity->getFirstEvents().size() == 1);
    REQUIRE(entity->getLastEvents().size() == 1);
    
    // event added at first and last
    auto d = entity->addEvent<SD_PRESENT>(0.2, true);
    auto e = entity->addEvent<SD_PRESENT>(0.4, true);
    REQUIRE(entity->getFirstEvents().size() == 2);
    REQUIRE(entity->getLastEvents().size() == 2);
    
    // two event removed
    entity->removeEvent(d);
    entity->removeEvent(e);
    REQUIRE(entity->getFirstEvents().size() == 1);
    REQUIRE(entity->getLastEvents().size() == 1);
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
    std::array<double, 3> meta = {0.2,0.4,0.5};
    std::array<double, 3> array = {0.5, 0.2, 0.3};
    auto m = entity->addMeta<SD_POSITION>(meta);
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
    sdEntity * secondEntity = scene.addEntity("SecondEntity", ECATEGORY::SD_SINK);
    REQUIRE(!scene.removeEntity("Nothing"));
    
    REQUIRE(firstEntity->getCategory() == ECATEGORY::SD_SOURCE);
    REQUIRE(secondEntity->getCategory() == ECATEGORY::SD_SINK);
    REQUIRE(firstEntity->getCategoryAsString() == "source");
    REQUIRE(secondEntity->getCategoryAsString() ==  "sink");

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
        auto set = scene.getEntityNames();
        REQUIRE(set.find("first") != set.end());
        REQUIRE(set.find("second") != set.end());
        REQUIRE(set.find("third") == set.end());
        
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
        try{
            (scene.setOrdering("dummy")); // unknown
            REQUIRE(false); // this line should be not reached because of the invalid value
        }catch(InvalidDescriptorException){}
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
    REQUIRE(sdUtils::almostEqual(scene.getDeltaTimeToNextEvent(1.4).first ,0.6));
    REQUIRE(sdUtils::almostEqual(scene.getDeltaTimeFromPreviousEvent(1.4).first ,1.4-1.3 ));

    REQUIRE(scene.getPreviousEventTime(1.4).first == 1.3);

}

TEST_CASE("addExtension() removeExtension() isExtensionActivated() getNumberOfActivatedExtensions()", "[sdScene]"){
    
    sdScene scene;
    scene.addExtension(EExtension::SD_SOURCE_SPREAD);
    REQUIRE(scene.getNumberOfActivatedExtensions() == 1);
    REQUIRE(scene.removeExtension(EExtension::SD_SOURCE_SPREAD) == 1);
    scene.addExtension(EExtension::SD_SOURCE_SPREAD);
    REQUIRE(!scene.addExtension(EExtension::SD_SOURCE_SPREAD)); // already exists
    scene.removeAllExtensions();
    REQUIRE(scene.getNumberOfActivatedExtensions() == 0);
    scene.addExtension(EExtension::SD_SOURCE_SPREAD);
    REQUIRE(scene.isExtensionActivated(EExtension::SD_SOURCE_SPREAD));
    REQUIRE(scene.removeExtension(EExtension::SD_SOURCE_SPREAD) == 1);
    REQUIRE(scene.removeExtension(EExtension::SD_SOURCE_SPREAD) == 0);
    REQUIRE(!scene.isExtensionActivated(EExtension::SD_SOURCE_SPREAD));
    
}

TEST_CASE("same as above but with strings"){
    
    sdScene scene;
    scene.addExtension("source-spread");
    REQUIRE(scene.getNumberOfActivatedExtensions() == 1);
    REQUIRE(scene.removeExtension("source-spread") == 1);
    REQUIRE(scene.getNumberOfActivatedExtensions() == 0);
    
    REQUIRE(scene.addExtension("source-spread"));
    REQUIRE(!scene.addExtension("source-spread")); // already exists
    scene.removeAllExtensions();
    REQUIRE(scene.getNumberOfActivatedExtensions() == 0);
    scene.addExtension("source-spread");
    REQUIRE(scene.isExtensionActivated("source-spread"));
    REQUIRE(!scene.isExtensionActivated("source-height"));
    
    REQUIRE(scene.removeExtension("source-spread") == 1);
    REQUIRE(scene.removeExtension("source-spread") == 0);
    REQUIRE(scene.removeExtension("source-height") == 0);
    
    REQUIRE(!scene.isExtensionActivated("source-spread"));
}

TEST_CASE("valid descriptor test"){
    sdScene scene;
    REQUIRE(scene.getNumberOfValidDescriptors() == 22);
    scene.addExtension(EExtension::SD_SOURCE_SPREAD);
    REQUIRE(scene.getNumberOfValidDescriptors() == 23);
}

#pragma mark sdOSCConverter
TEST_CASE("sdOSCConverter", "[sdOSCConverter]"){
    auto intArg = sdOSCConverter::toBlock(32);
    auto floatArg = sdOSCConverter::toBlock(23.42f);
    auto floatArg2 = sdOSCConverter::toBlock(29.42); // double to float
    auto stringArg = sdOSCConverter::toBlock(std::string("chikashi"));
    auto stringArg2 =sdOSCConverter::toBlock<std::string>("miyama");
    
    REQUIRE(sdOSCConverter::blockTo<int>(intArg) == 32);
    REQUIRE(sdOSCConverter::blockTo<float>(floatArg) == 23.42f);
    REQUIRE(sdOSCConverter::blockTo<float>(floatArg2) == 29.42f);

    REQUIRE(sdOSCConverter::blockTo<std::string>(stringArg) == std::string("chikashi"));
    REQUIRE(sdOSCConverter::blockTo<std::string>(stringArg2) == std::string("miyama"));
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

TEST_CASE("Test Extension enum to string conversion", "[sdConst]"){
    
    //// extension
    // enum to string
    REQUIRE(sdSpec::extensionToString(EExtension::SD_CORE) == "core");
    REQUIRE(sdSpec::extensionToString(EExtension::SD_MEDIA) == "media");
    REQUIRE(sdSpec::extensionToString(EExtension::SD_INTERPOLATION) == "interpolation");
    REQUIRE(sdSpec::extensionToString(EExtension::SD_POINTSET) == "pointset");
    REQUIRE(sdSpec::extensionToString(EExtension::SD_SOURCE_SPREAD) == "source-spread");
    REQUIRE(sdSpec::extensionToString(EExtension::SD_HARDWARE_OUT) == "hardware-out");
    REQUIRE(sdSpec::extensionToString(EExtension::SD_SINK_ENTITY) == "sink-entity");
    
    // string to enum
    REQUIRE(sdSpec::stringToExtension("core") == EExtension::SD_CORE);
    REQUIRE(sdSpec::stringToExtension("media") == EExtension::SD_MEDIA);
    REQUIRE(sdSpec::stringToExtension("interpolation") == EExtension::SD_INTERPOLATION);
    REQUIRE(sdSpec::stringToExtension("pointset") == EExtension::SD_POINTSET);
    REQUIRE(sdSpec::stringToExtension("source-spread") == EExtension::SD_SOURCE_SPREAD);
    REQUIRE(sdSpec::stringToExtension("hardware-out") == EExtension::SD_HARDWARE_OUT);
    REQUIRE(sdSpec::stringToExtension("sink-entity") == EExtension::SD_SINK_ENTITY);
    
    //// descriptor
    // enum to string
    REQUIRE(sdSpec::descriptorToString(EDescriptor::SD_TYPE) == "type");
    REQUIRE(sdSpec::descriptorToString(EDescriptor::SD_PRESENT) == "present");
    REQUIRE(sdSpec::descriptorToString(EDescriptor::SD_POSITION) == "position");
    REQUIRE(sdSpec::descriptorToString(EDescriptor::SD_ORIENTATION) == "orientation");
    
    REQUIRE(sdSpec::descriptorToString(EDescriptor::SD_MEDIA_ID) == "id");
    REQUIRE(sdSpec::descriptorToString(EDescriptor::SD_MEDIA_TYPE) == "type");
    REQUIRE(sdSpec::descriptorToString(EDescriptor::SD_MEDIA_LOCATION) == "location");
    REQUIRE(sdSpec::descriptorToString(EDescriptor::SD_MEDIA_CHANNEL) == "channel");
    REQUIRE(sdSpec::descriptorToString(EDescriptor::SD_MEDIA_TIME_OFFSET) == "time-offset");
    
    REQUIRE(sdSpec::descriptorToString(EDescriptor::SD_SOURCE_SPREAD_SPREAD) == "spread");
    
    REQUIRE(sdSpec::stringToDescriptor(EExtension::SD_CORE,"type") == EDescriptor::SD_TYPE);
    REQUIRE(sdSpec::stringToDescriptor(EExtension::SD_CORE,"present") == EDescriptor::SD_PRESENT);
    REQUIRE(sdSpec::stringToDescriptor(EExtension::SD_CORE,"position") == EDescriptor::SD_POSITION);
    REQUIRE(sdSpec::stringToDescriptor(EExtension::SD_CORE,"orientation") == EDescriptor::SD_ORIENTATION);
    
    REQUIRE(sdSpec::stringToDescriptor(EExtension::SD_MEDIA, "id") == SD_MEDIA_ID);
    REQUIRE(sdSpec::stringToDescriptor(EExtension::SD_MEDIA, "type") == SD_MEDIA_TYPE);
    REQUIRE(sdSpec::stringToDescriptor(EExtension::SD_MEDIA, "location") == SD_MEDIA_LOCATION);
    REQUIRE(sdSpec::stringToDescriptor(EExtension::SD_MEDIA, "channel") == SD_MEDIA_CHANNEL);
    REQUIRE(sdSpec::stringToDescriptor(EExtension::SD_MEDIA, "time-offset") == SD_MEDIA_TIME_OFFSET);
    
    REQUIRE(sdSpec::stringToDescriptor(EExtension::SD_POINTSET, "id") == SD_POINTSET_ID);
    REQUIRE(sdSpec::stringToDescriptor(EExtension::SD_POINTSET, "closed") == SD_POINTSET_CLOSED);
    REQUIRE(sdSpec::stringToDescriptor(EExtension::SD_POINTSET, "point") == SD_POINTSET_POINT);
    REQUIRE(sdSpec::stringToDescriptor(EExtension::SD_POINTSET, "handle") == SD_POINTSET_HANDLE);

    REQUIRE(sdSpec::stringToDescriptor(EExtension::SD_INTERPOLATION, "type") == SD_INTERPOLATION_TYPE);
    
    REQUIRE(sdSpec::stringToDescriptor(EExtension::SD_SOURCE_SPREAD, "spread") == SD_SOURCE_SPREAD_SPREAD);
    
    REQUIRE(sdSpec::stringToDescriptor(EExtension::SD_HARDWARE_OUT, "physical-channel") == SD_HARDWARE_OUT_PHYSICAL_CHANNEL);
    REQUIRE(sdSpec::stringToDescriptor(EExtension::SD_HARDWARE_OUT, "gain") == SD_HARDWARE_OUT_GAIN);
    

    
}

TEST_CASE("Test Utilities", "[sdConst]"){
    std::array<double, 3> xyz{1,1,1};
    auto aed = sdUtils::xyzToAed(xyz);
    auto rXyz = sdUtils::aedToXyz(aed);
    REQUIRE(sdUtils::almostEqual(rXyz[0], xyz[0]));
    REQUIRE(sdUtils::almostEqual(rXyz[1], xyz[1]));
    REQUIRE(sdUtils::almostEqual(rXyz[2], xyz[2]));
    // std::array to string
    REQUIRE( sdUtils::toString(std::array<int, 3>({{3,4,5}})) == "3 4 5");
    REQUIRE( sdUtils::toString(std::array<double, 2>({{3.3,4.4}})) == "3.3 4.4");
    
    // single arithmatic to string
    REQUIRE( sdUtils::toString(32) == "32");
    REQUIRE( sdUtils::toString(42.31) == "42.31");
    REQUIRE( sdUtils::toString(42.31f) == "42.31");
    
    // bool to string
    REQUIRE( sdUtils::toString(true) == "true");
    REQUIRE( sdUtils::toString(false) == "false");
    
    REQUIRE( sdUtils::stringTo<bool>("true") == true);
    REQUIRE( sdUtils::stringTo<bool>("false") == false);
    
    REQUIRE( sdUtils::stringTo<int>("43") == 43);
    REQUIRE( sdUtils::stringTo<double>("423.1") == 423.1);
    SECTION("int array"){
        auto array = sdUtils::stringToArray<int,3>("4 3 2");
        REQUIRE(array.at(0) == 4);
        REQUIRE(array.at(1) == 3);
        REQUIRE(array.at(2) == 2);
    }
    
    SECTION("double array"){
        auto array = sdUtils::stringToArray<double,2>("4.2 3.2");
        REQUIRE(array.at(0) == 4.2);
        REQUIRE(array.at(1) == 3.2);
    }
}


TEST_CASE("Copy sdScene", "[sdScene]"){
    sdScene sceneB;
    {
        sdScene sceneA;
        sceneA.addEntity("myEntity");
        sceneA.addEntity("yourEntity");
        sceneA.addEvent<SD_POSITION>("myEntity", 3.00, {10,20,30});
        sceneA.addEvent<SD_POSITION>("yourEntity", 3.00, {30,20,10});
        sceneB = std::move(sceneA);
        //scene A deleted here
    }

    auto myEntity =  sceneB.getEntity("myEntity");
    auto name = myEntity->getName();
    REQUIRE(name == "myEntity");
    
    auto youEntity = sceneB.getEntity("yourEntity");
    auto entityName = youEntity->getName();
    REQUIRE(entityName == "yourEntity");
    
    auto pos1 = sceneB.getEntity("myEntity")->getEvent<SD_POSITION>(3.00)->getValue();
    auto pos2 = sceneB.getEntity("yourEntity")->getEvent<SD_POSITION>(3.00)->getValue();
    
    REQUIRE(pos1[0] == 10);
    REQUIRE(pos1[1] == 20);
    REQUIRE(pos1[2] == 30);
    
    REQUIRE(pos2[0] == 30);
    REQUIRE(pos2[1] == 20);
    REQUIRE(pos2[2] == 10);
    
}


