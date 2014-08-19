CC = g++
CFLAGS = -arch x86_64 -arch i386 
INCLUDE = ../src
LIB_PATH = /usr/local/lib
LIBS =  -lstdc++ -lspatdif
SRC = $(shell echo *.cpp)
TARGETS = $(basename $(SRC))

all: $(TARGETS)


$(TARGETS):
	@echo "building $@"
	$(CC) $(CFLAGS) -I$(INCLUDE) -L$(LIB_PATH) $(LIBS) -o $@ $@.cpp

clean:
	rm $(TARGETS)
	rm *.dylib
