CC=g++
flags=-Wall

all: server client

server: server.cpp util.h
	$(CC) $(flags) server.cpp -o s

client: client.cpp 
	$(CC) $(flags) client.cpp -o c

clean:
	rm -f s
	rm -f c

