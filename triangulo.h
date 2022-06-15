#ifndef TRIANGULO_H
#define TRIANGULO_H

#include "vector.h"

typedef struct triangulo {
    vector_t normal;
    vector_t p0, p1, p2, e1, e2;
} triangulo_t;

triangulo_t *triangulo_crear(vector_t *normal, vector_t p1, vector_t p2, vector_t p3);
void triangulo_destruir(void *triangulo);
float triangulo_distancia(const void *triangulo, vector_t o, vector_t d, vector_t *punto, vector_t *normal);

#endif
