
CC = g++
CFLAGS = -g -O3 -Wall -w -dynamiclib
LIBS = -lstdc++
OBJECTS = *.o *.a
FILES = *.cpp
EXAMPLE_DIR = ../examples

TXML_PATH = ./libs/tinyxml2
TXML_FILE = tinyxml2.cpp 

JSON_PATH = ./libs/libjson
JSON_MAKE = makefile 

all: tinyxml2.o libjson.a libspatdif.dylib

tinyxml2.o : $(TXML_PATH)/$(TXML_FILES)
	$(CC) -c $(TXML_PATH)/$(TXML_FILE)
	

libjson.a : $(JSON_PATH)/$(JSON_MAKE)
	make -C $(JSON_PATH)
	cp $(JSON_PATH)/$@ ./

libspatdif.dylib: $(FILES)
	$(CC) $(CFLAGS) -I$(TXML_PATH) $(LIBS) -o $@ $(FILES) $(OBJECTS)
	cp $@ $(EXAMPLE_DIR)
clean: 
	rm *.dylib *.o *.a
	make clean -C $(JSON_PATH) 
