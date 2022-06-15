#ifndef MALLA_H
#define MALLA_H

#include "vector.h"

#include "arreglo.h"
#include "triangulo.h"

typedef struct malla {
    arreglo_t *triangulos;
} malla_t;

malla_t *malla_crear();
bool malla_agregar(malla_t *malla, triangulo_t *triangulo);
void malla_destruir(void *malla);
float malla_distancia(const void *malla, vector_t o, vector_t d, vector_t *punto, vector_t *normal);

#endif
