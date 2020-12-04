#define _POSIX_C_SOURCE 200809L
#include "abb.h"
#include "hash.h"
#include "testing.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CANTIDAD_ELEMENTOS 15

/* ******************************************************************
 *                 NUESTRAS PRUEBAS (ELIMINAR)
 * *****************************************************************/

//No debería hacer falta, hay que arreglar eso de que no te anda strcmp
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
	printf("\nPRUEBAS BUSCAR NODO\n");
	abb_t* abb = abb_crear(strcmp, NULL);

	char* claves[] = {"23", "34", "28", "42", "12", "16", "15", "19", "21", "55", "09", "20", "22", "48"};
	char* claves_no_abb[] = {"10", "45", "90", "0", "89", "2", "1", "0", "67890","899", "343", "1231", "44", "2321"};
	int datos[] = {23, 34, 28, 42, 12, 16, 15, 19, 21, 55, 9, 20, 22, 48};
	size_t cant = 14;

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
	print_test("Pertenece da true para todos los elementos", resultado_pertenece);

	//claves que no pertenen al abb
	resultado_pertenece = true;
	for (int i = 0; i < cant && resultado_obtener; i++) {
		if (abb_pertenece(abb,  claves_no_abb[i])) resultado_obtener = false;
	}
	print_test("Pertenece da false para elementos que no estan en el ABB", resultado_pertenece);

	abb_destruir(abb);
}

void pruebas_borrar() {
	printf("\nPRUEBAS BORRAR NODO\n");
	abb_t* abb = abb_crear(strcmp, NULL);

	char* claves[] = {"23", "34", "28", "42", "12", "16", "15", "19", "21", "55", "09", "20", "22", "48"};
	//char* claves_no_abb[] = {"10", "45", "90", "0", "89"};
	int datos[] = {23, 34, 28, 42, 12, 16, 15, 19, 21, 55, 9, 20, 22, 48};
	size_t cant = 14;

	for(int i = 0; i < cant; i++){
		abb_guardar(abb, claves[i], &datos[i]);
	}
	imprimir_abb(abb);

	print_test("La cantidad de elementos es correcta", abb_cantidad(abb) == cant);
	
	printf("borro el 22\n");
	print_test("El elemento pertenece al árbol", abb_pertenece(abb,claves[12]));
	print_test("Borrar hoja", *(int*)abb_borrar(abb, claves[12]) == datos[12]);
	cant--;
	print_test("La cantidad de elementos es correcta", abb_cantidad(abb) == cant);
	print_test("El elemento borrado ya no pertenece al árbol", !abb_pertenece(abb,claves[12]));
	imprimir_abb(abb);

	printf("borro el 55\n");
	print_test("El elemento pertenece al árbol", abb_pertenece(abb,claves[9]));
	print_test("Borrar con un hijo", *(int*)abb_borrar(abb, claves[9]) == datos[9]);
	cant--;
	print_test("La cantidad de elementos es correcta", abb_cantidad(abb) == cant);
	print_test("El elemento borrado ya no pertenece al árbol", !abb_pertenece(abb,claves[9]));
	imprimir_abb(abb);
	
	printf("borro el 34\n");
	print_test("El elemento pertenece al árbol", abb_pertenece(abb,claves[1]));
	print_test("Borrar con dos hijos", *(int*)abb_borrar(abb, claves[1]) == datos[1]);
	cant--;
	print_test("La cantidad de elementos es correcta", abb_cantidad(abb) == cant);
	print_test("El elemento borrado ya no pertenece al árbol", !abb_pertenece(abb,claves[1]));
	imprimir_abb(abb);
	
	abb_destruir(abb);
}

/* ******************************************************************
 *                      PRUEBAS PRIMITIVAS ABB
 * *****************************************************************/

// static void prueba_abb_un_elemento() {
// 	printf("\nprueba un solo elemento:\n");

