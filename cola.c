#include "cola.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct nodo{
	void* dato;
	struct nodo* sig;
}nodo_t;

struct cola{
	nodo_t *prim;
	nodo_t *ult;
};

static nodo_t* nodo_crear(void* valor);

cola_t* cola_crear(void){

	cola_t* cola = malloc(sizeof(cola_t));

	if(!cola) return NULL;

	cola->prim = NULL;
	cola->ult = NULL;

	return cola;
}

void cola_destruir(cola_t* cola, void (*destruir_dato)(void*)){
	
	while (!cola_esta_vacia(cola)){	
		void* dato = cola_desencolar(cola);
		if(destruir_dato) destruir_dato(dato);
	}

	free(cola);
}

bool cola_esta_vacia(const cola_t *cola){
	return cola->prim == NULL;
}

bool cola_encolar(cola_t* cola, void* valor){

	nodo_t* nodo = nodo_crear(valor);

	if(!nodo) return false;

	if(!cola_esta_vacia(cola)){
		cola->ult->sig = nodo;
	}
	else{
		cola->prim = nodo;
	}
	
	cola->ult = nodo;

	return true;
}

void* cola_ver_primero(const cola_t *cola){
	if(cola_esta_vacia(cola)){
		return NULL;	
	}
	return cola->prim->dato;
}

void* cola_desencolar(cola_t *cola){

	if(cola_esta_vacia(cola)) return NULL;

	void* dato = cola->prim->dato;
	
	nodo_t* nuevo_prim = cola->prim->sig;
	
	if(cola->prim == cola->ult) cola->ult = NULL;

	free(cola->prim);

	cola->prim = nuevo_prim;

	return dato;
}

static nodo_t* nodo_crear(void* valor){

	nodo_t* nodo = malloc(sizeof(nodo_t));

	if(!nodo) return NULL;

	nodo->dato = valor;
	nodo->sig = NULL;

	return nodo;
}