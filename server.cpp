#include <iostream>
#include <memory>
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
	int clave_autogenerada = rand() % 9000 +1000;

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
	
	//SE hace bind
        server.sun_family = AF_UNIX;
        strcpy(server.sun_path, dir_socket.c_str());
        if (bind(sock, (struct sockaddr *) &server, sizeof(struct sockaddr_un))) {
            perror("Error al bindiear");
            exit(1);
        }

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
                else
                    cout<<"-->"<<mensaje<<"\n";
            } while (readvalue > 0);
            close(cliente_sock);
        }

	// Uso elemental del almacenamiento KV:
	
	// Creamos un arreglo de bytes a mano
	byte data[] = { 0x01, 0x01, 0x01, 0x01, 0x01 };

	// Luego un vector utilizando el arreglo de bytes
	vector<byte> vdata(data, data + sizeof(data));
	
	// Creamos el valor
	Value val = { 5, vdata };
	
	// Insertamos un par clave, valor directamente
	// en el mapa KV
	
	// Nota: Debiera diseñarse una solución más robusta con una interfaz
	// adecuada para acceder a la estructura.
	db.insert(std::pair<unsigned long, Value>(1000, val));
		
	// Imprimir lo que hemos agregado al mapa KV.
	cout << db[1000].size << " " << (int) db[1000].data[0] << endl;

	return 0;
}
