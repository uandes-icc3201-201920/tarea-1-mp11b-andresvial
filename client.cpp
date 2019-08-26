#include <iostream>
#include <memory>
#include <string>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include "util.h"

using namespace std;

int main(int argc, char** argv) {

	cout<<"estoy vivo"<<endl;
	string dir_socket;
	int sflag = 0;
	int opt;

	struct sockaddr_un client;
	
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
	
	//CReo el socket
        int sock = socket(AF_UNIX, SOCK_STREAM, 0);
        if (sock < 0) {
            perror("opening stream socket");
            exit(1);
        }

	//SE conecta el socket
        client.sun_family = AF_UNIX;
        strcpy(client.sun_path, dir_socket.c_str());
        if (connect(sock, (struct sockaddr *) &client, sizeof(struct sockaddr_un)) < 0) {
            close(sock);
            perror("connecting stream socket");
            exit(1);
        }

	//Para mandar mensajes de prueba
	/*while(1<2){
		char mensaje[1024];
   		fgets(mensaje,1024,stdin);
		write(sock, mensaje, sizeof(mensaje));
	}*/



	string cmd = "";
	
	while (cmd != "quit") {
		if(cmd == "connect")
		{	
		}
		else if (cmd == "disconect")
		{
		}
		cout << ">";
		cin >> cmd;
	}

	return 0;	
}
