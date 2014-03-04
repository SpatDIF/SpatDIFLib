CC = g++
CFLAGS = -arch x86_64 -arch i386 
INCLUDE = ../src
LIB_PATH = /usr/local/lib
LIBS =  -lstdc++ -lspatdif
TARGETS = How_to_query.out FileWithExtension.out sdConverter.out sdDateTest.out\
			sdEventTest.out sdExtensionTest.out sdInfoTest.out sdLoaderTest.out sdOSCResponderTest.out\
			sdSaverTest.out sdSceneTest.out sdOSCConverterTest.out sdOSCMessageTest.out

all: $(TARGETS)

How_to_query.out:  How_to_query.cpp
	$(CC) $(CFLAGS) -I$(INCLUDE) -L$(LIB_PATH) -o $@ $? $(LIBS) 

FileWithExtension.out: FileWithExtension.cpp
	$(CC) $(CFLAGS) -I$(INCLUDE) -L$(LIB_PATH) -o $@ $? $(LIBS) 

sdConverter.out: sdConverter.cpp
	$(CC) $(CFLAGS) -I$(INCLUDE) -L$(LIB_PATH) -o $@ $? $(LIBS) 

sdDateTest.out: sdDateTest.cpp
	$(CC) $(CFLAGS) -I$(INCLUDE) -L$(LIB_PATH) -o $@ $? $(LIBS) 

sdEventTest.out: sdEventTest.cpp
	$(CC) $(CFLAGS) -I$(INCLUDE) -L$(LIB_PATH) -o $@ $? $(LIBS) 

sdExtensionTest.out:  sdExtensionTest.cpp
	$(CC) $(CFLAGS) -I$(INCLUDE) -L$(LIB_PATH) -o $@ $? $(LIBS) 

sdInfoTest.out: sdInfoTest.cpp
	$(CC) $(CFLAGS) -I$(INCLUDE) -L$(LIB_PATH) -o $@ $? $(LIBS) 

sdLoaderTest.out: sdLoaderTest.cpp
	$(CC) $(CFLAGS) -I$(INCLUDE) -L$(LIB_PATH) -o $@ $? $(LIBS) 

sdOSCResponderTest.out: sdOSCResponderTest.cpp
	$(CC) $(CFLAGS) -I$(INCLUDE) -L$(LIB_PATH) -o $@ $? $(LIBS) 

sdOSCConverterTest.out: sdOSCConverterTest.cpp
	$(CC) $(CFLAGS) -I$(INCLUDE) -L$(LIB_PATH) -o $@ $? $(LIBS) 

sdOSCMessageTest.out: sdOSCMessageTest.cpp
	$(CC) $(CFLAGS) -I$(INCLUDE) -L$(LIB_PATH) -o $@ $? $(LIBS)

sdSaverTest.out: sdSaverTest.cpp
	$(CC) $(CFLAGS) -I$(INCLUDE) -L$(LIB_PATH) -o $@ $? $(LIBS) 

sdSceneTest.out: sdSceneTest.cpp
	$(CC) $(CFLAGS) -I$(INCLUDE) -L$(LIB_PATH) -o $@ $? $(LIBS) 

clean:
	rm $(TARGETS)
	rm *.dylib
