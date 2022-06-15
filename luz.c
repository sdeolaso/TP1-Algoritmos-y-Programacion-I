#include "luz.h"

#include <stdlib.h>

luz_t *luz_crear(vector_t posicion, color_t color, bool es_puntual)
{
    luz_t *l = malloc(sizeof(luz_t));
    if(l == NULL) return NULL;
    l->color = color;
    l->posicion = posicion;
    l->es_puntual = es_puntual;
    return l;
}

static void _luz_destruir(luz_t *luz)
{
    free(luz);
}

void luz_destruir(void *luz)
{
    _luz_destruir((luz_t *)luz);
}
