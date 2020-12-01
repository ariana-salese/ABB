#define _POSIX_C_SOURCE 200809L
#include "abb.h"
#include <stdlib.h>
#include <string.h>

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

/* ******************************************************************
 *                       FUNCIONES AUXILIARES
 * *****************************************************************/

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

abb_nodo_t* abb_nodo_crear(const char* clave, void* dato) {
    abb_nodo_t* abb_nodo = malloc(sizeof(abb_nodo_t));
    if (abb_nodo) return NULL;

    char* copia_clave = strdup(clave);
    if(!copia_clave) return NULL;

    abb_nodo->clave = copia_clave;
    abb_nodo->dato = dato;
    abb_nodo->der = NULL;
    abb_nodo->izq = NULL;

    return abb_nodo;
}

void ubicar_nodo(abb_comparar_clave_t cmp, abb_nodo_t* nodo_ant, abb_nodo_t* nodo_act, abb_nodo_t* nodo_nuevo) {
    if (!nodo_act) {
        if (cmp(nodo_nuevo->clave, nodo_ant->clave) > 0) nodo_act->der = nodo_nuevo; //si la clave nueva es mayor a la anterior
        else nodo_act->izq = nodo_nuevo;
        return;
    }
    if (cmp(nodo_act->clave, nodo_nuevo->clave) > 0) ubicar_nodo(cmp, nodo_act, nodo_act->izq, nodo_nuevo);//acual > ant 
    else if (cmp(nodo_act->clave, nodo_nuevo->clave) < 0) ubicar_nodo(cmp, nodo_act, nodo_act->der, nodo_nuevo); //actual < ant
    else { //act == ant
        nodo_act->dato = nodo_nuevo->dato;
        return;
    }
}

bool abb_guardar(abb_t *arbol, const char *clave, void *dato) {
    abb_nodo_t* nuevo_abb_nodo = abb_nodo_crear(clave, dato);
    if (!nuevo_abb_nodo) return false;

    if (arbol->raiz) {
        ubicar_nodo(arbol->cmp ,NULL, arbol->raiz, nuevo_abb_nodo);
    } else {
        arbol->raiz = nuevo_abb_nodo;
    }

    arbol->cantidad++;

    return true;
}

/*
void *abb_borrar(abb_t *arbol, const char *clave) {

}

void *abb_obtener(const abb_t *arbol, const char *clave) {

}

bool abb_pertenece(const abb_t *arbol, const char *clave) {

}
*/

size_t abb_cantidad(abb_t *arbol) {
    return arbol->cantidad;
}

/*
void abb_destruir(abb_t *arbol) {

}
*/

/* *****************************************************************
 *                   PRIMITIVAS DEL ITERADOR EXTERNO
 * *****************************************************************/

/*
abb_iter_t *abb_iter_in_crear(const abb_t *arbol) {

}

bool abb_iter_in_avanzar(abb_iter_t *iter) {

}

const char *abb_iter_in_ver_actual(const abb_iter_t *iter) {

}

bool abb_iter_in_al_final(const abb_iter_t *iter) {

}

void abb_iter_in_destruir(abb_iter_t* iter) {

}
*/

/* *****************************************************************
 *                      DECLARACION ITERADOR INTERNO
 * *****************************************************************/
 
/*
void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra) {

}
*/
