#ifndef OBJETO_H
#define OBJETO_H

#include "color.h"

#include "esfera.h"
#include "plano.h"
#include "triangulo.h"
#include "malla.h"

typedef enum objeto_id {
    ESFERA, MALLA, PLANO, TRIANGULO
} objeto_id_t;

typedef float (*distanciadores_t)(const void *, vector_t, vector_t, vector_t *, vector_t *);
typedef void (*destructores_t)(void *);

typedef struct objeto {
    float ka, kd, ks, kr;
    color_t color;
    objeto_id_t id;
    void *geometria;
} objeto_t;

objeto_t *objeto_crear_esfera(vector_t centro, float radio, color_t color, float ka, float kd, float ks, float kr);
objeto_t *objeto_crear_malla(char *STL, color_t color, float ka, float kd, float ks, float kr);
objeto_t *objeto_crear_plano(vector_t normal, vector_t punto, color_t color, float ka, float kd, float ks, float kr);
objeto_t *objeto_crear_triangulo(vector_t p1, vector_t p2, vector_t p3, color_t color, float ka, float kd, float ks, float kr);
float objeto_distancia(const objeto_t *objeto, vector_t o, vector_t d, vector_t *punto, vector_t *normal);
void objeto_destruir(void *objeto);

#endif
