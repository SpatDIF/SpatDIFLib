CC = clang++
CFLAGS =  -O0 -arch i386 -arch x86_64 -g
INCLUDE = ../src
JSON_PATH = $(INCLUDE)/libs/libjson
JSON_CORE_PATH = $(INCLUDE)/libs/libjson/_internal/Source

TXML_PATH = $(INCLUDE)/libs/tinyxml2
TXML_FILE = tinyxml2.cpp 
TXML_FLAG = -arch x86_64 -arch i386 #FAT

LIBS =  -lstdc++
SRC = $(shell echo *.cpp)
TARGETS = sdDateTest sdInfoTest

all: tinyxml2.o libjson $(TARGETS)

tinyxml2.o: $(TXML_PATH)/$(TXML_FILES)
	$(CC) $(TXML_FLAG) -c $(TXML_PATH)/$(TXML_FILE)	

libjson: $(JSON_PATH)/$(JSON_MAKE)
	make -C $(JSON_PATH)

sdDateTest:
	$(CC) $(CFLAGS) -I$(INCLUDE) -I$(JSON_PATH) -I$(JSON_CORE_PATH) -I$(TXML_PATH) $(LIBS) -o $@ $@.cpp ../src/*.cpp $(JSON_PATH)/Objects_static/*.o tinyxml2.o

sdInfoTest:
	$(CC) $(CFLAGS) -I$(INCLUDE) -I$(JSON_PATH) -I$(JSON_CORE_PATH) -I$(TXML_PATH) $(LIBS) -o $@ $@.cpp ../src/*.cpp $(JSON_PATH)/Objects_static/*.o tinyxml2.o

clean:
	rm $(TARGETS)
	rm *.out
	rm -rf *.dSYM
	rm *.dylib
