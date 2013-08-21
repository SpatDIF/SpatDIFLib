
CC = g++
CFLAGS = -g -O3 -Wall -w -dynamiclib
LIBS = -lstdc++
OBJECTS = *.o
FILES = *.cpp
EXAMPLE_DIR = ../examples

TXML_PATH = ./libs/tinyxml2
TXML_FILE = tinyxml2.cpp 

all: tinyxml2.o libspatdif.dylib

tinyxml2.o : $(TXML_PATH)/$(TXML_FILES)
	$(CC) -c $(TXML_PATH)/$(TXML_FILE)

libspatdif.dylib: $(FILES)
	$(CC) $(CFLAGS) -I$(TXML_PATH) $(LIBS) -o $@ $(FILES) $(OBJECTS)
	cp $@ $(EXAMPLE_DIR)
clean: 
	rm *.dylib *.o