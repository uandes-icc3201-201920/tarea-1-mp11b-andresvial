#include <iostream>
#include <string>
#include <memory>
#include <sstream>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <time.h> 
#include <map>
#include <vector>
#include <pthread.h>

using namespace std;


typedef struct
{
    int sock;
    struct sockaddr address;
    unsigned int addr_len;
} connection_t;

// Almacenamiento KV
map<unsigned long, string> db;

void* funcion_thread( void* arg){
	connection_t * conn;
	conn = (connection_t *)arg;
	char mensaje[50000];
        int readvalue;
	do {
                if ((readvalue = read(conn->sock, mensaje, 50000)) < 0)
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
					k << "La key generada para el valor es: ";
					//k << clave_autogenerada;
					k << 1;
					//db.insert(std::pair<unsigned long, string>(clave_autogenerada, v));
					db.insert(std::pair<unsigned long, string>(1, v));
					//clave_autogenerada++;
                  		 	write(conn->sock,(k.str()).c_str(), 50000);
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
						write(conn->sock, mensaje, 50000);
					}
					else{
                  		 		write(conn->sock,db[k].c_str(), 50000);
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
                  		 	write(conn->sock, mensaje, 50000);
				}
				else if (strcmp(token,"d")==0)//Comando: delete
				{
					token = strtok(NULL, ",");
					strcpy(mensaje,token);
					string s = mensaje;//Se convierte el mensaje en string
                  		 	int k = stoi(s);//Se convierte el string de la key, a un int para poder buscarlo en db y eliminarlo
					db.erase(k);//Elimina la tupla kv
					strcpy(mensaje,"Borrado con exito");
                  		 	write(conn->sock, mensaje, 50000);
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
						write(conn->sock, mensaje, 50000);
					}
					else
					{
						string m = "Se Insertó correctamente";
						strcpy(mensaje,m.c_str());
						db.insert(std::pair<unsigned long, string>(key,value));
						write(conn->sock, mensaje, 50000);
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
						write(conn->sock, mensaje, 50000);
					}
					else
					{
						string m = "Se actualizo el valor correctamente";
						strcpy(mensaje,m.c_str());
						db[key] = value;
						write(conn->sock, mensaje, 50000);
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
					write(conn->sock,mensaje, 50000);
					
				}

				else if (strcmp(token,"di")==0)//Comando: disconnect
				{
				    close(conn->sock);
				    free(conn);
				    pthread_exit(0);	
				}
				token = strtok(NULL, ",");
				cont++;
			}
			
		}
			
            } while (readvalue > 0);
}

int main(int argc, char** argv) {
	
	int opt;
	int socket_local;
	connection_t* conexion_cliente;
	string dir_socket;
	srand (time(NULL));

	struct sockaddr_un server; 
	struct cliente_addr;
	
	// Procesar opciones de linea de comando
    	while ((opt = getopt (argc, argv, "s:")) != -1) {
        switch (opt)
		{
			/* Procesar el flag s si el usuario lo ingresa */
			case 's':
				dir_socket = optarg;
				break;
			default:
				dir_socket = "/tmp/db.tuples.sock";
          }	    	
    }

	//Se crea el socket
	socket_local = socket(AF_UNIX, SOCK_STREAM, 0);
	if (socket_local < 0) {
		perror("Error al crear el socket");
 	        exit(1);
	}
	
	unlink(dir_socket.c_str());

	//SE hace bind
        server.sun_family = AF_UNIX;
        strcpy(server.sun_path, dir_socket.c_str());
        if (bind(socket_local, (struct sockaddr *) &server, sizeof(struct sockaddr_un))) {
            perror("Error al bindiear");
            exit(1);
        }
	//Insertar valores random
	db.insert(std::pair<unsigned long, string>(1020, "hola"));
	db.insert(std::pair<unsigned long, string>(1030, "como"));
	db.insert(std::pair<unsigned long, string>(1040, "estas"));
	db.insert(std::pair<unsigned long, string>(1050, "5000"));
	db.insert(std::pair<unsigned long, string>(1060, "teclado"));
	db.insert(std::pair<unsigned long, string>(1070, "144"));
	
	int clave_autogenerada = rand() % 9000 +1000; //Se genera una clave autogenerada

	//Se detiene a escuhar y luego a aceptar conecciones en un loop infinito
	pthread_t t1;
	listen(socket_local, 10);
        while(1<2) {
	    conexion_cliente = (connection_t *)malloc(sizeof(connection_t));
            conexion_cliente->sock = accept(socket_local, &conexion_cliente->address, &conexion_cliente->addr_len);
            if ( conexion_cliente->sock == -1){
		free(conexion_cliente);
                cout<<"Error al aceptar la conexion"<<endl;
		}
            else{
		/* start a new thread but do not wait for it */
		//pthread_create(&t1, NULL, funcion_thread, static_cast<void*>(&db));
		pthread_create(&t1, NULL, funcion_thread, (void*) conexion_cliente);
		pthread_detach(t1);
	    }
        }

	return 0;
}
