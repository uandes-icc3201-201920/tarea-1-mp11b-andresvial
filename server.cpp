#include <iostream>
#include <string>
#include <memory>
#include <sstream>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <time.h> 
#include "util.h"

using namespace std;


// Almacenamiento KV
KVStore db;

int main(int argc, char** argv) {
	
	int sflag = 0;
	int opt;
	string dir_socket;
	srand (time(NULL));

	struct sockaddr_un server;
	
	// Procesar opciones de linea de comando
    	while ((opt = getopt (argc, argv, "s:")) != -1) {
        switch (opt)
		{
			/* Procesar el flag s si el usuario lo ingresa */
			case 's':
				sflag = 1;
				dir_socket = optarg;
				break;
			default:
				dir_socket = "/tmp/db.tuples.sock";
          }	    	
    }

	//Se crea el socket
	int sock = socket(AF_UNIX, SOCK_STREAM, 0);
	if (sock < 0) {
		perror("Error al crear el socket");
 	        exit(1);
	}
	
	unlink(dir_socket.c_str());

	//SE hace bind
        server.sun_family = AF_UNIX;
        strcpy(server.sun_path, dir_socket.c_str());
        if (bind(sock, (struct sockaddr *) &server, sizeof(struct sockaddr_un))) {
            perror("Error al bindiear");
            exit(1);
        }
	//Insertar valores random
	db.insert(std::pair<unsigned long, string>(1020, "hola"));
	db.insert(std::pair<unsigned long, string>(1030, "hola"));
	db.insert(std::pair<unsigned long, string>(1040, "hola"));
	db.insert(std::pair<unsigned long, string>(1050, "hola"));
	db.insert(std::pair<unsigned long, string>(1060, "hola"));
	db.insert(std::pair<unsigned long, string>(1070, "hola"));
	
	int clave_autogenerada = rand() % 9000 +1000; //Se genera una clave autogenerada

	//Se detiene a escuhar y luego a aceptar conecciones en un loop infinito
	listen(sock, 10);
	int readvalue;
        while(1<2) {
	    char mensaje[50000];
            int cliente_sock = accept(sock, 0, 0);
            if (cliente_sock == -1)
                cout<<"Error al aceptar la conexion"<<endl;
            else do {
                if ((readvalue = read(cliente_sock, mensaje, 50000)) < 0)
                    perror("reading stream message");
                else if (readvalue == 0)
                    cout<<"Coneccion terminada\n";
                else{
			char *token;
			token = strtok(mensaje, ",");
			int cont =0;
			while( token != NULL ) {
				if (strcmp(token,"i1")==0)//Comando: insert(value)
				{
					token = strtok(NULL, ",");
					strcpy(mensaje,token);
					string v = mensaje;
					stringstream k;
					k << clave_autogenerada;
					db.insert(std::pair<unsigned long, string>(clave_autogenerada, v));
					clave_autogenerada++;
                  		 	write(cliente_sock,(k.str()).c_str(), 50000);
				}
				else if (strcmp(token,"g")==0)//Comando: get(key)
				{
					token = strtok(NULL, ",");
					strcpy(mensaje,token);
					string s = mensaje;
                  		 	int k = stoi(s);
					if(db.find(k) == db.end())
					{
						string error = "Error: La Key no se encuentra en la BD";
						strcpy(mensaje,error.c_str());
						write(cliente_sock, mensaje, 50000);
					}
					else{
                  		 		write(cliente_sock,db[k].c_str(), 50000);
					}
				}
				else if (strcmp(token,"p")==0)//Comando: peek(key)
				{
					token = strtok(NULL, ",");
					strcpy(mensaje,token);
					string s = mensaje;
                  		 	int k = stoi(s);
					//Si la key se encuentra en la bd, se envia el mesaje true y si no se envia false
					if(db.find(k) != db.end())
					{
						strcpy(mensaje,"True");
					}
					else
					{
						strcpy(mensaje,"False");
					}
                  		 	write(cliente_sock, mensaje, 50000);
				}
				else if (strcmp(token,"d")==0)//Comando: delete
				{
					token = strtok(NULL, ",");
					strcpy(mensaje,token);
					string s = mensaje;//Se convierte el mensaje en string
                  		 	int k = stoi(s);//Se convierte el string de la key, a un int para poder buscarlo en db y eliminarlo
					db.erase(k);//Elimina la tupla kv
					strcpy(mensaje,"Borrado con exito");
                  		 	write(cliente_sock, mensaje, 50000);
				}
				else if (strcmp(token,"i2")==0)//Comando: insert(key,value)
				{
					token = strtok(NULL, ",");
					string k = token;
					int key = stoi(k);
					token = strtok(NULL, ",");
					string value = token;
					
					if(db.find(key) != db.end())
					{
						string error = "Error: La Key ya se encuentra en la BD";
						strcpy(mensaje,error.c_str());
						write(cliente_sock, mensaje, 50000);
					}
					else
					{
						string m = "Se Insertó correctamente";
						strcpy(mensaje,m.c_str());
						db.insert(std::pair<unsigned long, string>(key,value));
						write(cliente_sock, mensaje, 50000);
					}
				
				}

				else if (strcmp(token,"u")==0)//Comando: update(key,value)
				{
					token = strtok(NULL, ",");
					string k = token;
					int key = stoi(k);
					token = strtok(NULL, ",");
					token[strlen(token)] = '\0';
					string value = token;
					
					if(db.find(key) == db.end())
					{
						string error = "Error: La Key no se encuentra en la BD";
						strcpy(mensaje,error.c_str());
						write(cliente_sock, mensaje, 50000);
					}
					else
					{
						string m = "Se actualizo el valor correctamente";
						strcpy(mensaje,m.c_str());
						db[key] = value;
						write(cliente_sock, mensaje, 50000);
					}
				
				}
				else if (strcmp(token,"l")==0)//Comando: list
				{
					string lista = "[";//Se añade corchete a la lista
					//Se leen todas las key y si es encontrada la key se manda a la lista 
					map<unsigned long, string>::iterator it;
					for ( it = db.begin(); it != db.end(); it++ )
					{
						stringstream k;
						k << it->first;
						lista += " "+k.str()+" ";
  
					}
					//Se le coloca corchete final a la lista
					lista += "]";
					strcpy(mensaje,lista.c_str());
					write(cliente_sock,mensaje, 50000);
					
				}
				token = strtok(NULL, ",");
				cont++;
			}
			
		}
			
            } while (readvalue > 0);
            close(cliente_sock);
        }

	return 0;
}
