#define _POSIX_C_SOURCE 200809L
#include "abb.h"
#include "cola.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct abb_nodo {
    struct abb_nodo* der;
    struct abb_nodo* izq;
    char* clave;
    void* dato; 
} abb_nodo_t;

struct abb {
    abb_nodo_t* raiz;
    abb_comparar_clave_t cmp;
    abb_destruir_dato_t destruir_dato;
    size_t cantidad;
};

struct abb_iter {
    const abb_t* arbol;
    cola_t* cola;
};

typedef enum accion {
    NO_BORRAR, BORRAR
} accion_t;

typedef enum cantidad_hijos {
    CERO_HIJOS, UN_HIJO, DOS_HIJOS
} cantidad_hijos_t;

/* ******************************************************************
 *                       FUNCIONES AUXILIARES
 * *****************************************************************/

//Devuelve la cantidad de hijos del nodo pasado por parametro. 
size_t contar_hijos(abb_nodo_t* nodo) {
    if (!nodo->der && !nodo->izq) return CERO_HIJOS;
    if (nodo->der && nodo->izq) return DOS_HIJOS;
    return UN_HIJO;
}

//Recibe por parametro un nodo con uno o cero hijos. Devuelve el unico hijo del nodo o
//NULL si tiene cero hijos. 
abb_nodo_t* buscar_hijo_unico(abb_nodo_t* nodo) {
    if (nodo->der) return nodo->der;
    if (nodo->izq) return nodo->izq;
    return NULL;
}

//Busca un reemplazo del nodo. Si tiene hijo derecho, devuelve el menor a su derecha, 
//caso contrario, el mayor a su izquierda. 
abb_nodo_t* buscar_reemplazo(abb_nodo_t* nodo) {
    abb_nodo_t* nodo_act;
    if (nodo->der) {
        nodo_act = nodo->der;
        while (nodo_act->izq) nodo_act = nodo_act->izq;
    } else {
        nodo_act = nodo->izq;
        while (nodo_act->der) nodo_act = nodo_act->der;
    }
    return nodo_act;
}

//Destruye el nodo pasado por parametro.
//Post: todos sus datos asociados fueron liberados 
void abb_nodo_destruir(abb_nodo_t* nodo, abb_destruir_dato_t destruir_dato, accion_t borrar) {
    if (destruir_dato && borrar) destruir_dato(nodo->dato);
    free(nodo->clave);
    free(nodo);
}

//Destruye cada uno de los nodos del arbol
void _abb_destruir(abb_nodo_t* nodo, abb_destruir_dato_t destruir_dato, abb_t *arbol) {
	if (!nodo) return;

	_abb_destruir(nodo->izq, destruir_dato, arbol);
	_abb_destruir(nodo->der, destruir_dato, arbol);

    abb_nodo_destruir(nodo, destruir_dato, BORRAR);
}

//Crea un nodo para el abb. 
abb_nodo_t* abb_nodo_crear(const char* clave, void* dato) {
    abb_nodo_t* abb_nodo = malloc(sizeof(abb_nodo_t));
    if (!abb_nodo) return NULL;

    char* copia_clave = strdup(clave);
    if (!copia_clave) {
        free(abb_nodo);
        return NULL;
    }

    abb_nodo->clave = copia_clave;
    abb_nodo->dato = dato;
    abb_nodo->der = NULL;
    abb_nodo->izq = NULL;

    return abb_nodo;
}

//Busca un nodo cuya clave coincide con la pasada por parametro. Si la clave no se encuentra devuelve
//NULL. Si se pasa por parametro BORRAR, el nodo buscado se elimina.
//Post: la condicion de abb se sigue cumpliendo. 
abb_nodo_t* buscar_nodo(abb_comparar_clave_t cmp, abb_nodo_t* nodo_act, abb_nodo_t* nodo_ant, const char* clave, accion_t borrar) {
    if (!nodo_act) return NULL;

    int resultado_cmp_act = cmp(nodo_act->clave, clave);

    if (resultado_cmp_act > 0) {
        return buscar_nodo (cmp, nodo_act->izq, nodo_act, clave, borrar);
    }
    else if (resultado_cmp_act < 0) {
        return buscar_nodo (cmp, nodo_act->der, nodo_act, clave, borrar);
    }
    
    if (!borrar) return nodo_act;

    int resultado_cmp_ant = 0;
    if (nodo_ant) resultado_cmp_ant = cmp(clave, nodo_ant->clave);
    size_t cant_hijos = contar_hijos(nodo_act);
    abb_nodo_t* reemplazo = NULL;

    if ((cant_hijos == CERO_HIJOS || cant_hijos == UN_HIJO) && nodo_ant) {
        reemplazo = buscar_hijo_unico(nodo_act);

        if (resultado_cmp_ant > 0) nodo_ant->der = reemplazo;
        else nodo_ant->izq = reemplazo; 

        return nodo_act;
    }
    
    reemplazo = buscar_reemplazo(nodo_act);
    abb_nodo_t* nodo_a_devolver = abb_nodo_crear(nodo_act->clave, nodo_act->dato);
    if (!nodo_a_devolver) return NULL;

    buscar_nodo(cmp, nodo_act, nodo_ant, reemplazo->clave, BORRAR);
    
    free(nodo_act->clave);
    nodo_act->clave = reemplazo->clave;
    nodo_act->dato = reemplazo->dato;
    free(reemplazo);

    return nodo_a_devolver;
}

