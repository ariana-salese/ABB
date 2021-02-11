#include "pila.h"
#include <stdlib.h>
#include <stdio.h>

#define FACT_INCREMENTO 2
#define FACT_DECREMENTO 2
#define DISP_DECREMENTO 4 //DISP por "disparador" o "trigger"
#define CAPACIDAD_INIC 10

bool pila_redimensionar(pila_t* pila, size_t tam_nuevo);

size_t pila_capacidad(pila_t* pila);

/* Definición del struct pila proporcionado por la cátedra.
 */
struct pila {
    void **datos;
    size_t cantidad;   // Cantidad de elementos almacenados.
    size_t capacidad;  // Capacidad del arreglo 'datos'.
};

pila_t* pila_crear(void){

	pila_t* pila = malloc(sizeof(pila_t));

	if(pila == NULL){
		return NULL;
	}

	pila->datos = malloc(CAPACIDAD_INIC * sizeof(void*));

	if (pila->datos == NULL){
		free(pila);
		return NULL;
	}

	pila->cantidad = 0;
	pila->capacidad = CAPACIDAD_INIC;

	return pila;
}

void pila_destruir(pila_t* pila){
	free(pila->datos);
	free(pila);
}

bool pila_esta_vacia(const pila_t* pila){
	return pila->cantidad == 0;
}

bool pila_apilar(pila_t* pila, void* valor){

	if(pila->cantidad == pila->capacidad){
		if(!pila_redimensionar(pila,FACT_INCREMENTO * pila->capacidad)){
			return false;
		}
	}

	pila->datos[pila->cantidad] = valor;
	pila->cantidad++;

	return true;
}

void* pila_ver_tope(const pila_t* pila){

	if(pila_esta_vacia(pila)){
		return NULL;
	}

	return pila->datos[pila->cantidad - 1];
}

void* pila_desapilar(pila_t* pila){

	if(pila->cantidad == 0){
		return NULL;
	}

	void* dato = pila->datos[pila->cantidad - 1];
	pila->cantidad--;

	if(pila->cantidad <= pila->capacidad/DISP_DECREMENTO && pila->cantidad != 0){
		pila_redimensionar(pila, pila->capacidad/FACT_DECREMENTO);
	}

	return dato;
}

bool pila_redimensionar(pila_t* pila, size_t tam_nuevo){
	void** datos_nuevo = realloc(pila->datos, tam_nuevo * sizeof(void*));

	if (datos_nuevo == NULL){
		return false;
	}

	pila->capacidad = tam_nuevo;
	pila->datos = datos_nuevo;

	return true;
}

