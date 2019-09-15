#include <iostream>
#include <memory>
#include <string>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <ctime>
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
	bool conectado = false;
	while (cmd != "quit") {
	
		cout << ">";
		cin >> cmd;
		int par1 = cmd.find('(');//Encuentra en el cmd el primer parentesis
		string comando = cmd.substr(0,par1);//String que que toma solo el comando(connect,disconnect,etc.)

		if(cmd == "connect")
		{
      		        if (conectado==false){
				//Creo el socket
				sock = socket(AF_UNIX, SOCK_STREAM, 0);
	       			if (sock < 0) {
		   		 	cout<<"Error al crear el socket"<<endl;
		   		 	continue;
	      			}
				//Se conecta el socket
				client.sun_family = AF_UNIX;
				strcpy(client.sun_path, dir_socket.c_str());
				if (connect(sock, (struct sockaddr *) &client, sizeof(struct sockaddr_un)) < 0)
		 		{
		   			 close(sock);
		   			 cout<<"Error al conectar el socket"<<endl;
		  			 continue;
	       			 }
				else{
					conectado = true;
					cout<<"Conectado con exito al servidor"<<endl;
				}
			}
			else{
				cout<<"Ya estas conectado"<<endl;
			}
			continue;		
		}
		else if (cmd == "disconnect")
		{
			if(conectado==true)
			{
				string instruccion = "di,";
				write(sock, instruccion.c_str(), 50000);
				close(sock);
				cout<<"Desconectado con exito del servidor"<<endl;
				conectado=false;
			}

			else{
				cout<<"Error: No se pudo desconectar ya que no hay conexion en primer lugar"<<endl;
			}
			continue;
		}
		else if (cmd == "list")
		{
			if (conectado==false){
				cout<<"Error: No te has conectado al server"<<endl;
				continue;
			}

			string instruccion = "l,";
			write(sock, instruccion.c_str(), 50000);
			char recibido[50000];
			read(sock, recibido, 50000);
			cout<<recibido<<endl;
			continue;
		}
		else if (comando == "insert" || comando == "get" || comando == "peek" || comando == "update" || comando == "delete")
		{
			if (conectado==false){
				cout<<"Error: No te has conectado al server"<<endl;
				continue;
			}
	
			string kv = cmd.substr(par1);//Substring a partir del parentesis '('
			int coma = kv.find(',');//Encuentra la posicion de la coma dentro del substring kv 
			string value = kv.substr(coma+1);//String value
			int n = value.length();//Toma el largo del string value
			value.erase(n-1);//Se elimina el ultimo elemento 

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
						write(sock, instruccion.c_str(), 50000);
					}
					else
					{		 
						string key = kv.substr(1,coma-1);
						string instruccion = "i2," + key + ","+value;
						write(sock, instruccion.c_str(), 50000);
					}
				}
				else if(comando == "get")
				{
					string k = kv.substr(1,kv.size()-2);
					string instruccion = "g," + k;
					write(sock, instruccion.c_str(), 50000);
					
				}
				else if(comando == "peek")
				{
					string k = kv.substr(1,kv.size()-2);
					string instruccion = "p," + k;
					write(sock, instruccion.c_str(), 50000);
					
				}
				else if(comando == "update")
				{
					string key = kv.substr(1,coma-1);
					string instruccion = "u," + key + ","+value;
					write(sock, instruccion.c_str(), 50000);
				}
				else if(comando == "delete")
				{
					string k = kv.substr(1,kv.size()-2);
					string instruccion = "d," + k;
					write(sock, instruccion.c_str(),50000);
					
				}
				else //Si no es ningun comando
				{	
					cout<<"Comando no valido"<<endl;
					continue;
				}
				//Se recibe el mensaje mandado por el socket como respuesta al comando
				char recibido[50000];
				read(sock, recibido, 50000);
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
