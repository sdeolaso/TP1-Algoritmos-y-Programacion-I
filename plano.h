#ifndef PLANO_H
#define PLANO_H

#include "vector.h"

typedef struct plano {
    vector_t punto;
    vector_t normal;
} plano_t;

plano_t *plano_crear(vector_t normal, vector_t punto);
void plano_destruir(void *plano);
float plano_distancia(const void *plano, vector_t o, vector_t d, vector_t *punto, vector_t *normal);

#endif