//     abb_t* arbol = abb_crear(strcmp, NULL);
// 	char* clave = "5";
// 	int valor = 5;

// 	print_test("Crear arbol vacio", arbol);
// 	print_test("El arbol esta vacio", abb_cantidad(arbol) == 0);
// 	print_test("Guardar clave", abb_guardar(arbol, clave, &valor));
// 	print_test("La cantidad de elementos es 1", abb_cantidad(arbol) == 1);
// 	print_test("La clave pertenece al arbol", abb_pertenece(arbol, clave));
// 	print_test("Obtener clave devuelve su valor", *(int*)abb_obtener(arbol, clave) == valor);
// 	print_test("Borrar clave devuelve su valor", *(int*)abb_borrar(arbol, clave) == valor);
// 	print_test("La clave ya no pertenece al arbol", !abb_pertenece(arbol, clave));
// 	print_test("La cantidad de elementos es 0", abb_cantidad(arbol) == 0);

// 	abb_destruir(arbol);
// }

// static void prueba_abb_guardar() {
// 	printf("\n> prueba guardar algunos elementos:\n");
// 	abb_t* arbol = abb_crear(strcmp, NULL);

// 	char* claves[] = {"23", "34", "28", "42", "12", "16", "15", "19", "21", "55", "09", "20", "22", "48", "6"};
// 	//char* claves_no_abb[] = {"10", "45", "90", "0", "89", "2", "1", "0", "67890","899", "343", "1231", "44", "2321", "3"};
// 	int datos[] = {23, 34, 28, 42, 12, 16, 15, 19, 21, 55, 9, 20, 22, 48, 6};
// 	bool resultado_guardar = true;
// 	bool resultado_cantidad = true;

// 	for(int i = 0; i < CANTIDAD_ELEMENTOS; i++) {
// 		if (!abb_guardar(arbol, claves[i], &datos[i])) resultado_guardar = false;
// 		if (abb_cantidad(arbol) != i + 1) resultado_cantidad = false;
// 	}

// 	print_test ("Se guardaron algunos elementos", resultado_guardar);
// 	print_test ("La cantidad de elementos es correcta", resultado_cantidad);
// 	//print_test("Se cumplen las condiciones de abb", condicion_de_abb(arbol));

// 	abb_destruir(arbol);
// }

static void prueba_crear_abb_vacio() {
	printf("> prueba arbol vacio:\n");

    abb_t* arbol = abb_crear(strcmp, NULL);
	char* clave = "A";

    print_test("Crear arbol vacio", arbol);
    print_test("La cantidad de elementos es 0", abb_cantidad(arbol) == 0);
    print_test("Obtener clave A, es NULL, no existe", !abb_obtener(arbol, clave));
    print_test("Pertenece clave A, es false, no existe", !abb_pertenece(arbol, clave));
    print_test("Borrar clave A, es NULL, no existe", !abb_borrar(arbol, clave));

    abb_destruir(arbol);
	//print_test("Se destruyo el arbol correctamente", true);
}

static void prueba_abb_insertar() {
	printf("\n> prueba insertar algunos elementos\n");

	char *clave1 = "23", *clave2 = "34", *clave3 = "28";
	int valor1 = 23, valor2 = 34, valor3 = 28;

	abb_t* arbol = abb_crear(strcmp, NULL);

	/* Inserta 1 valor y luego lo borra */
	print_test("Guardar clave1", abb_guardar(arbol, clave1, &valor1));
	print_test("La cantidad de elementos es 1", abb_cantidad(arbol) == 1);
	print_test("Obtener clave1 es valor1", *(int*)abb_obtener(arbol, clave1) == valor1);
	print_test("Pertenece clave1, es true", abb_pertenece(arbol, clave1));
	print_test("Borrar clave1, es valor1", *(int*)abb_borrar(arbol, clave1) == valor1);
	print_test("Pertenece clave1, es false", !abb_pertenece(arbol, clave1));
	print_test("La cantidad de elementos es 0", abb_cantidad(arbol) == 0);

	/* Inserta otros 2 valores y no los borra (se destruyen con el hash) */
	print_test("Guardar clave2", abb_guardar(arbol, clave2, &valor2));
    print_test("La cantidad de elementos es 1", abb_cantidad(arbol) == 1);
    print_test("Obtener clave2 es valor2", *(int*)abb_obtener(arbol, clave2) == valor2);
    print_test("Pertenece clave2, es true", abb_pertenece(arbol, clave2));

	print_test("Guardar clave3", abb_guardar(arbol, clave3, &valor3));
    print_test("La cantidad de elementos es 2", abb_cantidad(arbol) == 2);
    print_test("Obtener clave3 es valor3", *(int*)abb_obtener(arbol, clave3) == valor3);
    print_test("Pertenece clave3, es true", abb_pertenece(arbol, clave3));

	abb_destruir(arbol);
}

