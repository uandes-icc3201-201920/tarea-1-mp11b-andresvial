CC=g++
flags=-Wall

all: server client

server: server.cpp util.h
	$(CC) $(flags) -pthread server.cpp -o server

client: client.cpp 
	$(CC) $(flags) client.cpp -o client

clean:
	rm -f server
	rm -f client
