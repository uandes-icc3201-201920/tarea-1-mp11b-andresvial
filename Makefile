flags=-Wall

server: server.cpp client.cpp
	g++ $(flags) server.cpp -o server
	g++ $(flags) client.cpp -o client

clean:
	rm -f *.o



