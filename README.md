# icc3201-201920-tarea1-avial5@miuandes.cl-maponce@miuandes.cl
Repositorio base para tarea 1 de Sistemas Operativos y Redes 201920

**Dificultades encontradas:**
* La implementacion de sockets: el como usarlos, investigarlos y sus funciones listen() y accept(), tambien el que el cliente no se quede esperando con un read infinito.

* Los comandos: que se cumpliera el timeout del comando connect, diferenciar en el codigo los comandos insert(key,value) e insert(value), el manejo de substrings para estos comandos.

**Funciones no implementadas:**
* Faltó implementar el timeout de 10 segundos para cuando se ejecuta el cliente

**Problemas conocidos:**
* Si se inserta o se hace get con una key como un string en vez de int, ocurren errores.
