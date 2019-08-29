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
	
	string cmd = "";
	
	int sock = 0;

	while (cmd != "quit") {
	
		cout << ">";
		cin >> cmd;
		int par1 = cmd.find('(');
		int par2 = cmd.find(')');
		string comando = cmd.substr(0,par1);
		if(cmd == "connect")
		{
			//CReo el socket
      		        sock = socket(AF_UNIX, SOCK_STREAM, 0);
       			if (sock < 0) {
           		 	perror("opening stream socket");
           		 	exit(1);
      			  }
			//SE conecta el socket
        		client.sun_family = AF_UNIX;
        		strcpy(client.sun_path, dir_socket.c_str());
        		if (connect(sock, (struct sockaddr *) &client, sizeof(struct sockaddr_un)) < 0)
	 		{
           			 close(sock);
           			 perror("connecting stream socket");
          			 exit(1);
       			 }
			continue;		
		}
		else if (cmd == "disconnect")
		{
			if(sock > 0)
			{
				close(sock);
			}
			continue;
		}
		else if (cmd == "list")
		{
			string instruccion = "l,";
			write(sock, instruccion.c_str(), sizeof(instruccion.c_str()));
			char recibido[1024];
			read(sock, recibido, 1024);
			cout<<recibido<<endl;
			continue;
		}
		else if (comando == "insert" || comando == "get" || comando == "peek" || comando == "update" || comando == "delete")
		{	
			string kv = cmd.substr(par1);//Substring a partir del parentesis '('
			int coma = kv.find(',');//Encuentra la posicion de la coma dentro del substring kv 
			int p = kv.find(')');//Encuentra el parentesis final en el substring kv ')'
			string value = kv.substr(coma+1,p-3);//String value

			if(kv.size() < 3) //Si no se escribe nada entre los parentesis será invalido
			{
				cout<<"Comando no valido"<<endl;
				continue;
			}
			else{

				if(comando == "insert")
				{
					if(coma == -1) //Sino hay coma en el string, se asume que se querrá insertar un value
					{
						string v = kv.substr(1,kv.size()-2);
						string instruccion = "i1," + v;
						write(sock, instruccion.c_str(), sizeof(instruccion.c_str()));
					}
					else
					{		 
						string key = kv.substr(1,coma-1);
						string instruccion = "i2," + key + ","+value;
						write(sock, instruccion.c_str(), sizeof(instruccion.c_str()));
					}
				}
				else if(comando == "get")
				{
					string k = kv.substr(1,kv.size()-2);
					string instruccion = "g," + k;
					write(sock, instruccion.c_str(), sizeof(instruccion.c_str()));
					
				}
				else if(comando == "peek")
				{
					string k = kv.substr(1,kv.size()-2);
					string instruccion = "p," + k;
					write(sock, instruccion.c_str(), sizeof(instruccion.c_str()));
					
				}
				else if(comando == "update")
				{
					string key = kv.substr(1,coma-1);
					string instruccion = "u," + key + ","+value;
					write(sock, instruccion.c_str(), sizeof(instruccion.c_str()));
				}
				else if(comando == "delete")
				{
					string k = kv.substr(1,kv.size()-2);
					string instruccion = "d," + k;
					write(sock, instruccion.c_str(), sizeof(instruccion.c_str()));
					
				}
				else //Si no es ningun comando
				{	
					cout<<"Comando no valido"<<endl;
					continue;
				}
				//Se recibe el mensaje mandado por el socket como respuesta al comando
				char recibido[1024];
				read(sock, recibido, 1024);
				cout<<recibido<<endl;
			}
		}
		else if(cmd != "quit")
		{
			cout<<"Comando no valido"<<endl;
			continue;
		}
		
	}
	
	return 0;	
}
