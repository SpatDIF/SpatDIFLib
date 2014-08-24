CC = clang++
CFLAGS = -arch x86_64 -arch i386 -g
INCLUDE = ../src
LIB_PATH = /usr/local/lib
LIBS =  -lstdc++ -lspatdif
SRC = $(shell echo *.cpp)
TARGETS = $(basename $(SRC))

all: $(TARGETS)

$(TARGETS):
	@echo "building $@"
	$(CC) $(CFLAGS) -I$(INCLUDE) -L$(LIB_PATH) $(LIBS) -o bin/$@.out $@.cpp

clean:
	rm bin/*.out
	rm -r bin/*.dSYM
	rm *.dylib
