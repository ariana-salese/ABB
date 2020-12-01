#include "abb.h"
#include <stdlib.h>

typedef struct abb_nodo {
    struct abb_nodo* der;
    struct abb_nodo* izq;
    char* clave;
    void* dato; 
} abb_nodo_t;

struct abb {
    abb_nodo_t* raiz;
    abb_comparar_clave_t* cmp;
    abb_destruir_dato_t* destruir_dato;
    size_t cantidad;
};

abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato) {
    abb_t* arbol = malloc(sizeof(abb_t));
    if (!arbol) return NULL;
    arbol->cmp = cmp;
    arbol->destruir_dato = destruir_dato;
    arbol->raiz = NULL;
    arbol->cantidad = 0;
    return arbol; 
}

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

/*

bool abb_guardar(abb_t *arbol, const char *clave, void *dato) {

}

void *abb_borrar(abb_t *arbol, const char *clave) {

}

void *abb_obtener(const abb_t *arbol, const char *clave) {

}

bool abb_pertenece(const abb_t *arbol, const char *clave) {

}

size_t abb_cantidad(abb_t *arbol) {

}

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

