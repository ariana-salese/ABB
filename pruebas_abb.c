#include "abb.h"
#include "testing.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// static void prueba_...(void) {

// }

void pruebas_internas() { //Lugar para correr pruebas internas, borrar al final

	abb_t* abb = abb_crear(strcmp, NULL);

	char* claves[] = {"18","23","16","21","12","17","11","15"};
	int datos[] = {18,23,16,21,12,17,11,15};

	for(int i = 0; i < 8; i++){
		abb_guardar(abb, claves[i], datos+i);
		imprimir_abb(abb);
		printf("El árbol de arriba tiene altura: %zu\n",abb_altura(abb));
		printf("\n\n");
	}
}

void pruebas_abb_estudiante() {
    pruebas_internas();
}

//#ifndef CORRECTOR

int main(void) {
    pruebas_abb_estudiante();
    return failure_count() > 0;
}

//#endif
