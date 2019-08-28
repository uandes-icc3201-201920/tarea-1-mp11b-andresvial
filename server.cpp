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
				return EXIT_FAILURE;
          }	    	
    }

	if (sflag == 0){
		dir_socket = "/tmp/db.tuples.sock";
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
	
	db.insert(std::pair<unsigned long, string>(1020, "hola"));
	//Se detiene a escuhar y luego a aceptar conecciones en un loop infinito
	listen(sock, 10);
	int readvalue;
        while(1<2) {
	    char mensaje[1024];
            int cliente_sock = accept(sock, 0, 0);
            if (cliente_sock == -1)
                perror("aceptar");
            else do {
                if ((readvalue = read(cliente_sock, mensaje, 1024)) < 0)
                    perror("reading stream message");
                else if (readvalue == 0)
                    cout<<"Terminando coneccion\n";
                else{
			char *token;
			token = strtok(mensaje, ",");
			int cont =0;
			while( token != NULL ) {
				if (strcmp(token,"i1")==0)//Comando: insert(value)
				{
					int clave_autogenerada = rand() % 9000 +1000; //Se genera una clave autogenerada
					token = strtok(NULL, ",");
					strcpy(mensaje,token);
					string v = mensaje;
					stringstream k;
					k << clave_autogenerada;
					db.insert(std::pair<unsigned long, string>(clave_autogenerada, v));
                  		 	write(cliente_sock,(k.str()).c_str(), sizeof((k.str()).c_str()));
				}
				else if (strcmp(token,"g")==0)//Comando: get(key)
				{
					token = strtok(NULL, ",");
					strcpy(mensaje,token);
					string s = mensaje;
                  		 	int k = stoi(s);
                  		 	write(cliente_sock,db[k].c_str(), sizeof(db[k].c_str()));
				}
				else if (strcmp(token,"p")==0)
				{
					token = strtok(NULL, ",");
					strcpy(mensaje,token);
					string s = mensaje;
                  		 	int k = stoi(s);
					if(db.find(k) != db.end())
					{
						strcpy(mensaje,"True");
					}
					else
					{
						strcpy(mensaje,"False");
					}
                  		 	write(cliente_sock, mensaje, sizeof(mensaje));
				}
				else if (strcmp(token,"d")==0)
				{
					token = strtok(NULL, ",");
					strcpy(mensaje,token);
					string s = mensaje;
                  		 	int k = stoi(s);
					db.erase(k);//Elimina la tupla kv
                  		 	write(cliente_sock, mensaje, sizeof(mensaje));
				}
				else if (strcmp(token,"i2")==0)
				{
					token = strtok(NULL, ",");
					strcpy(mensaje,token);
                  		 	cout<<cont<<endl;
				}
				token = strtok(NULL, ",");
				cont++;
			}
			
		}
			
            } while (readvalue > 0);
            close(cliente_sock);
        }

	// Uso elemental del almacenamiento KV:
	
	// Creamos un arreglo de bytes a mano
	//byte data[] = { 0x01, 0x01, 0x01, 0x01, 0x01 };

	// Luego un vector utilizando el arreglo de bytes
	//vector<byte> vdata(data, data + sizeof(data));
	
	// Creamos el valor
	//Value val = { 5, vdata };
	
	// Insertamos un par clave, valor directamente
	// en el mapa KV
	
	// Nota: Debiera diseñarse una solución más robusta con una interfaz
	// adecuada para acceder a la estructura.
	//db.insert(std::pair<unsigned long, Value>(1000, val));
		
	// Imprimir lo que hemos agregado al mapa KV.
	//cout << db[1]<< " " << db[1]<< endl;

	return 0;
}
