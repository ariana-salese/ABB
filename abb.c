#define _POSIX_C_SOURCE 200809L
#include "abb.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
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

typedef enum accion {
    NO_BORRAR, BORRAR
} accion_t;

typedef enum cantidad_hijos {
    CERO_HIJOS, UN_HIJO, DOS_HIJOS
} cantidad_hijos_t;


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

size_t contar_hijos(abb_nodo_t* nodo) {
    if (!nodo->der && !nodo->izq) return CERO_HIJOS;
    if (nodo->der && nodo->izq) return DOS_HIJOS;
    return UN_HIJO;
}

abb_nodo_t* buscar_hijo_unico(abb_nodo_t* nodo) {
    if (nodo->der) return nodo->der;
    if (nodo->izq) return nodo->izq;
    return NULL;
}

abb_nodo_t* buscar_reemplazo(abb_nodo_t* nodo) {
    abb_nodo_t* nodo_act = nodo->der;

    while (nodo_act->izq) nodo_act = nodo_act->izq;
    
    return nodo_act;
}

void _abb_destruir(abb_nodo_t* nodo, abb_destruir_dato_t destruir_dato, abb_t *arbol) { // Tiene que hacer recorrido Post-Order si no me equivoco
	
	if (!nodo) return;

	_abb_destruir(nodo->izq, destruir_dato, arbol);
	_abb_destruir(nodo->der, destruir_dato, arbol);

	if (destruir_dato) destruir_dato(nodo->dato);
	free(nodo->clave);
	free(nodo);
}

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

abb_nodo_t* buscar_nodo(abb_comparar_clave_t cmp, abb_nodo_t* nodo_act, abb_nodo_t* nodo_ant, const char* clave, accion_t borrar) {
    if (!nodo_act) return NULL;
    //printf("busco resultado cmp\n");
    int resultado_cmp_act = cmp(nodo_act->clave, clave);
    //printf("tengo resultado\n");
    if (resultado_cmp_act > 0) return buscar_nodo (cmp, nodo_act->izq, nodo_act, clave, borrar);
    else if (resultado_cmp_act < 0) return buscar_nodo (cmp, nodo_act->der, nodo_act, clave, borrar);
    //printf("encontre la clave a eliminar\n");
    if (!borrar) return nodo_act;

    int resultado_cmp_ant = 0;
    //printf("busco resultado cmp de ant\n");
    if (nodo_ant) resultado_cmp_ant = cmp(clave, nodo_ant->clave);
    //printf("busco cantidad de hijos\n");
    size_t cant_hijos = contar_hijos(nodo_act);
    //printf("tengo cantidad de hijos\n");
    abb_nodo_t* reemplazo = NULL;

    if (cant_hijos == CERO_HIJOS || cant_hijos == UN_HIJO) {
        //printf("tiene uno o cero hijos\n");
        reemplazo = buscar_hijo_unico(nodo_act); //si se te ocurre un nombre mas representativo mejor, no se me ocurre uno que no sea eterno
        if (resultado_cmp_ant > 0) nodo_ant->der = reemplazo;
        else nodo_ant->izq = reemplazo; 

        return nodo_act;
    }
    //printf("voy a reemplazar\n");
    reemplazo = buscar_reemplazo(nodo_act);
    abb_nodo_t* nodo_a_devolver = abb_nodo_crear(nodo_act->clave, nodo_act->dato);
    if (!nodo_a_devolver) return NULL;
    //if (!reemplazo) return nodo_a_devolver;

    buscar_nodo(cmp, nodo_act, nodo_ant, reemplazo->clave, BORRAR);
    
    free(nodo_act->clave);
    //if (arbol->destruir_dato) arbol->destruir_dato(nodo_act->dato);
    nodo_act->clave = reemplazo->clave;
    nodo_act->dato = reemplazo->dato;
    free(reemplazo);
	//printf("termine\n");
    return nodo_a_devolver;
}

void ubicar_nodo(abb_t* arbol, abb_nodo_t* nodo_ant, abb_nodo_t* nodo_act, abb_nodo_t* nodo_nuevo) {
   //printf("holi\n");
    if (!nodo_act) {
        //printf("entre\n");
        if (arbol->cmp(nodo_nuevo->clave, nodo_ant->clave) > 0) nodo_ant->der = nodo_nuevo;      
        else nodo_ant->izq = nodo_nuevo;
        return;
    }
    int resultado_cmp = arbol->cmp(nodo_nuevo->clave, nodo_act->clave);
    //printf("La clave actual es: %s\n", nodo_act->clave);
    //printf("cmp devuelve: %i\n", resultado_cmp);
    if (resultado_cmp > 0) {
    	//printf("la clave nueva es mayor que la clave actual\n");
    	ubicar_nodo(arbol, nodo_act, nodo_act->der, nodo_nuevo); //acutal > nueva
    } 
    else if (resultado_cmp < 0) {
    	//printf("la clave nueva es menor que la clave actual\n");
    	ubicar_nodo(arbol, nodo_act, nodo_act->izq, nodo_nuevo); //actual < nueva
    } 
    else { //act == nueva
    	//printf("Iguales, reemplazo\n");
        if (arbol->destruir_dato) arbol->destruir_dato(nodo_act->dato);
        nodo_act->dato = nodo_nuevo->dato;
        free(nodo_nuevo->clave);
        free(nodo_nuevo);
        arbol->cantidad--;
        return;
    }
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
    	//printf("No estaba vacio, lo ubico\n");
        ubicar_nodo(arbol, NULL, arbol->raiz, nuevo_abb_nodo);
    } else {
        arbol->raiz = nuevo_abb_nodo;
        //printf("Estaba vacio, lo pongo en la raiz\n");
    }
    //printf("Guardé algo\n");
    arbol->cantidad++;

    return true;
}

void *abb_borrar(abb_t *arbol, const char *clave) {
    abb_nodo_t* nodo_a_eliminar;

    if (arbol->cantidad == 1) nodo_a_eliminar = arbol->raiz;
    else nodo_a_eliminar = buscar_nodo(arbol->cmp, arbol->raiz, NULL, clave, BORRAR);
    
    if (!nodo_a_eliminar) return NULL;
    
    void* dato_a_devolver = nodo_a_eliminar->dato;
    free(nodo_a_eliminar->clave);
    free(nodo_a_eliminar);

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
