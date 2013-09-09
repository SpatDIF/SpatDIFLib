UNAME := $(shell uname -s)
CC = g++
CFLAGS =  -g -O0
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
INSTALL_PATH = /usr/local/lib

all: 
	-@make $(UNAME)

Darwin: tinyxml2.o libjson.a libspatdif.dylib
	cp libspatdif.dylib ../examples

Linux: tinyxml2.o libjson.a libspatdif.so

tinyxml2.o : $(TXML_PATH)/$(TXML_FILES)
	$(CC) -c $(TXML_PATH)/$(TXML_FILE)	

libjson.a : $(JSON_PATH)/$(JSON_MAKE)
	make -C $(JSON_PATH)
	cp $(JSON_PATH)/$@ ./

libspatdif.so: $(FILES)
	$(CC) $(CFLAGS) -o $@ -shared $(LIBS) $(INCLUDE_PATH) tinyxml2.o $(FILES) libjson.a

libspatdif.dylib: $(FILES)
	$(CC) $(CFLAGS) -o $@ -dynamiclib $(LIBS) $(INCLUDE_PATH) tinyxml2.o libjson.a $(FILES)
	

install:
	-@make install_$(UNAME)

install_Linux:
	sudo cp libspatdif.so $(INSTALL_PATH)

install_Darwin:
	cp libspatdif.dylib $(INSTALL_PATH)

clean:
	rm *.dylib *.so *.o *.a
	make clean -C $(JSON_PATH) 