static void prueba_abb_reemplazar() {
	printf("\n> prueba reemplazar dato\n");

	char *clave1 = "clave1", *clave2 = "clave2";
	int valor1a = 11, valor1b = 12, valor2a = 21, valor2b = 22;

	abb_t* arbol = abb_crear(strcmp, NULL);
	print_test("Crear arbol vacio", arbol);

	/* Inserta 2 valores y luego los reemplaza */
	print_test("Guardar clave1", abb_guardar(arbol, clave1, &valor1a));
	print_test("Obtener clave1 es valor1a", *(int*)abb_obtener(arbol, clave1) == valor1a);
	print_test("Guardar clave2", abb_guardar(arbol, clave2, &valor2a));
	print_test("Obtener clave2 es valor2a", *(int*)abb_obtener(arbol, clave2) == valor2a);
	print_test("La cantidad de elementos es 2", abb_cantidad(arbol) == 2);

	print_test("Guardar clave1 con otro valor", abb_guardar(arbol, clave1, &valor1b));
    print_test("Obtener clave1 es valor1b", *(int*)abb_obtener(arbol, clave1) == valor1b);
    print_test("Guardar clave2 con otro valor", abb_guardar(arbol, clave2, &valor2b));
    print_test("Obtener clave2 es valor2b", *(int*)abb_obtener(arbol, clave2) == valor2b);
    print_test("La cantidad de elementos es 2", abb_cantidad(arbol) == 2);

	abb_destruir(arbol);
}

static void prueba_abb_reemplazar_con_destruir() {
	printf("\n> prueba reemplazar dato con destruir\n");

    abb_t* arbol = abb_crear(strcmp, free);

    char *clave1 = "clave1", *clave2 = "clave2";
	char *valor1a, *valor1b, *valor2a, *valor2b;  

    /* Pide memoria para 4 valores */
    valor1a = malloc(10 * sizeof(char));
    valor1b = malloc(10 * sizeof(char));
    valor2a = malloc(10 * sizeof(char));
    valor2b = malloc(10 * sizeof(char));

    /* Inserta 2 valores y luego los reemplaza (debe liberar lo que reemplaza) */
    print_test("Guardar clave1", abb_guardar(arbol, clave1, valor1a));
    print_test("Obtener clave1 es valor1a", abb_obtener(arbol, clave1) == valor1a);
    print_test("Guardar clave2", abb_guardar(arbol, clave2, valor2a));
    print_test("Obtener clave2 es valor2a", abb_obtener(arbol, clave2) == valor2a);
    print_test("La cantidad de elementos es 2", abb_cantidad(arbol) == 2);

    print_test("Guardar clave1 con otro valor", abb_guardar(arbol, clave1, valor1b));
    print_test("Obtener clave1 es valor1b", abb_obtener(arbol, clave1) == valor1b);
    print_test("Guardar clave2 con otro valor", abb_guardar(arbol, clave2, valor2b));
    print_test("Obtener clave2 es valor2b", abb_obtener(arbol, clave2) == valor2b);
    print_test("La cantidad de elementos es 2", abb_cantidad(arbol) == 2);

    /* Se destruye el hash (se debe liberar lo que quedó dentro) */
    abb_destruir(arbol);
}


