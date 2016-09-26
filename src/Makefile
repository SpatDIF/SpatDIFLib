UNAME := $(shell uname -s)
CC = clang++
CFLAGS =  -std=c++11 -O0 -arch x86_64 -arch i386
LIBS = -lstdc++
OBJECTS = *.o *.a
FILES = *.cpp

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

Darwin: tinyxml2.o libjson libspatdif.dylib 
	# cp libspatdif.dylib ../examples
	# install_name_tool -id @loader_path/../Resources/libspatdif.dylib libspatdif.dylib # PD external needs this
	# install_name_tool -id @loader_path/../../spatdiflib/src/libspatdif.dylib libspatdif.dylib # Max externals needs this
	# cp libspatdif.dylib libspatdif.dylib.dSYM/Contents/resources/DWARF/
	ar rcs libSpatDIF.a *.o
	rm *.o

Linux: tinyxml2.o libjson.a libspatdif.so
	cd ../examples; make 

tinyxml2.o: $(TXML_PATH)/$(TXML_FILES)
	$(CC) $(TXML_FLAG) -c $(TXML_PATH)/$(TXML_FILE)	

libjson: $(JSON_PATH)/$(JSON_MAKE)
	make -C $(JSON_PATH)

libspatdif.dylib: $(FILES)
	$(CC) $(CFLAGS) $(LIBS) $(INCLUDE_PATH) -c $(FILES)
	$(CC) $(CFLAGS) -o $@ -dynamiclib -flat_namespace *.o $(JSON_OBJECT_PATH)/*.o 

install:
	-@make install_$(UNAME)

install_Linux:
	sudo cp libspatdif.so $(INSTALL_PATH)

install_Darwin:
	install_name_tool -id $(INSTALL_PATH)/libspatdif.dylib libspatdif.dylib
	sudo cp libspatdif.dylib $(INSTALL_PATH)

clean:	
	-@make clean_$(UNAME)

clean_Linux:
	rm *.so *.o
	cd ../examples; make clean

clean_Darwin:
	rm *.dylib *.o
	cd ../src/libs/libjson ;make clean
