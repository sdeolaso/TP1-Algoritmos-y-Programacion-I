#include "arreglo.h"

#include <stdlib.h>

arreglo_t *arreglo_crear()
{
    arreglo_t *r = malloc(sizeof(arreglo_t));
    if(r == NULL) return NULL;
    r->v = NULL;
    r->n = 0;
    return r;
}

void arreglo_destruir(arreglo_t *a, void (*destruir_dato)(void *))
{
    if(destruir_dato != NULL)
    {
        for(size_t i = 0; i < a->n; i++)
            destruir_dato(a->v[i]);
    }
    free(a->v);
    free(a);
}

bool arreglo_agregar(arreglo_t *a, void *dato)
{
    void **aux = realloc(a->v, sizeof(void *) * (a->n + 1));
    if(aux == NULL) return false;
    a->v = aux;
    a->v[a->n] = dato;
    (a->n)++;
    return true;
}
