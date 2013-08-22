CC = c++
CFLAGS =  -O3 -Wall -w 
LIBS = -lstdc++ -lspatdif
LIB_PATH = ../src
PATH = ../src

FILES = sdLoaderTest sdSaverTest.cpp sdDateTest.cpp sdInfoTest.cpp sdSceneTest.cpp How_to_query.cpp
TARGETS = sdLoaderTest sdSaverTest sdDateTest sdInfoTest sdSceneTest How_to_query

all: $(TARGETS)

sdLoaderTest: sdLoaderTest.cpp
	$(CC) $(CFLAGS) -I$(PATH) -L$(LIB_PATH) $(LIBS) -o $@ $?

sdSaverTest: sdSaverTest.cpp
	$(CC) $(CFLAGS) -I$(PATH) -L$(LIB_PATH) $(LIBS) -o $@ $?


sdDateTest: sdDateTest.cpp
	$(CC) $(CFLAGS) -I$(PATH) -L$(LIB_PATH) $(LIBS) -o $@ $?

sdInfoTest: sdInfoTest.cpp
	$(CC) $(CFLAGS) -I$(PATH) -L$(LIB_PATH) $(LIBS) -o $@ $?

sdSceneTest: sdSceneTest.cpp
	$(CC) $(CFLAGS) -I$(PATH) -L$(LIB_PATH) $(LIBS) -o $@ $?

How_to_query: How_to_query.cpp
	$(CC) $(CFLAGS) -I$(PATH) -L$(LIB_PATH) $(LIBS) -o $@ $?

clean: 
	rm $(TARGETS)