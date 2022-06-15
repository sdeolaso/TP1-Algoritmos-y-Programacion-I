#include "triangulo.h"

#include <stdlib.h>

#ifndef INFINITO
#define INFINITO __FLT_MAX__
#endif
#ifndef EPSILON
#define EPSILON 0.00001
#endif

triangulo_t *triangulo_crear(vector_t *normal, vector_t p0, vector_t p1, vector_t p2)
{
    triangulo_t *triang = malloc(sizeof(triangulo_t));
    if(triang == NULL) return NULL;

    triang->p0 = p0;
    triang->p1 = p1;
    triang->p2 = p2;
    triang->e1 = vector_resta(p1, p0);
    triang->e2 = vector_resta(p2, p0);

    if(normal == NULL) triang->normal = vector_normalizar(vector_producto_vectorial(triang->e1, triang->e2));
    else triang->normal = vector_normalizar(*normal);

    return triang;
}

static void _triangulo_destruir(triangulo_t *triangulo)
{
    free(triangulo);
}

void triangulo_destruir(void *triangulo)
{
    _triangulo_destruir((triangulo_t *)triangulo);
}

static float _triangulo_distancia(const triangulo_t *triangulo, vector_t o, vector_t d, vector_t *punto, vector_t *normal)
{
    vector_t h = vector_producto_vectorial(d, triangulo->e2);
    float a = vector_producto_interno(triangulo->e1, h);
    if(fabsf(a) < EPSILON) return INFINITO;

    vector_t s = vector_resta(o, triangulo->p0);
    float u = vector_producto_interno(s, h) / a;
    if(u < 0 || u > 1) return INFINITO;

    vector_t q = vector_producto_vectorial(s, triangulo->e1);
    float v = vector_producto_interno(d, q) / a;
    if(v < 0 || (u + v) > 1) return INFINITO;

    float t = vector_producto_interno(triangulo->e2, q) / a;
    if(t < 0) return INFINITO;

    *punto = vector_interpolar_recta(o, d, t);
    *normal = triangulo->normal;
    return t;
}

float triangulo_distancia(const void *triangulo, vector_t o, vector_t d, vector_t *punto, vector_t *normal)
{
    return _triangulo_distancia((triangulo_t *)triangulo, o, d, punto, normal);
}
