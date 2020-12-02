#define _POSIX_C_SOURCE 200809L
#include "abb.h"
#include "testing.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// static void prueba_...(void) {

// }

/* ******************************************************************
 *                 NUESTRAS PRUEBAS (ELIMINAR)
 * *****************************************************************/

int mi_cmp (const char* a, const char* b) {
	int _a = atoi(a);
	int _b = atoi(b);
	
	if (_a > _b) return 1;
	if (_a < _b) return -1;
	return 0;
}

void pruebas_internas() { //Lugar para correr pruebas internas, borrar al final

	abb_t* abb = abb_crear(strcmp, NULL);

	char* claves[] = {"18","23","16","21","12","17","11","15"};
	int datos[] = {18,23,16,21,12,17,11,15};

	for(int i = 0; i < 8; i++){
		abb_guardar(abb, claves[i], datos+i);
		imprimir_abb(abb);
		printf("El árbol de arriba tiene altura: %zu\n", abb_altura(abb));
		printf("\n\n");
	}
}

void prueba_internas_ari() {
	abb_t* abb = abb_crear(mi_cmp, NULL);

	char* claves[] = {"9","7","1","16","17"};
	int datos[] = {9,7,1,16,17};
	size_t cant = 5;

	for(int i = 0; i < cant; i++){
		printf("la clave es: %i\n", datos[i]);
		abb_guardar(abb, claves[i], &datos[i]);
		imprimir_abb(abb);
		printf("El árbol de arriba tiene altura: %zu\n", abb_altura(abb));
		printf("\n\n");
	}
}

void prueba_buscar_nodo() {
	abb_t* abb = abb_crear(mi_cmp, NULL);

	char* claves[] = {"9","7","1","16","17"};
	char* claves_no_abb[] = {"10", "45", "90","0", "89"};
	int datos[] = {9,7,1,16,17};
	size_t cant = 5;

	for(int i = 0; i < cant; i++){
		abb_guardar(abb, claves[i], &datos[i]);
	}
	imprimir_abb(abb);

	print_test("Obtener raiz", *(int*)abb_obtener(abb,  claves[0]) == datos[0]);

	bool resultado_obtener = true;
	for (int i = 0; i < cant && resultado_obtener; i++) {
		if (*(int*)abb_obtener(abb,  claves[i]) != datos[i]) resultado_obtener = false;
	}
	print_test("Obtener todos los elementos", resultado_obtener);

	bool resultado_pertenece = true;
	//claves del abb
	for (int i = 0; i < cant && resultado_obtener; i++) {
		if (!abb_pertenece(abb,  claves[i])) resultado_obtener = false;
	}
	print_test("Resultado pertenece claves q perteneces", resultado_pertenece);

	//claves que no pertenen al abb
	resultado_pertenece = true;
	for (int i = 0; i < cant && resultado_obtener; i++) {
		if (abb_pertenece(abb,  claves_no_abb[i])) resultado_obtener = false;
	}
	print_test("Resultado no perteneces al abb", resultado_pertenece);
}

void pruebas_borrar() {
	abb_t* abb = abb_crear(mi_cmp, NULL);

	char* claves[] = {"9","7","1","16","17"};
	//char* claves_no_abb[] = {"10", "45", "90","0", "89"};
	int datos[] = {9,7,1,16,17};
	size_t cant = 5;

	for(int i = 0; i < cant; i++){
		abb_guardar(abb, claves[i], &datos[i]);
	}
	imprimir_abb(abb);

	print_test("La cantidad de elementos es correcta", abb_cantidad(abb) == cant);

	printf("borro el 17\n");
	print_test("Borrar hoja", *(int*)abb_borrar(abb, "17") == 17);
	cant--;
	print_test("La cantidad de elementos es correcta", abb_cantidad(abb) == cant);
	imprimir_abb(abb);

	printf("borro el 7\n");
	print_test("Borrar con un hijo", *(int*)abb_borrar(abb, "7") == 7);
	cant--;
	print_test("La cantidad de elementos es correcta", abb_cantidad(abb) == cant);
	imprimir_abb(abb);

	printf("borro el 9\n");
	print_test("Borrar con dos hijos/raiz", *(int*)abb_borrar(abb, "9") == 9);
	cant--;
	print_test("La cantidad de elementos es correcta", abb_cantidad(abb) == cant);
	imprimir_abb(abb);
}

void pruebas_abb_estudiante() {
	//nuestras pruebas (ELIMINAR)
    pruebas_internas();
	prueba_internas_ari();
	prueba_buscar_nodo();
	pruebas_borrar();

	//pruebas primitivas abb

	//pruebas iterador externo

	//pruebas iterador interno
}

//#ifndef CORRECTOR

int main(void) {
    pruebas_abb_estudiante();
    return failure_count() > 0;
}

//#endif
