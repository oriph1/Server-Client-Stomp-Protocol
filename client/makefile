CFLAGS:=-c -Wall -Weffc++ -g -std=c++11 -Iinclude
LDFLAGS:=-lboost_system -lpthread

all: StompWCIClient

EchoClient: bin/ConnectionHandler.o bin/echoClient.o
	g++ -o bin/EchoClient bin/ConnectionHandler.o bin/echoClient.o $(LDFLAGS)

StompWCIClient: bin/ConnectionHandler.o bin/StompClient.o bin/Event.o  bin/StompProtocol.o  bin/Reader.o  bin/Summary.o  bin/Writer.o
	g++ -o bin/StompWCIClient bin/ConnectionHandler.o bin/StompClient.o bin/Event.o bin/StompProtocol.o  bin/Reader.o  bin/Summary.o  bin/Writer.o $(LDFLAGS)

bin/ConnectionHandler.o: src/ConnectionHandler.cpp
	g++ $(CFLAGS) -o bin/ConnectionHandler.o src/ConnectionHandler.cpp

bin/echoClient.o: src/echoClient.cpp
	g++ $(CFLAGS) -o bin/echoClient.o src/echoClient.cpp

bin/Event.o: src/Event.cpp
	g++ $(CFLAGS) -o bin/Event.o src/Event.cpp

bin/StompClient.o: src/StompClient.cpp
	g++ $(CFLAGS) -o bin/StompClient.o src/StompClient.cpp

bin/StompProtocol.o:src/StompProtocol.cpp
	g++ $(CFLAGS) -o bin/StompProtocol.o src/StompProtocol.cpp

bin/Reader.o:src/Reader.cpp
	g++ $(CFLAGS) -o bin/Reader.o src/Reader.cpp
	
bin/Writer.o:src/Writer.cpp
	g++ $(CFLAGS) -o bin/Writer.o src/Writer.cpp

bin/Summary.o:src/Summary.cpp
	g++ $(CFLAGS) -o bin/Summary.o src/Summary.cpp	

.PHONY: clean
clean:
	rm -f bin/*
	