static void prueba_abb_borrar() {
	printf("\n> prueba borrar\n");

	abb_t* arbol = abb_crear(strcmp, NULL);

	char* claves[] = {"23", "34", "28", "42", "12", "16", "15", "19", "21", "55", "09", "20", "22", "48", "6"};
	//char* claves_no_abb[] = {"10", "45", "90", "0", "89"};
	int datos[] = {23, 34, 28, 42, 12, 16, 15, 19, 21, 55, 9, 20, 22, 48, 6};
	bool resultado_guardar = true;

	for(int i = 0; i < CANTIDAD_ELEMENTOS; i++) {
		if (!abb_guardar(arbol, claves[i], &datos[i])) resultado_guardar = false;
	}

	print_test("Se guardaron algunos elementos", resultado_guardar);
	print_test("La cantidad de elementos es correcta", abb_cantidad(arbol) == CANTIDAD_ELEMENTOS);
	
	printf("borrar hoja:\n");
	print_test("La clave pertenece al árbol", abb_pertenece(arbol,claves[12]));
	print_test("Borrar la hoja", *(int*)abb_borrar(arbol, claves[12]) == datos[12]);
	print_test("La cantidad de elementos es correcta", abb_cantidad(arbol) == CANTIDAD_ELEMENTOS - 1);
	print_test("La clave borrada ya no pertenece al árbol", !abb_pertenece(arbol,claves[12]));
	print_test("Obtener clave borrada es NULL", !abb_obtener(arbol, claves[12]));
	
	printf("borrar nodo con un hijo:\n");
	print_test("La clave pertenece al árbol", abb_pertenece(arbol,claves[9]));
	print_test("Borrar nodo con un hijo", *(int*)abb_borrar(arbol, claves[9]) == datos[9]);
	print_test("La cantidad de elementos es correcta", abb_cantidad(arbol) == CANTIDAD_ELEMENTOS - 2);
	print_test("La clave borrada ya no pertenece al árbol", !abb_pertenece(arbol,claves[9]));
	print_test("Obtener clave borrada es NULL", !abb_obtener(arbol, claves[9]));
	
	printf("borrar nodo con dos hijos:\n");
	print_test("El elemento pertenece al árbol", abb_pertenece(arbol,claves[1]));
	print_test("Borrar nodo con dos hijos", *(int*)abb_borrar(arbol, claves[1]) == datos[1]);
	print_test("La cantidad de elementos es correcta", abb_cantidad(arbol) == CANTIDAD_ELEMENTOS - 3);
	print_test("La clave borrada ya no pertenece al árbol", !abb_pertenece(arbol,claves[1]));
	print_test("Obtener clave borrada es NULL", !abb_obtener(arbol, claves[9]));
	
	abb_destruir(arbol);
}

static void prueba_abb_clave_vacia() {
	printf("\n> prueba clave vacia\n");

    abb_t* arbol = abb_crear(strcmp, NULL);

    char *clave = "", *valor = "";

    print_test("Guardar clave vacia", abb_guardar(arbol, clave, valor));
    print_test("La cantidad de elementos es 1", abb_cantidad(arbol) == 1);
    print_test("Obtener clave vacia es valor", abb_obtener(arbol, clave) == valor);
    print_test("Pertenece clave vacia, es true", abb_pertenece(arbol, clave));
    print_test("Borrar clave vacia, es valor", abb_borrar(arbol, clave) == valor);
    print_test("La cantidad de elementos es 0", abb_cantidad(arbol) == 0);

    abb_destruir(arbol);
}

