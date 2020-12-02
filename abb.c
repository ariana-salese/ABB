#define _POSIX_C_SOURCE 200809L
#include "abb.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h> //Para imprimir, se puede sacar

#define COMPACT //Para imprimir, se puede sacar

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
 *                 IMPRIMIR ARBOL (BORRAR AL FINAL)
 * *****************************************************************/

size_t _abb_altura(abb_nodo_t *nodo, size_t altura) { //Es para saber hasta donde imprimir

    if(!nodo) return altura;
    
    size_t alt_izq = _abb_altura(nodo->izq, altura + 1);
    size_t alt_der = _abb_altura(nodo->der, altura + 1);
    
    return alt_izq > alt_der ? alt_izq : alt_der;
}

size_t abb_altura(const abb_t* abb) {
    return _abb_altura(abb->raiz,0);
}

int _print_t(abb_nodo_t *nodo, int is_izq, int offset, int depth, char s[20][255]) {
    char b[20];
    int width = 5;

    if (!nodo) return 0;

    sprintf(b, "(%03d)", *((int*)nodo->dato));

    int izq  = _print_t(nodo->izq,  1, offset,             depth + 1, s);
    int der = _print_t(nodo->der, 0, offset + izq + width, depth + 1, s);

#ifdef COMPACT
    for (int i = 0; i < width; i++)
        s[depth][offset + izq + i] = b[i];

    if (depth && is_izq) {

        for (int i = 0; i < width + der; i++)
            s[depth - 1][offset + izq + width/2 + i] = '-';

        s[depth - 1][offset + izq + width/2] = '.';

    } else if (depth && !is_izq) {

        for (int i = 0; i < izq + width; i++)
            s[depth - 1][offset - width/2 + i] = '-';

        s[depth - 1][offset + izq + width/2] = '.';
    }
#else
    for (int i = 0; i < width; i++)
        s[2 * depth][offset + izq + i] = b[i];

    if (depth && is_izq) {

        for (int i = 0; i < width + der; i++)
            s[2 * depth - 1][offset + izq + width/2 + i] = '-';

        s[2 * depth - 1][offset + izq + width/2] = '+';
        s[2 * depth - 1][offset + izq + width + der + width/2] = '+';

    } else if (depth && !is_izq) {

        for (int i = 0; i < izq + width; i++)
            s[2 * depth - 1][offset - width/2 + i] = '-';

        s[2 * depth - 1][offset + izq + width/2] = '+';
        s[2 * depth - 1][offset - width/2 - 1] = '+';
    }
#endif

    return izq + width + der;
}

void print_t(abb_nodo_t *nodo) {
    char s[20][255];
    for (int i = 0; i < _abb_altura(nodo,0) + 2; i++)
        sprintf(s[i], "%80s", " ");

    _print_t(nodo, 0, 0, 0, s);

    for (int i = 0; i < _abb_altura(nodo,0) + 2; i++)
        printf("%s\n", s[i]);
}

void imprimir_abb(abb_t *arbol) {
	print_t(arbol->raiz);
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

abb_nodo_t* abb_nodo_crear(const char* clave, void* dato) {
    abb_nodo_t* abb_nodo = malloc(sizeof(abb_nodo_t));
    if (!abb_nodo) return NULL;

    char* copia_clave = strdup(clave);
    if(!copia_clave) {
        free(abb_nodo);
        return NULL;
    }

    abb_nodo->clave = copia_clave;
    abb_nodo->dato = dato;
    abb_nodo->der = NULL;
    abb_nodo->izq = NULL;

    return abb_nodo;
}

void ubicar_nodo(abb_comparar_clave_t cmp, abb_nodo_t* nodo_ant, abb_nodo_t* nodo_act, abb_nodo_t* nodo_nuevo) {
    if (!nodo_act) {
    	//printf("Ultimo paso\n");
        if (cmp(nodo_nuevo->clave, nodo_ant->clave) > 0) { //si la clave nueva es mayor a la anterior
        	nodo_ant->der = nodo_nuevo;
        } 
        else {
        	nodo_ant->izq = nodo_nuevo;
        } 
        return;
    }
    if (cmp(nodo_act->clave, nodo_nuevo->clave) > 0) {
    	//printf("la clave nueva es menor que la clave actual\n");
    	ubicar_nodo(cmp, nodo_act, nodo_act->izq, nodo_nuevo);//acual > nueva
    } 
    else if (cmp(nodo_act->clave, nodo_nuevo->clave) < 0) {
    	//printf("la clave nueva es mayor que la clave actual\n");
    	ubicar_nodo(cmp, nodo_act, nodo_act->der, nodo_nuevo); //actual < nueva
    } 
    else { //act == nueva
    	//printf("Iguales, reemplazo\n");
        nodo_act->dato = nodo_nuevo->dato;
        return;
    }
}

bool abb_guardar(abb_t *arbol, const char *clave, void *dato) {
    abb_nodo_t* nuevo_abb_nodo = abb_nodo_crear(clave, dato);
    if (!nuevo_abb_nodo) return false;

    if (arbol->raiz) {
    	//printf("No estaba vacio, lo ubico\n");
        ubicar_nodo(arbol->cmp ,NULL, arbol->raiz, nuevo_abb_nodo);
    } else {
        arbol->raiz = nuevo_abb_nodo;
        //printf("Estaba vacio, lo pongo en la raiz\n");
    }
    //printf("Guardé algo\n");
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
