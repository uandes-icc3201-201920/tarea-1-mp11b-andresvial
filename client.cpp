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
	
	//Para mandar mensajes de prueba
	/*while(1<2){
		char mensaje[1024];
   		fgets(mensaje,1024,stdin);
		write(sock, mensaje, sizeof(mensaje));
	}*/
	
	string cmd = "";
	
	while (cmd != "quit") {
	
		cout << ">";
		cin >> cmd;
		int par1 = cmd.find('(');
		int par2 = cmd.find(')');
		if(cmd == "connect")
		{
			//SE conecta el socket
        		client.sun_family = AF_UNIX;
        		strcpy(client.sun_path, dir_socket.c_str());
        		if (connect(sock, (struct sockaddr *) &client, sizeof(struct sockaddr_un)) < 0)
	 		{
           			 close(sock);
           			 perror("connecting stream socket");
          			  exit(1);
       			 }		
		}
		else if (cmd == "disconnect")
		{
			cout<<2<<endl;
		}
		else if (cmd == "list")
		{
			cout<<3<<endl;
		}
		else
		{
			string comando = cmd.substr(0,par1);	
			string kv = cmd.substr(par1);
			int coma = kv.find(',');
			int p = kv.find(')');
			int key = stoi(kv.substr(1,coma-1));
			string value = kv.substr(coma+1,p-3);
			int k = stoi(kv.substr(1,kv.size()-2));
			if(comando == "insert")
			{
				cout<<"La key es:"<<key<<endl;
				cout<<"El value es:"<<value<<endl;
			}
			else if(comando == "get")
			{

				cout<<"La key es:"<<k<<endl;
				
			}
			else if(comando == "peek")
			{
				cout<<"La key es:"<<k<<endl;
				
			}
			else if(comando == "update")
			{
				cout<<"La key es:"<<key<<endl;
				cout<<"El value es:"<<value<<endl;
			}
			else if(comando == "delete")
			{
				cout<<"La key es:"<<k<<endl;
				
			}
		}
	}
	
	return 0;	
}
