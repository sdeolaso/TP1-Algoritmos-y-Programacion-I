#include "plano.h"

#include <stdlib.h>

#ifndef INFINITO
#define INFINITO __FLT_MAX__
#endif
#ifndef EPSILON
#define EPSILON 0.00001
#endif

plano_t *plano_crear(vector_t normal, vector_t punto)
{
    plano_t *pla = malloc(sizeof(plano_t));
    if(pla == NULL) return NULL;
    pla->normal = vector_normalizar(normal);
    pla->punto = punto;
    return pla;
}

static void _plano_destruir(plano_t *plano)
{
    free(plano);
}

void plano_destruir(void *plano)
{
    _plano_destruir((plano_t *)plano);
}

static float _plano_distancia(const plano_t *plano, vector_t o, vector_t d, vector_t *punto, vector_t *normal)
{
    float prod_int = vector_producto_interno(d, plano->normal);
    if(fabsf(prod_int) < EPSILON) return INFINITO;

    float t = vector_producto_interno(vector_resta(plano->punto, o), plano->normal) / prod_int;

    if(t >= 0) 
    {
        *punto = vector_interpolar_recta(o, d, t);
        *normal = plano->normal;
        return t;
    }
    
    return INFINITO;
}

float plano_distancia(const void *plano, vector_t o, vector_t d, vector_t *punto, vector_t *normal)
{
    return _plano_distancia((plano_t *)plano, o, d, punto, normal);
}
