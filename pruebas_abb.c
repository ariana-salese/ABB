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
	abb_t* abb = abb_crear(strcmp, NULL);

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
	//char* claves_no_abb[] = {"10", "45", "90", "0", "89", "45", "35", "88", "24", "54", "56", "99", "78", "77", "66"};
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
 *                      FUNCIONES AUXILIARES
 * *****************************************************************/

bool sumar_n(const char* clave, void* dato, void* extra) {
	int dato_act = *(int*)dato;

	if (dato_act > 30) return false;

	*(int*)extra += dato_act;
	return true;
}

bool obtener_dato(const char* clave, void* dato, void* extra) {
	lista_insertar_ultimo(extra, dato);
	return true;
}

bool duplicar_dato(const char* clave, void* dato, void* extra) {
	*(int*)dato = *(int*)dato * 2;
	return true;
}

bool imprimir_clave(const char* clave, void* dato, void* extra) {
	if (strcmp(clave, (char*)extra) == 0)  {	
		printf("\n");
		return false;
	}
	printf("%s ", clave);
	return true;
}

/* ******************************************************************
 *                      PRUEBAS PRIMITIVAS ABB
 * *****************************************************************/

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

	/* Inserta otros 2 valores y no los borra (se destruyen con el arbol) */
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

    /* Se destruye el arbol (se debe liberar lo que quedó dentro) */
    abb_destruir(arbol);
}


