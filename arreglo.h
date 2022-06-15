#ifndef ARREGLO_H
#define ARREGLO_H

#include <stddef.h>
#include <stdbool.h>

typedef struct arreglo{
    void **v;
    size_t n;
} arreglo_t;

arreglo_t *arreglo_crear();
void arreglo_destruir(arreglo_t *a, void (*destruir)(void *dato));
bool arreglo_agregar(arreglo_t *a, void *dato);

#endif
