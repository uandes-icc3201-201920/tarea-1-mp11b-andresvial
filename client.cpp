#include <iostream>
#include <memory>
#include <string>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include "util.h"

using namespace std;

int main(int argc, char** argv) {

	string dir_socket;
	int sflag = 0;
	int opt;
	
 	while ((opt = getopt (argc, argv, "s:")) != -1) 
	{
        	switch (opt)
		{
			/* Procesar el flag s si el usuario lo ingresa */
			case 's':
				sflag = 1;
				dir_socket = optarg;
				break;
			default:
				return EXIT_FAILURE;
         	 }	    	
   	 }
	if(sflag == 0)
	{
		dir_socket == "/tmp/db.tuples.sock.";
	}
	
	string cmd = "";
	
	while (cmd != "quit") {
		if(cmd == "connect")
		{
			cout<<1<<endl;		
		}
		else if (cmd == "disconect")
		{}
		cout << ">";
		cin >> cmd;
	}

	return 0;	
}