static void prueba_abb_borrar() {
	printf("\n> prueba borrar\n");

	abb_t* arbol = abb_crear(strcmp, NULL);

	char* claves[] = {"23", "34", "28", "42", "12", "16", "15", "19", "21", "55", "09", "20", "22", "48", "6"};
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

static void prueba_abb_destruir_con_free() {
	printf("\n> prueba destruir abb liberando datos con free\n");

    abb_t* arbol = abb_crear(strcmp, free);
	char* claves[] = {"23", "34", "28", "42", "12", "16", "15", "19", "21", "55", "09", "20", "22", "48", "6"};
	char* datos[CANTIDAD_ELEMENTOS];

	print_test("El arbol se creo vacio", arbol);

	/* pide memoria para cada dato */
	for (size_t i = 0; i < CANTIDAD_ELEMENTOS; i++) datos[i] = malloc(sizeof(char) * CANTIDAD_ELEMENTOS);

	bool resultado_guardar = true;

	/* guarda los elementos */
	for (size_t i = 0; i < CANTIDAD_ELEMENTOS; i++) if (!abb_guardar(arbol, claves[i], datos[i])) resultado_guardar = false;
	
	print_test("Se guardaron los elementos con datos dinamicos", resultado_guardar);

	abb_destruir(arbol);
	printf("Se destruyo el arbol y sus elementos\n");
}

void _lista_destruir (void* lista) {
	lista_destruir(lista, NULL);
}

static void prueba_abb_destruir_con_otra_funcion_de_destruccion() {
	printf("\n> prueba destruir abb liberando datos con otra funcion\n");

	char* claves[] = {"23", "34", "28", "42", "12", "16", "15", "19", "21", "55", "09", "20", "22", "48", "6"};
	lista_t* datos[CANTIDAD_ELEMENTOS];
    abb_t* arbol = abb_crear(strcmp, _lista_destruir);

	print_test("El arbol se creo vacio", arbol);

	/* crea cada lista */
	for (size_t i = 0; i < CANTIDAD_ELEMENTOS; i++) datos[i] = lista_crear();

	bool resultado_guardar = true;

	/* guarda los elementos */
	for (size_t i = 0; i < CANTIDAD_ELEMENTOS; i++) if (!abb_guardar(arbol, claves[i], datos[i])) resultado_guardar = false;
	
	print_test("Se guardaron los elementos con datos dinamicos", resultado_guardar);

	abb_destruir(arbol);
	printf("Se destruyo el arbol y sus elementos\n");

}

void _prueba_abb_volumen(size_t cantidad_elementos, size_t cantidad_digitos) {
	/* crea estructura auxiliar */
	hash_t* elem_pertenecen = hash_crear(NULL);

	char* claves[cantidad_elementos];
	int valores[cantidad_elementos];
	int max = 3;

	abb_t* arbol = abb_crear(strcmp, NULL);

	//GUARDA ELEMENTOS
	/* pruebas */
	bool resultado_guardar = true;
	bool resultado_obtener = true;
	bool resultado_cantidad = true;
	bool resultado_pertenece = true;
	bool resultado_no_pertenece = true;
	
	/* pide memoria para las claves */
	for (size_t i = 0; i < cantidad_elementos; i++) claves[i] = malloc(sizeof(char) * (cantidad_digitos + 1));

	for (size_t i = 0; i < cantidad_elementos;) {
		valores[i] = rand() % (cantidad_elementos * max);
	 	sprintf(claves[i], "%d", valores[i]);

		if (hash_pertenece(elem_pertenecen, claves[i])) {
			free(claves[i]);
			claves[i] = malloc(sizeof(char) * (cantidad_digitos + 1));
			continue;
		}

		if (abb_pertenece(arbol, claves[i])) resultado_no_pertenece = false;
		if (!abb_guardar(arbol, claves[i], &valores[i])) resultado_guardar = false;
		if (*(int*)abb_obtener(arbol, claves[i]) != valores[i]) resultado_obtener = false;
		if (!abb_pertenece(arbol, claves[i])) resultado_pertenece = false;
		hash_guardar(elem_pertenecen, claves[i], &valores[i]);
		i++;
		if (abb_cantidad(arbol) != i) resultado_cantidad = false;
	}

	print_test("Previo a guardar no pertenecen", resultado_no_pertenece);
	print_test("Guardar todos los elementos", resultado_guardar);
	print_test("Todos pertenecen", resultado_pertenece);
	print_test("La cantidad de elementos se mantuvo correcta", resultado_cantidad);
	print_test("Obtener cada una de las claves devuelve su valor", resultado_obtener);
	
	//REEMPLAZA ELEMENTOS
	hash_iter_t* iter_reemplazar = hash_iter_crear(elem_pertenecen);
	int reemplazos[cantidad_elementos/2];
	char* claves_modificadas[cantidad_elementos/2];

	/* pruebas */
	bool resultado_reemplazar = true;
	resultado_guardar = true;
	resultado_cantidad = true;
	resultado_pertenece = true;

	for (size_t i = 0; i < cantidad_elementos / 2; i++) {
	
		const char* clave_actual = hash_iter_ver_actual(iter_reemplazar);
		char* clave_actual_copia = strdup(clave_actual);
		claves_modificadas[i] = clave_actual_copia;

		reemplazos[i] = rand() % (cantidad_elementos * max);

		if (!abb_guardar(arbol, clave_actual, &reemplazos[i])) resultado_guardar = false;
		if (!abb_pertenece(arbol, clave_actual)) resultado_pertenece = false;
		if (abb_cantidad(arbol) != cantidad_elementos) resultado_cantidad = false;
		if (*(int*)abb_obtener(arbol, clave_actual) != reemplazos[i]) resultado_reemplazar = false;

		hash_iter_avanzar(iter_reemplazar);
	}

	/* actualiza valores */
	for (size_t i = 0; i < cantidad_elementos / 2; i++) hash_guardar(elem_pertenecen, claves_modificadas[i], &reemplazos[i]);

	print_test("Reemplazar mitad de los datos", resultado_guardar);
	print_test("Se mantuvo la cantidad de elementos", resultado_cantidad);
	print_test("Los datos se modificaron", resultado_reemplazar);
	print_test("Las claves no se modificaron", resultado_pertenece);

	//BORRA ELEMENTOS
	hash_iter_t* iter_borrar = hash_iter_crear(elem_pertenecen);

	/* pruebas */
	bool resultado_borrar = true;
	bool resultado_borrar_segunda_vez = true;
	resultado_cantidad = true;
	resultado_no_pertenece = true;

	for (size_t i = 0; i < cantidad_elementos; i++) {
		const char* clave_actual = hash_iter_ver_actual(iter_borrar);

		if (*(int*)abb_borrar(arbol, clave_actual) != *(int*)hash_obtener(elem_pertenecen, clave_actual)) resultado_borrar = false;
		if (abb_borrar(arbol, clave_actual)) resultado_borrar_segunda_vez = false;
		if (abb_cantidad(arbol) != cantidad_elementos - i - 1) resultado_cantidad = false;
		if (abb_pertenece(arbol, clave_actual)) resultado_no_pertenece = false;

		hash_iter_avanzar(iter_borrar);
	}

	print_test("Se borraron todos los elementos", resultado_borrar);
	print_test("Borrar una segunda vez devuelve NULL", resultado_borrar_segunda_vez);
	print_test("la cantidad se actualizo correctamente", resultado_cantidad);
	print_test("Los elementos borrados ya no pertenecen", resultado_no_pertenece);
	print_test("la cantidad es 0", abb_cantidad(arbol) == 0);

	//LIBERA MEMORIA
	for (size_t i = 0; i < cantidad_elementos; i++) free(claves[i]);

	for (size_t i = 0; i< cantidad_elementos / 2; i++) free(claves_modificadas[i]);

	hash_iter_destruir(iter_reemplazar);
	hash_iter_destruir(iter_borrar);
	hash_destruir(elem_pertenecen);
	abb_destruir(arbol);
}

static void prueba_abb_volumen() {

	printf("\n> prueba de volumen (5000 elementos)\n");
	_prueba_abb_volumen(5000, 5);

	printf("\n> prueba de volumen (10000 elementos)\n");
	_prueba_abb_volumen(10000, 5);
}

/* ******************************************************************
 *                    PRUEBAS ITERADOR EXTERNO
 * *****************************************************************/

/* ******************************************************************
 *                    PRUEBAS ITERADOR INTERNO
 * *****************************************************************/

static void prueba_iterar_int() {
	abb_t* arbol = abb_crear(strcmp, NULL);

	char* claves[] = {"23", "34", "28", "42", "12", "16", "15", "19", "21", "55", "09", "20", "22", "48", "06"};
	int datos[] = {23, 34, 28, 42, 12, 16, 15, 19, 21, 55, 9, 20, 22, 48, 6};
	bool resultado_guardar = true;

	for(int i = 0; i < CANTIDAD_ELEMENTOS; i++) {
		if (!abb_guardar(arbol, claves[i], &datos[i])) resultado_guardar = false;
	}

	print_test("Se guardaron algunos elementos", resultado_guardar);

	/* con extra hasta x dato */
	printf("\n> prueba iterar con extra y corte segun dato\n");

	int suma_total = 191;
	int suma_total_iter = 0;

	abb_in_order(arbol, sumar_n, &suma_total_iter);

	print_test("Con funcion visitar (sumar todos)", suma_total_iter == suma_total);

	/* con extra hasta x clave */
	printf("\n> prueba iterar con extra y corte segun clave\n");

	printf("Con funcion visitar imprimir_claves, resultado esperado: 06 09 12 15 16 19\n");

	printf("Resultado obtenido: ");
	abb_in_order(arbol, imprimir_clave, "20");

	/* con extra y sin corte */
	printf("\n> prueba iterar con extra y sin corte\n");

	int datos_en_orden[] = {6, 9, 12, 15, 16, 19, 20, 21, 22, 23, 28, 34, 42, 48, 55};
	lista_t* datos_en_orden_iter = lista_crear();

	abb_in_order(arbol, obtener_dato, datos_en_orden_iter);

	print_test("Con funcion visitar (obtener datos)", lista_largo(datos_en_orden_iter) == CANTIDAD_ELEMENTOS);

	bool resultado_orden = true;
	for (size_t i = 0; i < CANTIDAD_ELEMENTOS; i++) {
		if (datos_en_orden[i] != *(int*)lista_borrar_primero(datos_en_orden_iter)) resultado_orden = false;
	}

	print_test("Los datos quedaron en el orden correcto", resultado_orden);

	lista_destruir(datos_en_orden_iter, NULL);

	/* sin extra y modificando datos */
	printf("\n> prueba iterar sin extra y modificando datos\n");

	bool resultado_modificar = true;
	abb_in_order(arbol, duplicar_dato, NULL);

	for (size_t i = 0; i < CANTIDAD_ELEMENTOS; i++) {
		if (*(int*)abb_obtener(arbol, claves[i]) != datos[i]) resultado_modificar = false;
	}

	print_test("Con funcion visitar (duplicar dato)", resultado_modificar);

	abb_destruir(arbol);
}

void nuestras_pruebas_primitivas_abb() {
	prueba_buscar_nodo();
	pruebas_borrar();
}

void pruebas_primitivas_abb() {
	prueba_crear_abb_vacio();
	prueba_abb_insertar();
	prueba_abb_reemplazar();
	prueba_abb_reemplazar_con_destruir();
	prueba_abb_borrar();
	prueba_abb_clave_vacia();
	prueba_abb_valor_null();
	prueba_abb_destruir_con_free();
	prueba_abb_destruir_con_otra_funcion_de_destruccion();
	prueba_abb_volumen();
}

// void pruebas_iterador_externo() {
//     prueba_iter_ext_abb_vacio();
//     prueba_abb_iterar_ext();
//     prueba_abb_iterar_ext_volumen(5000);
// }

void pruebas_iterador_interno() {
	prueba_iterar_int();
}

void pruebas_abb_estudiante() {

	//nuestras_pruebas_primitivas_abb();
	
	printf("PRUEBAS PRIMITIVAS ARBOL:\n");
	pruebas_primitivas_abb();

	//printf("\nPRUEBAS ITERADOR EXTERNO:\n");
	//pruebas_iterador_externo();

	printf("\nPRUEBAS ITERADOR INTERNO:\n");
	pruebas_iterador_interno();
}

//#ifndef CORRECTOR

int main(void) {
    pruebas_abb_estudiante();
    return failure_count() > 0;
}

//#endif
