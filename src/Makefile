UNAME := $(shell uname -s)
CC = g++
CFLAGS = -O3 -Wall -w -shared
LIBS = -lstdc++
OBJECTS = *.o *.a
FILES = *.cpp
EXAMPLE_DIR = ../examples

TXML_PATH = ./libs/tinyxml2
TXML_FILE = tinyxml2.cpp 

JSON_PATH = ./libs/libjson
JSON_SOURCE_PATH = $(JSON_PATH)/_internal/Source
JSON_MAKE = makefile 

INCLUDE_PATH = -I$(TXML_PATH) -I$(JSON_PATH) -I$(JSON_SOURCE_PATH)

all: 
	-@make $(UNAME)

MacOSX: tinyxml2.o libjson.a libspatdif.dylib

Linux: tinyxml2.o libjson.a libspatdif.so

tinyxml2.o : $(TXML_PATH)/$(TXML_FILES)
	$(CC) -c $(TXML_PATH)/$(TXML_FILE)	

libjson.a : $(JSON_PATH)/$(JSON_MAKE)
	make -C $(JSON_PATH)
	cp $(JSON_PATH)/$@ ./

libspatdif.so: $(FILES)
	$(CC) -o $@ -shared $(LIBS) $(INCLUDE_PATH)  tinyxml2.o libjson.a $(FILES)

clean: 
	rm *.dylib *.so *.o *.a
	make clean -C $(JSON_PATH) 
