UNAME := $(shell uname -s)
CC = g++
CFLAGS =  -O0 -arch x86_64 -arch i386
LIBS = -lstdc++
OBJECTS = *.o *.a
FILES = *.cpp
EXAMPLE_DIR = ../examples

TXML_PATH = ./libs/tinyxml2
TXML_FILE = tinyxml2.cpp 
TXML_FLAG = -arch x86_64 -arch i386 #FAT

JSON_PATH = ./libs/libjson
JSON_SOURCE_PATH = $(JSON_PATH)/_internal/Source
JSON_OBJECT_PATH = $(JSON_PATH)/Objects_static

JSON_MAKE = makefile 

INCLUDE_PATH = -I$(TXML_PATH) -I$(JSON_PATH) -I$(JSON_SOURCE_PATH)
INSTALL_PATH = /usr/local/lib

all: 
	-@make $(UNAME)

Darwin: tinyxml2.o libspatdif.dylib $(JSON_MAKE)
	install_name_tool -id @loader_path/../Resources/libspatdif.dylib libspatdif.dylib
	cp libspatdif.dylib libspatdif.dylib.dSYM/Contents/resources/DWARF/
	cp libspatdif.dylib ../examples
	cd ../examples;	make

Linux: tinyxml2.o libjson.a libspatdif.so
	cd ../examples; make 

tinyxml2.o : $(TXML_PATH)/$(TXML_FILES)
	$(CC) $(TXML_FLAG) -c $(TXML_PATH)/$(TXML_FILE)	

libjson.a : $(JSON_PATH)/$(JSON_MAKE)
	make -C $(JSON_PATH)
	cp $(JSON_PATH)/$@ ./

libspatdif.so: $(FILES)
	$(CC) $(CFLAGS) -o $@ -shared $(LIBS) $(INCLUDE_PATH) tinyxml2.o libjson.a

libspatdif.dylib: $(FILES)
	$(CC) $(CFLAGS) -o $@ -dynamiclib $(LIBS) $(INCLUDE_PATH) tinyxml2.o $(JSON_OBJECT_PATH)/*.o $(FILES)

install:
	-@make install_$(UNAME)

install_Linux:
	sudo cp libspatdif.so $(INSTALL_PATH)

install_Darwin:
	cp libspatdif.dylib $(INSTALL_PATH)


clean:	
	-@make clean_$(UNAME)

clean_Linux:
	rm *.so *.o *.a
	cd ../examples; make clean

clean_Darwin:
	rm *.dylib *.o
	cd ../examples; make clean
	cd ../src/libs/libjson ;make clean
