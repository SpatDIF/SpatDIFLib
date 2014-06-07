CC = g++
CFLAGS = -arch x86_64 -arch i386 
INCLUDE = ../
LIB_PATH = ../
LIBS =  -lstdc++ -lspatdif
TARGET = libspatDIFC.a

all:  sdDate_c.c sdInfo_c.c
	$(CC) -c $(CFLAGS) -I$(INCLUDE) -L$(LIB_PATH) $? $(LIBS) 
	libtool -static -o $(TARGET) *.o
	make -C ./examples

clean:
	rm $(TARGET) *.o
	make clean -C ./examples 
