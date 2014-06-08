CC = g++
CFLAGS = -arch x86_64 -arch i386 
INCLUDE = -I../ -I../..
LIB_PATH = /usr/local/lib
LIBS =  -lstdc++ -lspatdif
WRAPPER = ../libspatDIFC.a
TARGETS = sdDateTest.out sdInfoTest.out sdSceneTest.out sdLoaderTest.out


all: $(TARGETS)

sdDateTest.out: sdDateTest.c
	$(CC) $(CFLAGS) $(INCLUDE) -L$(LIB_PATH) -o $@ $? $(WRAPPER) $(LIBS) 

sdInfoTest.out: sdInfoTest.c
	$(CC) $(CFLAGS) $(INCLUDE) -L$(LIB_PATH) -o $@ $? $(WRAPPER) $(LIBS) 

sdSceneTest.out: sdSceneTest.c
	$(CC) $(CFLAGS) $(INCLUDE) -L$(LIB_PATH) -o $@ $? $(WRAPPER) $(LIBS) 

sdLoaderTest.out: sdLoaderTest.c
	$(CC) $(CFLAGS) $(INCLUDE) -L$(LIB_PATH) -o $@ $? $(WRAPPER) $(LIBS) 

clean:
	rm $(TARGETS)
