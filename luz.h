#ifndef LUZ_H
#define LUZ_H

#include <stdbool.h>

#include "color.h"
#include "vector.h"

typedef struct {
    vector_t posicion;
    color_t color;
    bool es_puntual;
} luz_t;

luz_t *luz_crear(vector_t posicion, color_t color, bool es_puntual);
void luz_destruir(void *luz);

#endif
