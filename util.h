#include <map>
#include <vector>
#include <string>

using namespace std;

typedef unsigned char byte;

// Esta estructura guarda el valor en la estructura K-V
typedef struct {
	size_t size;
	int data;
} Value;

// Definicion de la estructura K-V utilizada
// Clave es numerica y valor es dado por estructura Value.
typedef map<unsigned long, string> KVStore;
