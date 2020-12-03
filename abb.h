#ifndef ABB_H
#define ABB_H

#include <stdbool.h>
#include <stddef.h>

/* ******************************************************************
 *           DEFINICION DE ESTRUCTURAS Y TIPOS DE DATO
 * *****************************************************************/

typedef struct abb abb_t;
typedef struct abb_iter abb_iter_t;

typedef int (*abb_comparar_clave_t) (const char *, const char *);
typedef void (*abb_destruir_dato_t) (void *);

/* ******************************************************************
 *                       PRIMITIVAS DEL ABB
 * *****************************************************************/

//DECLARACIONES A BORRAR
void imprimir_abb(abb_t *arbol);
size_t abb_altura(const abb_t* abb);

//Crea un abb.
//Post: devuelve un abb.
abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato);

//Guarda un elemento en el arbol, si la clave ya se encuentra en la
//estructura, la reemplaza. De no poder guardarlo devuelve false.
//Pre: la estructura arbol fue creada.
//Post: se almaceno el par (clave, dato).
bool abb_guardar(abb_t *arbol, const char *clave, void *dato);

//Borra un elemento del arbol y devuelve el dato asociado. Devuelve NULL
//si no estaba.
//Pre: la estructura arbol fue inicializada.
//Post: el elementos fue borrado de la estructura y se devolvio, en el 
//caso de que estuviera guardad.
void *abb_borrar(abb_t *arbol, const char *clave);

//Obtiene el valor de un elemento del arbol, si la clave no se enceuntra
//devuelve NULL.
//Pre: la estructura arbol fue inicializada.
void *abb_obtener(const abb_t *arbol, const char *clave);

//Determina si la clave pertenece o no al arbol.
//Pre: la estructura arbol fue inicializada.
bool abb_pertenece(const abb_t *arbol, const char *clave);

//Devuelve la cantidad de elementos del arbol.
//Pre: la estructura arbol fue inicializada.
size_t abb_cantidad(abb_t *arbol);

//Destruye la estructura liberando la memoria pedida y llamando a la funcion
//destruir para cada par (clave, dato).
//Pre: la estructura arbol fue inicializada.
//Post: la estructura arbol fue destruida.
void abb_destruir(abb_t *arbol);

/* *****************************************************************
 *                   PRIMITIVAS DEL ITERADOR EXTERNO
 * *****************************************************************/

//Crea el iterador inorder para el arbol.
//Pre: la estructura arbol fue inicializada. 
abb_iter_t *abb_iter_in_crear(const abb_t *arbol);

//Avanza el iterador. Devuelve false si no es posible avanzar.
//Pre: el iterador fue creado
bool abb_iter_in_avanzar(abb_iter_t *iter);

//Devuelve la clave actual, esa clave no se puede modificar ni liberar. Si 
//el iterador se encuentra al final devuelve NULL.
//Pre: el iterador fue creado.
const char *abb_iter_in_ver_actual(const abb_iter_t *iter);

//Comprueba si terminó la iteración
//Pre: el iterador fue creado.
bool abb_iter_in_al_final(const abb_iter_t *iter);

//Destruye el iterador
//Pre: el iterador fue creado.
void abb_iter_in_destruir(abb_iter_t* iter);

/* *****************************************************************
 *                      DECLARACION ITERADOR INTERNO
 * *****************************************************************/

//visita cada elemento y le aplica la funcion visitar, si devuelve true
//continua iterando o completar la iteracion del arbol.
void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra);

/* *****************************************************************
 *                      PRUEBAS UNITARIAS
 * *****************************************************************/

void pruebas_abb_estudiante(void);

#endif