CC = g++
CFLAGS = 
INCLUDE = ../src
LIB_PATH = ../src
LIBS = -lspatdif -lstdc++
LD_LIBRARY_PATH= $(LIB_PATH)
TARGETS = sdDateTest sdInfoTest sdSaverTest sdLoaderTest How_to_query

all: $(TARGETS)

sdDateTest: sdDateTest.cpp
	export LD_LIBRARY_PATH=$(LD_LIBRARY_PATH)
	$(CC) $(CFLAGS) -I$(INCLUDE) -L$(LIB_PATH) $(LIBS) -o $@ $?

sdInfoTest: sdInfoTest.cpp
	$(CC) $(CFLAGS) -I$(INCLUDE) -L$(LIB_PATH) $(LIBS) -o $@ $?

sdSaverTest: sdSaverTest.cpp
	$(CC) $(CFLAGS) -I$(INCLUDE) -L$(LIB_PATH) $(LIBS) -o $@ $?

sdLoaderTest: sdLoaderTest.cpp
	$(CC) $(CFLAGS) -I$(INCLUDE) -L$(LIB_PATH) $(LIBS) -o $@ $? 

How_to_query:  How_to_query.cpp
	$(CC) $(CFLAGS) -I$(INCLUDE) -L$(LIB_PATH) $(LIBS) -o $@ $? 

clean:
	rm $(TARGETS)