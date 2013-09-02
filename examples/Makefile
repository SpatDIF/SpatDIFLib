CC = g++
CFLAGS = 
INCLUDE = ../src
LIB_PATH = ../src
LIBS =  -lstdc++ -lspatdif
TARGETS = sdDateTest sdInfoTest sdSaverTest sdLoaderTest How_to_query

all: $(TARGETS)

sdDateTest: sdDateTest.cpp
	$(CC) $(CFLAGS) -I$(INCLUDE) -L$(LIB_PATH) -o $@ $? $(LIBS)

sdInfoTest: sdInfoTest.cpp
	$(CC) $(CFLAGS) -I$(INCLUDE) -L$(LIB_PATH) -o $@ $? $(LIBS) 

sdSaverTest: sdSaverTest.cpp
	$(CC) $(CFLAGS) -I$(INCLUDE) -L$(LIB_PATH) -o $@ $? $(LIBS) 

sdLoaderTest: sdLoaderTest.cpp
	$(CC) $(CFLAGS) -I$(INCLUDE) -L$(LIB_PATH) -o $@ $? $(LIBS)  

How_to_query:  How_to_query.cpp
	$(CC) $(CFLAGS) -I$(INCLUDE) -L$(LIB_PATH) -o $@ $? $(LIBS) 

clean:
	rm $(TARGETS)