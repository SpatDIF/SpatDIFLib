UNAME := $(shell uname -s)
CC = clang++
CFLAGS =  -std=c++11 -O3 -arch x86_64 -arch i386
LIBS = -lstdc++
INCLUDE_DIR = -I../src/
OBJECTS = *.o
FILES = *.cpp


all: 
	-@make $(UNAME)

Darwin: 1basic

1basic:
	(CC) (CFLAGS) (LIBS) (INCLUDE_DIR) (OBJSCTS) (FILES)
