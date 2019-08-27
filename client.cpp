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
	
	string cmd = "";
	
	while (cmd != "quit") {
	
		cout << ">";
		cin >> cmd;
		int par1 = cmd.find('(');
		int par2 = cmd.find(')');
		string comando = cmd.substr(0,par1);
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
			//Para mandar mensajes de prueba
			while(1<2){
			char mensaje[1024];
   			fgets(mensaje,1024,stdin);
			write(sock, mensaje, sizeof(mensaje));
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
		else if (comando == "insert" || comando == "get" || comando == "peek" || comando == "update" || comando == "delete" )
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
						cout<<"El value es:"<<v<<endl;
					}
					else
					{		
						int key = stoi(kv.substr(1,coma-1));
						cout<<"La key es:"<<key<<endl;
						cout<<"El value es:"<<value<<endl;
					}
				}
				else if(comando == "get")
				{
					int k = stoi(kv.substr(1,kv.size()-2));
					cout<<"La key es:"<<k<<endl;
					
				}
				else if(comando == "peek")
				{
					int k = stoi(kv.substr(1,kv.size()-2));
					cout<<"La key es:"<<k<<endl;
					
				}
				else if(comando == "update")
				{
					int key = stoi(kv.substr(1,coma-1));
					cout<<"La key es:"<<key<<endl;
					cout<<"El value es:"<<value<<endl;
				}
				else if(comando == "delete")
				{
					int k = stoi(kv.substr(1,kv.size()-2));
					cout<<"La key es:"<<k<<endl;
					
				}
				else //Si no es ningun comando
				{	
					cout<<"Comando no valido"<<endl;
					continue;
				}
			}
		}
		else
		{
			cout<<"Comando no valido"<<endl;
			continue;
		}
	}
	
	return 0;	
}
