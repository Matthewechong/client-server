SHELL = /bin/bash
OBJECTS = udpServer.o

srcdir = .

CC = -lboost_system -lboost_thread -lpthread

INSTALL = ./install -c

a.out : $(OBJECTS)
	g++ -O0 -g -std=c++11 -o a.out $(OBJECTS) $(CC)
	
udpServer.o: udpServer.cpp

.PHONY : clean
clean :
	rm $(OBJECTS)

install : 