static void prueba_abb_valor_null() {
	printf("\n> prueba valor igual a NULL\n");

    abb_t* arbol = abb_crear(strcmp, NULL);

    char *clave = "", *valor = NULL;

    /* Inserta 1 valor y luego lo borra */
    print_test("Guardar clave vacia valor NULL", abb_guardar(arbol, clave, valor));
    print_test("La cantidad de elementos es 1", abb_cantidad(arbol) == 1);
    print_test("Obtener clave vacia es valor NULL", abb_obtener(arbol, clave) == valor);
    print_test("Pertenece clave vacia, es true", abb_pertenece(arbol, clave));
    print_test("Borrar clave vacia, es valor NULL", abb_borrar(arbol, clave) == valor);
    print_test("La cantidad de elementos es 0", abb_cantidad(arbol) == 0);

    abb_destruir(arbol);
}

// void _prueba_abb_volumen(size_t cantidad_elementos, size_t cantidad_digitos) {
// 	hash_t* elem_pertenecen = hash_crear(NULL);
// 	hash_t* elem_no_pertenecen = hash_crear(NULL);
// 	abb_t* arbol = abb_crear(strcmp, NULL);

// 	abb_destruir(arbol);
// 	char* claves[cantidad_elementos];
// 	int valores[cantidad_elementos];
// 	bool resultado_guardar = true;
// 	bool resultado_reemplazar = true;

// 	for (size_t i = 0; i < cantidad_elementos;) claves[i] = malloc(sizeof(char) * (cantidad_digitos + 1));

// 	for (size_t i = 0; i < cantidad_elementos;) {
// 		valores[i] = rand() % (cantidad_elementos * 2);
// 	 	sprintf(claves[i], "%d", valores[i]);
// 		if (!hash_pertenece(elem_pertenecen, claves[i])) {

// 			i++;
// 		} else {
// 			hash_guardar(elem_no_pertenecen, )
// 		}
// 	}
// 	// for (size_t i = 0; i < cantidad_elementos;) {
// 	// 	valores[i] = rand() % (cantidad_elementos * 2);
// 	// 	printf("%i\n", valores[i]);
// 	// 	claves[i] = malloc(sizeof(char) * (cantidad_digitos + 1));
// 	// 	sprintf(claves[i], "%d", valores[i]); //primero hay que reservar espacio! error
// 	// 	printf("en cadena es: %s\n", claves[i]);
// 	// }

// 	for (size_t i = 0; i < cantidad_elementos; i++) {
// 		free(claves[i]);
// 	}
// }

static void prueba_abb_volumen() {

	printf("\n> prueba de volumen (5000 elementos)\n");
	_prueba_abb_volumen(100, 3);

	printf("\n> prueba de volumen (10000 elementos)\n");
	//_prueba_abb_volumen(10000);
}

void nuestras_pruebas_primitivas_abb() {
	prueba_buscar_nodo();
	pruebas_borrar();
}

void pruebas_primitivas_abb() {
	prueba_crear_abb_vacio();
	prueba_abb_guardar(); //eliminable maybe, lo voy a hacer en prueba de volumen 
	prueba_abb_insertar();
	prueba_abb_reemplazar();
	prueba_abb_reemplazar_con_destruir();
	prueba_abb_borrar();
	prueba_abb_clave_vacia();
	prueba_abb_valor_null();
	prueba_abb_volumen();
}

// void pruebas_iterador_externo() {

// }

// void pruebas_iterador_interno() {

// }

void pruebas_abb_estudiante() {

	//nuestras_pruebas_primitivas_abb();
	
	printf("PRUEBAS PRIMITIVAS ARBOL:\n");
	pruebas_primitivas_abb();

	//printf("\nPRUEBAS ITERADOR EXTERNO:\n");
	//pruebas_iterador_externo();

	//printf("\nPRUEBAS ITERADOR INTERNO:\n");
	//pruebas_iterador_interno();
}

//#ifndef CORRECTOR

int main(void) {
    pruebas_abb_estudiante();
    return failure_count() > 0;
}

//#endif