// Ubica el nuevo nodo cumpliendo con la condicion de abb. En caso de ya encontrarse la clave del 
// nuevo nodo, reemplaza su dato. 
void ubicar_nodo(abb_t* arbol, abb_nodo_t* nodo_ant, abb_nodo_t* nodo_act, abb_nodo_t* nodo_nuevo) {
    if (!nodo_act) {
        if (arbol->cmp(nodo_nuevo->clave, nodo_ant->clave) > 0) nodo_ant->der = nodo_nuevo;      
        else nodo_ant->izq = nodo_nuevo;
        return;
    }
    int resultado_cmp = arbol->cmp(nodo_nuevo->clave, nodo_act->clave);

    if (resultado_cmp > 0) {
    	ubicar_nodo(arbol, nodo_act, nodo_act->der, nodo_nuevo); //actual > nueva
    } 
    else if (resultado_cmp < 0) {
    	ubicar_nodo(arbol, nodo_act, nodo_act->izq, nodo_nuevo); //actual < nueva
    } 
    else { //actual == nueva
        if (arbol->destruir_dato) arbol->destruir_dato(nodo_act->dato);
        nodo_act->dato = nodo_nuevo->dato;
        abb_nodo_destruir(nodo_nuevo, arbol->destruir_dato, NO_BORRAR);
        arbol->cantidad--;
        return;
    }
}

//Completa la cola recibida por parametro, si claves es true, con las claves de los nodos
//del arbol. Si claves es false, encola los nodos.
void poblar_cola_iter(cola_t* cola, abb_nodo_t* nodo, bool claves) {
	
	if (!nodo) return;

	poblar_cola_iter(cola, nodo->izq, claves);
	if (claves) cola_encolar(cola, nodo->clave);
    else cola_encolar(cola, nodo);
	poblar_cola_iter(cola, nodo->der, claves);
}

/* ******************************************************************
 *                       PRIMITIVAS DEL ABB
 * *****************************************************************/

abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato) {
    abb_t* arbol = malloc(sizeof(abb_t));
    if (!arbol) return NULL;

    arbol->cmp = cmp;
    arbol->destruir_dato = destruir_dato;
    arbol->raiz = NULL;
    arbol->cantidad = 0;

    return arbol; 
}

bool abb_guardar(abb_t *arbol, const char *clave, void *dato) {
    abb_nodo_t* nuevo_abb_nodo = abb_nodo_crear(clave, dato);
    if (!nuevo_abb_nodo) return false;

    if (arbol->raiz) {
        ubicar_nodo(arbol, NULL, arbol->raiz, nuevo_abb_nodo);
    } else {
        arbol->raiz = nuevo_abb_nodo;
    }

    arbol->cantidad++;

    return true;
}

void *abb_borrar(abb_t *arbol, const char *clave) {
    abb_nodo_t* nodo_a_eliminar = NULL;

    if (arbol->cantidad == 1 && arbol->cmp(arbol->raiz->clave, clave) == 0) {
        nodo_a_eliminar = arbol->raiz;
    } else {
        nodo_a_eliminar = buscar_nodo(arbol->cmp, arbol->raiz, NULL, clave, BORRAR);
    }
    
    if (!nodo_a_eliminar) return NULL;
    
    void* dato_a_devolver = nodo_a_eliminar->dato;
    abb_nodo_destruir(nodo_a_eliminar, arbol->destruir_dato, NO_BORRAR);

    arbol->cantidad--;
    if (arbol->cantidad == 0) arbol->raiz = NULL;
    
    return dato_a_devolver;
}   

void *abb_obtener(const abb_t *arbol, const char *clave) {
    abb_nodo_t* nodo = buscar_nodo(arbol->cmp, arbol->raiz, NULL, clave, NO_BORRAR);

    if (!nodo) return NULL;
    return nodo->dato;
}

bool abb_pertenece(const abb_t *arbol, const char *clave) {
    abb_nodo_t* nodo = buscar_nodo(arbol->cmp, arbol->raiz, NULL, clave, NO_BORRAR);

    if (!nodo) return false;
    return true;
}

size_t abb_cantidad(abb_t *arbol) {
    return arbol->cantidad;
}

void abb_destruir(abb_t *arbol) {
	_abb_destruir(arbol->raiz, arbol->destruir_dato, arbol);
	free(arbol);
}

/* *****************************************************************
 *                   PRIMITIVAS DEL ITERADOR EXTERNO
 * *****************************************************************/

abb_iter_t *abb_iter_in_crear(const abb_t *arbol) {
	abb_iter_t *iter = malloc(sizeof(abb_iter_t));

	if (!iter) return NULL;

	iter->cola = cola_crear();

	if (!iter->cola) {
		free(iter);
		return NULL;
	}

	iter->arbol = arbol;
	poblar_cola_iter(iter->cola, arbol->raiz, true);

	return iter;
}

bool abb_iter_in_avanzar(abb_iter_t *iter) {
	if (cola_esta_vacia(iter->cola)) return false;

	cola_desencolar(iter->cola);
	return true;
}

const char *abb_iter_in_ver_actual(const abb_iter_t *iter) {
	if (abb_iter_in_al_final(iter)) return NULL;
	return cola_ver_primero(iter->cola);
}

bool abb_iter_in_al_final(const abb_iter_t *iter) {
	return cola_esta_vacia(iter->cola);
}

void abb_iter_in_destruir(abb_iter_t* iter) {
	cola_destruir(iter->cola, NULL);
	free(iter);
}

/* *****************************************************************
 *                   DECLARACION ITERADOR INTERNO
 * *****************************************************************/

void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra) {

    if (!arbol->raiz) return;
	
    cola_t* nodos = cola_crear();
    if (!nodos) return; 

    poblar_cola_iter(nodos, arbol->raiz, false);

    while (!cola_esta_vacia(nodos)) {
        abb_nodo_t* nodo_act = (abb_nodo_t*)cola_desencolar(nodos); 
        if (!visitar(nodo_act->clave, nodo_act->dato, extra)) break;
    }

    cola_destruir(nodos, NULL);
}
