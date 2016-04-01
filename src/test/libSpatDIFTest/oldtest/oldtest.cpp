
#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "sdMain.h"
#include <array>
using namespace std;



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
        info.setTitle(string("My Wonderful Piece"));
        info.setDuration(12.0);
        
        REQUIRE( info.getAuthor() == "John");
        REQUIRE( info.getHost() == "sdInfoTest");
        REQUIRE( info.getDateAsString() == "2000-1-1");
        REQUIRE( info.getSession() == "1.1");
        REQUIRE( info.getLocation() == "ZHDK");
        REQUIRE( info.getAnnotation() == "this is a test");
        REQUIRE( info.getTitle() == "My Wonderful Piece");
        REQUIRE( info.getDuration() == 12.0);
    }
    {
        // set at once with strings and sdDate
        sdInfo info(string("Tom"), string("sdInfoTest"), sdDate(string("2012-04-03")), string("1.2"), string("ESB"), string("this is second test"),string(""));
        
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

TEST_CASE("Speaker Setup"){
    ifstream ifs("/Users/chikashi/Development/spatdiflib/src/test/libSpatDIFTest/TestCode/stereo-playback.xml");
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
    sdEntity *leftSpeaker = myScene.getEntity("left");
    sdEntity *rightSpeaker = myScene.getEntity("right");

    auto positionXYZ = leftSpeaker->getMeta<SD_POSITION>()->getValue();
    auto aed = xyzToAed(positionXYZ);
    REQUIRE(almostEqual(aed[0], -30.0));
    REQUIRE(almostEqual(aed[1] , 0.0));
    REQUIRE(almostEqual(aed[2] , 2.0));
    auto type = leftSpeaker->getMeta<SD_TYPE>()->getValue();
    REQUIRE(type == EType::SD_LOUDSPEAKER);

    auto physicalChannel = rightSpeaker->getMeta<SD_HARDWARE_OUT_PHYSICAL_CHANNEL>()->getValue();
    REQUIRE(physicalChannel == 2);
    
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
    
    
    sdInfo info = myScene.getInfo();
    REQUIRE(info.getDuration() == 650.3);
    REQUIRE(info.getTitle() == "My Master Piece");
    
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
    //dumpBytes(ic);
    REQUIRE(ioriginal == converter.blockTo<int>(ic));
    
    float foriginal = 1.234;
    vector<unsigned char> fc = converter.toBlock(foriginal);
    //dumpBytes(fc);
    REQUIRE(foriginal == converter.blockTo<float>(fc));
    
    string soriginal = "hello";
    vector<unsigned char> sc = converter.toBlock(soriginal); // here call it directly
    //dumpBytes(sc);
    REQUIRE(soriginal == converter.blockTo<string>(sc));
    
}

TEST_CASE("FileExtension"){
    using namespace std;
    ifstream ifsXML("/Users/chikashi/Development/spatdiflib/src/test/libSpatDIFTest/TestCode/turenas_insect.xml");
    string xmlString;
    
    if (ifsXML.is_open())
    {
        while ( ifsXML.good() )
        {
            string str;
            getline(ifsXML,str);
            xmlString.append(str);
        }
        ifsXML.close();
    }
    
    sdScene scene = sdLoader::sceneFromXML(xmlString);
    // here how you get a pointer to two source entities
    auto insect = scene.getEntity("insect");
    auto elephant = scene.getEntity("elephant");
    
    // now you can ask these entities about their attached extensions
    auto insectMediaLocationEvent = insect->getFirstEvent<SD_MEDIA_LOCATION>();
    auto elephantMediaLocationEvent = elephant->getFirstEvent<SD_MEDIA_LOCATION>();
    
    REQUIRE(insect->getName() == "insect");
    REQUIRE(insectMediaLocationEvent->getValueAsString()  == "insect_simulated.aiff");
    REQUIRE(insectMediaLocationEvent->getTime() == 0.0);
    
    REQUIRE(elephant->getName() == "elephant");
    REQUIRE(elephantMediaLocationEvent->getValueAsString() == "elephant_simulated.aiff");
    REQUIRE(elephantMediaLocationEvent->getTime() == 0.0);
    
}

TEST_CASE("sdSaverTest"){
    sdInfo info(string("chikashi miyama"), string("sdSaverTest"), string("2013-08-04"), string("Cologne"), string("1.2"), string("this is a test"), string("My Second Master Piece"), 645.32);
    sdScene scene(info);
    
    scene.addExtension(EExtension::SD_MEDIA);
    //the scene has one entities
    auto myEntity = scene.addEntity("myEntity", EKind::SD_SOURCE);
    auto yourEntity = scene.addEntity("yourEntity", EKind::SD_SOURCE);
    myEntity->addEvent<SD_POSITION>(9.15 , {1.0,2.0,3.0});
    myEntity->addEvent<SD_ORIENTATION>(2.12 , {0.0,1.0,2.0});
    yourEntity->addEvent<SD_POSITION>(4.2, {5.0,4.0,3.0});
    yourEntity->addEvent<SD_MEDIA_LOCATION>(5.0, "/tmp/tmpaudio.wav");
    string generatedString = sdSaver::XMLFromScene(&scene);
    //cout << generatedString;
    scene.setOrdering(EOrdering::SD_TRACK);
    generatedString = sdSaver::XMLFromScene(&scene);
    //cout << generatedString;
}

TEST_CASE("sdSceneTest"){
    
    sdInfo info(
                string("Chikashi"),
                string("sdSceneTest"),
                string("2013-08-04"),
                string("1.2"),
                string("Cologne"),
                string("this a scene test"));
    
    sdScene scene(info);
    scene.addExtension(EExtension::SD_MEDIA);
    REQUIRE(scene.getNumberOfActivatedExtensions()==1);
    auto myEntity = scene.addEntity("myEntity");
    myEntity->addEvent(string("1.0"), string("position"), string("0.0 0.1 0.2"));
    myEntity->addEvent<SD_POSITION>(2.0, {0.0, 0.1, 0.2});
    myEntity->addEvent<SD_MEDIA_GAIN>(2.0, 0.5523);
    
    auto yourEntity = scene.addEntity("yourEntity"); // spawn an entity
    REQUIRE( scene.getNumberOfEntities() == 2);
    auto providedEntity = scene.getEntity("myEntity");
    REQUIRE(providedEntity->getNumberOfEvents() == 3);
    auto positionEvent = providedEntity->getEvent<SD_POSITION>(0.2);
    REQUIRE(!positionEvent);
    auto events = scene.getNextEventsFromAllEntities(0.4);
    
    REQUIRE(events.size() == 1);
    REQUIRE( scene.getNextEventTime(0.4).first == 1.0);
    REQUIRE( scene.getDeltaTimeToNextEvent(0.4).first == (1.0-0.4));
    
    auto duplicated = scene.addEntity("myEntity"); //if the name of existing entity, returns pointer to existing one
    REQUIRE( scene.getNumberOfEntities() == 2); // not increased
    
}

TEST_CASE( "sdEventTest"){
    
    sdScene scene;
    auto  myEntity = scene.addEntity("myEntity");
    auto  yourEntity = scene.addEntity("yourEntity");
    
    myEntity->addEvent<SD_POSITION>(5.0,{0.0, 1.0, 2.0});
    auto myEvent = myEntity->getEvent<SD_POSITION>(5.0);
    REQUIRE(myEvent->getValueAsString() == "0 1 2");
    
    myEntity->addEvent<SD_POSITION>(5.0,  {3.0, 2.0, 1.0}); // overwritten
    
    REQUIRE(myEntity->getEvent<SD_POSITION>(5.0)->getValueAsString() == "3 2 1");
    myEntity->addEvent<SD_ORIENTATION>(5.0, {1.0, 2.0, 3.0});
    
    yourEntity->addEvent<SD_POSITION>(10.0, {5.0, 4.0, 3.0});
    yourEntity->addEvent<SD_ORIENTATION>(5.0, {10.0, 9.0, 8.0});
    
    {
        REQUIRE(myEntity->getEvent<SD_POSITION>(5.0)->getValueAsString() == "3 2 1");
        auto events = myEntity->getEvents(5.0);
        REQUIRE(events.size() == 2);
        
        auto allEvents = scene.getEventsFromAllEntities(5.0);
        REQUIRE(allEvents.size() == 3);
    }
    {
        auto nextEvent = myEntity->getNextEvent<SD_POSITION>(3.0);
        auto nextEvents = myEntity->getNextEvents(3.0);
        
        REQUIRE(nextEvent->getValueAsString() == "3 2 1");
        REQUIRE(nextEvents.size() == 2); // one orientation + one position
        auto allEvents = scene.getNextEventsFromAllEntities(3.0);
        REQUIRE(allEvents.size() == 3);
        
    }
    {
        auto prevEvent = myEntity->getPreviousEvent<SD_POSITION>(7.0);
        auto prevEvents = myEntity->getPreviousEvents(7.0);
        
        REQUIRE(prevEvent->getValueAsString() == "3 2 1");
        REQUIRE(prevEvents.size() == 2);
        auto allEvents = scene.getPreviousEventsFromAllEntities(7.0);
        REQUIRE(allEvents.size() == 3);
    }
    {
        auto firstEvent = myEntity->getFirstEvent<SD_POSITION>();
        REQUIRE(firstEvent->getTime() == 5.0);
        REQUIRE(firstEvent->getValueAsString() == "3 2 1");
        auto firstEvents = myEntity->getFirstEvents();
        REQUIRE(firstEvents.size() == 2);
        auto allFirstEvents = scene.getFirstEventsFromAllEntities();
        REQUIRE(allFirstEvents.size() == 3);
    }
    {
        auto lastEvent = myEntity->getLastEvent<SD_POSITION>();
        REQUIRE(lastEvent->getTime() == 5.0);
        REQUIRE(lastEvent->getValueAsString() == "3 2 1");
        auto lastEvents = myEntity->getLastEvents();
        REQUIRE(lastEvents.size() == 2);
        auto allLastEvents  = scene.getLastEventsFromAllEntities();
        REQUIRE(allLastEvents.size() == 1);
    }
    {
        auto eventsInRangeWithDescriptor = myEntity->getEvents(3.0, 7.0, SD_POSITION);
        REQUIRE(eventsInRangeWithDescriptor.size() == 1);
        auto eventsInRange = myEntity->getEvents(3.0, 7.0);
        REQUIRE(eventsInRange.size() == 2);
        auto allEventsInRange = scene.getEventsFromAllEntities(3.0, 9.0);
        REQUIRE(allEventsInRange.size() == 3);
    }
}

TEST_CASE("sdOSCResponder"){
    sdScene scene;
    sdOSCResponder oscResponder(&scene);
    vector<sdOSCMessage> returnedMessageVector;
    
    {
        sdOSCMessage setQueryTime("/spatdifcmd/setQueryTime");
        setQueryTime.appendArgument(1.52f);
        oscResponder.forwardOSCMessage(setQueryTime);
        sdOSCMessage getQueryTime("/spatdifcmd/getQueryTime");
        returnedMessageVector = oscResponder.forwardOSCMessage(getQueryTime);
        REQUIRE(returnedMessageVector[0].getArgument<float>(0) == 1.52f);
    }
    {
        sdOSCMessage setWriteTime("/spatdifcmd/setWriteTime");
        setWriteTime.appendArgument(3.14f);
        oscResponder.forwardOSCMessage(setWriteTime);
        sdOSCMessage getWriteTime("/spatdifcmd/getWriteTime");
        returnedMessageVector = oscResponder.forwardOSCMessage(getWriteTime);
        REQUIRE(returnedMessageVector[0].getArgument<float>(0)  == 3.14f);
    }
    
    {
        sdOSCMessage setInterval("/spatdifcmd/setInterval");
        setInterval.appendArgument(2.25f);
        oscResponder.forwardOSCMessage(setInterval);
        sdOSCMessage getInterval("/spatdifcmd/getInterval");
        returnedMessageVector = oscResponder.forwardOSCMessage(getInterval);
        REQUIRE(returnedMessageVector[0].getArgument<float>(0) == 2.25f);
    }
    {
        sdOSCMessage setOrderingToTrack("/spatdifcmd/setOrdering");
        setOrderingToTrack.appendArgument(std::string("track"));
        oscResponder.forwardOSCMessage(setOrderingToTrack);
        
        sdOSCMessage getOrdering("/spatdifcmd/getOrdering");
        returnedMessageVector = oscResponder.forwardOSCMessage(getOrdering);
        REQUIRE(returnedMessageVector[0].getArgument<std::string>(0) == "track");
        
        sdOSCMessage setOrderingToTime("/spatdifcmd/setOrdering");
        setOrderingToTime.appendArgument(std::string("time"));
        oscResponder.forwardOSCMessage(setOrderingToTime);
        
        returnedMessageVector = oscResponder.forwardOSCMessage(getOrdering);
        REQUIRE(returnedMessageVector[0].getArgument<std::string>(0) == "time");
        
    }
    {
        sdOSCMessage addTest1Entity("/spatdifcmd/addEntity");
        addTest1Entity.appendArgument(std::string("test1Entity"));
        oscResponder.forwardOSCMessage(addTest1Entity);
        
        sdOSCMessage addTest2Entity("/spatdifcmd/addEntity");
        addTest2Entity.appendArgument(std::string("test2Entity"));
        oscResponder.forwardOSCMessage(addTest2Entity);
        
        sdOSCMessage getNumberOfEntities("/spatdifcmd/getNumberOfEntities");
        returnedMessageVector = oscResponder.forwardOSCMessage(getNumberOfEntities);
        REQUIRE(returnedMessageVector[0].getArgument<int>(0) == 2);
        
        sdOSCMessage getEntityNames("/spatdifcmd/getEntityNames");
        returnedMessageVector = oscResponder.forwardOSCMessage(getEntityNames);
        REQUIRE( returnedMessageVector[0].getArgument<std::string>(0) == "test1Entity");
        REQUIRE( returnedMessageVector[0].getArgument<std::string>(1) == "test2Entity");
        
        
        sdOSCMessage removeTest1Entity("/spatdifcmd/removeEntity");
        removeTest1Entity.appendArgument(std::string("test1Entity"));
        returnedMessageVector = oscResponder.forwardOSCMessage(removeTest1Entity);
        returnedMessageVector = oscResponder.forwardOSCMessage(getEntityNames);
        REQUIRE(returnedMessageVector[0].getArgument<std::string>(0) == "test2Entity");
        
        sdOSCMessage removeAllEntities("/spatdifcmd/removeAllEntities");
        oscResponder.forwardOSCMessage(removeAllEntities);
        returnedMessageVector = oscResponder.forwardOSCMessage(getNumberOfEntities);
        REQUIRE( returnedMessageVector[0].getArgument<int>(0) == 0);
        
    }
    {
        // both query and write at 10.0
        sdOSCMessage setWriteTime("/spatdifcmd/setWriteTime");
        setWriteTime.appendArgument(10.0f);
        oscResponder.forwardOSCMessage(setWriteTime);
        sdOSCMessage setQueryTime("/spatdifcmd/setQueryTime");
        setQueryTime.appendArgument(10.0f);
        oscResponder.forwardOSCMessage(setQueryTime);
        
        sdOSCMessage addEntity("/spatdifcmd/addEntity");
        addEntity.appendArgument(std::string("myEntity"));
        oscResponder.forwardOSCMessage(addEntity);
        
        sdOSCMessage setPosition("/spatdifcmd/setPosition");
        setPosition.appendArgument(std::string("myEntity"));
        setPosition.appendArgument(1.0f);
        setPosition.appendArgument(2.0f);
        setPosition.appendArgument(3.0f);
        oscResponder.forwardOSCMessage(setPosition);
        
        sdOSCMessage getPosition("/spatdifcmd/getPosition");
        getPosition.appendArgument(std::string("myEntity"));
        returnedMessageVector = oscResponder.forwardOSCMessage(getPosition);
        REQUIRE(returnedMessageVector[0].getAllArgumentsAsString() == "myEntity 1 2 3");
    }
    {
        sdOSCMessage setQueryTime("/spatdifcmd/setQueryTime");
        setQueryTime.appendArgument(5.0f);
        oscResponder.forwardOSCMessage(setQueryTime);
        
        sdOSCMessage getNextPosition("/spatdifcmd/getNextPosition");
        getNextPosition.appendArgument(std::string("myEntity"));
        returnedMessageVector = oscResponder.forwardOSCMessage(getNextPosition);
        REQUIRE(returnedMessageVector[0].getAllArgumentsAsString() == "myEntity 1 2 3");
        
    }
    {
        // search event with previous command
        sdOSCMessage setQueryTime("/spatdifcmd/setQueryTime");
        setQueryTime.appendArgument(15.0f);
        oscResponder.forwardOSCMessage(setQueryTime);
        
        sdOSCMessage getPreviousPosition("/spatdifcmd/getPreviousPosition");
        getPreviousPosition.appendArgument(std::string("myEntity"));
        returnedMessageVector = oscResponder.forwardOSCMessage(getPreviousPosition);
        REQUIRE(returnedMessageVector[0].getAllArgumentsAsString() == "myEntity 1 2 3");
    }
    {
        sdOSCMessage setOrientation("/spatdifcmd/setOrientation");
        setOrientation.appendArgument(std::string("myEntity"));
        
        sdOSCMessage setWriteTime("/spatdifcmd/setWriteTime");
        setWriteTime.appendArgument(10.0f);
        oscResponder.forwardOSCMessage(setWriteTime);
        sdOSCMessage setQueryTime("/spatdifcmd/setQueryTime");
        setQueryTime.appendArgument(10.0f);
        oscResponder.forwardOSCMessage(setQueryTime);
        
        setOrientation.appendArgument(1.0f);
        setOrientation.appendArgument(2.0f);
        setOrientation.appendArgument(3.0f);
        oscResponder.forwardOSCMessage(setOrientation);
        
        sdOSCMessage getOrientation("/spatdifcmd/getOrientation");
        getOrientation.appendArgument(std::string("myEntity"));
        returnedMessageVector = oscResponder.forwardOSCMessage(getOrientation);
        REQUIRE( returnedMessageVector[0].getAllArgumentsAsString() == "myEntity 1 2 3");
    }
    {
        sdOSCMessage setPresent("/spatdifcmd/setPresent");
        setPresent.appendArgument(std::string("myEntity"));
        setPresent.appendArgument<int>(1);
        oscResponder.forwardOSCMessage(setPresent);
        
        sdOSCMessage getPresent("/spatdifcmd/getPresent");
        getPresent.appendArgument(std::string("myEntity"));
        returnedMessageVector = oscResponder.forwardOSCMessage(getPresent);
        REQUIRE( returnedMessageVector[0].getAllArgumentsAsString() == "myEntity 1");
    }
    {
        sdOSCMessage setQueryTime("/spatdifcmd/setQueryTime");
        setQueryTime.appendArgument(5.0f);
        oscResponder.forwardOSCMessage(setQueryTime);
        
        sdOSCMessage setInterval("/spatdifcmd/setInterval");
        setInterval.appendArgument(10.0f);
        oscResponder.forwardOSCMessage(setInterval);
        
        sdOSCMessage getEventsSetsFromAllEntities("/spatdifcmd/getEventSetsFromAllEntities");
        returnedMessageVector = oscResponder.forwardOSCMessage(getEventsSetsFromAllEntities);
        REQUIRE( returnedMessageVector.size() == 3);
        
    }
    {
        sdOSCMessage addExtension("/spatdifcmd/addExtension");
        addExtension.appendArgument(std::string("media"));
        oscResponder.forwardOSCMessage(addExtension);
        
        sdOSCMessage getNumberOfActivatedExtensions("/spatdifcmd/getNumberOfActivatedExtensions");
        returnedMessageVector = oscResponder.forwardOSCMessage(getNumberOfActivatedExtensions);
        REQUIRE(returnedMessageVector[0].getAllArgumentsAsString() == "1");
        
        sdOSCMessage setWriteTime("/spatdifcmd/setWriteTime");
        setWriteTime.appendArgument(15.0f);
        oscResponder.forwardOSCMessage(setWriteTime);
        
        sdOSCMessage setQueryTime("/spatdifcmd/setQueryTime");
        setQueryTime.appendArgument(15.0f);
        oscResponder.forwardOSCMessage(setQueryTime);
        
        sdOSCMessage setID("/spatdifcmd/media/setID");
        setID.appendArgument(std::string("myEntity"));
        setID.appendArgument(std::string("piano"));
        oscResponder.forwardOSCMessage(setID);
        
        sdOSCMessage setType("/spatdifcmd/media/setType");
        setType.appendArgument(std::string("myEntity"));
        setType.appendArgument(std::string("file"));
        oscResponder.forwardOSCMessage(setType);
        
        sdOSCMessage setLocation("/spatdifcmd/media/setLocation");
        setLocation.appendArgument("myEntity");
        setLocation.appendArgument("/Users/spat/Music/piano.aif");
        oscResponder.forwardOSCMessage(setLocation);
        
        sdOSCMessage setChannel("/spatdifcmd/media/setChannel");
        setChannel.appendArgument(std::string("myEntity"));
        setChannel.appendArgument(1);
        oscResponder.forwardOSCMessage(setChannel);
        
        sdOSCMessage setTimeOffset("/spatdifcmd/media/setTimeOffset");
        setTimeOffset.appendArgument(std::string("myEntity"));
        setTimeOffset.appendArgument(12.4f);
        oscResponder.forwardOSCMessage(setTimeOffset);
        
        sdOSCMessage setGain("/spatdifcmd/media/setGain");
        setGain.appendArgument(std::string("myEntity"));
        setGain.appendArgument(0.5f);
        oscResponder.forwardOSCMessage(setGain);
        
        sdOSCMessage getEventsSetsFromAllEntities("/spatdifcmd/getEventSetsFromAllEntities");
        returnedMessageVector = oscResponder.forwardOSCMessage(getEventsSetsFromAllEntities);
        REQUIRE(returnedMessageVector.size() == 5);
        
    }
    {
        sdOSCMessage setUnknown("/spatdifcmd/setPosition");
        setUnknown.appendArgument(std::string("unknownEntity"));
        setUnknown.appendArgument(1.0);
        setUnknown.appendArgument(2.0);
        setUnknown.appendArgument(3.0);
        oscResponder.forwardOSCMessage(setUnknown);
        // ignored
        
        sdOSCMessage getEntityNames("/spatdifcmd/getNumberOfEntities");
        returnedMessageVector = oscResponder.forwardOSCMessage(getEntityNames);
        REQUIRE(returnedMessageVector[0].getArgument<int>(0) == 1);
        
    }
    {
        sdOSCMessage returnedMessage;
        sdOSCMessage setAuthor("/spatdifcmd/setAuthor");
        setAuthor.appendArgument(std::string("Chikashi Miyama"));
        oscResponder.forwardOSCMessage(setAuthor);
        
        sdOSCMessage setHost("/spatdifcmd/setHost");
        setHost.appendArgument(std::string("Some computer"));
        oscResponder.forwardOSCMessage(setHost);
        
        sdOSCMessage setDate("/spatdifcmd/setDate");
        setDate.appendArgument(std::string("2014-3-7"));
        oscResponder.forwardOSCMessage(setDate);
        
        sdOSCMessage setLocation("/spatdifcmd/setLocation");
        setLocation.appendArgument(std::string("ICST, Zürich, Switzerland"));
        oscResponder.forwardOSCMessage(setLocation);
        
        sdOSCMessage setSession("/spatdifcmd/setSession");
        setSession.appendArgument(std::string("1.1"));
        oscResponder.forwardOSCMessage(setSession);
        
        sdOSCMessage setAnnotation("/spatdifcmd/setAnnotation");
        setAnnotation.appendArgument(std::string("This is a test with OSC messages"));
        oscResponder.forwardOSCMessage(setAnnotation);
        
        sdOSCMessage getAuthor("/spatdifcmd/getAuthor");
        returnedMessageVector = oscResponder.forwardOSCMessage(getAuthor);
        REQUIRE(returnedMessageVector[0].getAllArgumentsAsString() == "Chikashi Miyama");
        
        sdOSCMessage getHost("/spatdifcmd/getHost");
        returnedMessageVector = oscResponder.forwardOSCMessage(getHost);
        REQUIRE(returnedMessageVector[0].getAllArgumentsAsString() == "Some computer");;
        
        sdOSCMessage getDate("/spatdifcmd/getDate");
        returnedMessageVector = oscResponder.forwardOSCMessage(getDate);
        REQUIRE(returnedMessageVector[0].getAllArgumentsAsString() == "2014-3-7");
        
        sdOSCMessage getLocation("/spatdifcmd/getLocation");
        returnedMessageVector = oscResponder.forwardOSCMessage(getLocation);
        REQUIRE(returnedMessageVector[0].getAllArgumentsAsString() == "ICST, Zürich, Switzerland");
        
        sdOSCMessage getSession("/spatdifcmd/getSession");
        returnedMessageVector = oscResponder.forwardOSCMessage(getSession);
        REQUIRE(returnedMessageVector[0].getAllArgumentsAsString() == "1.1");
        
        sdOSCMessage getAnnotation("/spatdifcmd/getAnnotation");
        returnedMessageVector = oscResponder.forwardOSCMessage(getAnnotation);
        REQUIRE(returnedMessageVector[0].getAllArgumentsAsString() == "This is a test with OSC messages");
    }
    
}
