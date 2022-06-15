#include "esfera.h"

#include <stdlib.h>

#ifndef INFINITO
#define INFINITO __FLT_MAX__
#endif

esfera_t *esfera_crear(vector_t centro, float radio)
{
    esfera_t *esf = malloc(sizeof(esfera_t));
    if(esf == NULL) return NULL;
    esf->centro = centro;
    esf->radio = radio;
    return esf;
}

static void _esfera_destruir(esfera_t *esfera)
{
    free(esfera);
}

void esfera_destruir(void *esfera)
{
    _esfera_destruir((esfera_t *)esfera);
}

static float _esfera_distancia(const esfera_t *esfera, vector_t o, vector_t d, vector_t *punto, vector_t *normal)
{
    vector_t c_primo = vector_resta(esfera->centro, o);
    float beta = vector_producto_interno(c_primo, d);
    float delta = pow(beta, 2) - vector_producto_interno(c_primo, c_primo) + pow(esfera->radio, 2);
    
    if(delta < 0)
        return INFINITO;

    if(beta - sqrt(delta) > 0)
    {
        *punto = vector_interpolar_recta(o, d, beta - sqrt(delta));
        *normal = vector_normalizar(vector_resta(*punto, esfera->centro));
        return beta - sqrt(delta);
    }
    if(beta + sqrt(delta) > 0)
    {
        *punto = vector_interpolar_recta(o, d, beta + sqrt(delta));
        *normal = vector_normalizar(vector_resta(*punto, esfera->centro));
        return beta + sqrt(delta);
    }
    return INFINITO;
}

float esfera_distancia(const void *esfera, vector_t o, vector_t d, vector_t *punto, vector_t *normal)
{
    return _esfera_distancia((esfera_t *)esfera, o, d, punto, normal);
}
