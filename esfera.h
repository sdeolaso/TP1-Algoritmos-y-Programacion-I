#ifndef ESFERA_H
#define ESFERA_H

#include "vector.h"

typedef struct esfera {
    vector_t centro;
    float radio;
} esfera_t;

esfera_t *esfera_crear(vector_t centro, float radio);
void esfera_destruir(void *esfera);
float esfera_distancia(const void *esfera, vector_t o, vector_t d, vector_t *punto, vector_t *normal);

#endif
