#include "malla.h"

#include <stdlib.h>

#ifndef INFINITO
#define INFINITO __FLT_MAX__
#endif

malla_t *malla_crear()
{
    malla_t *mall = malloc(sizeof(malla_t));
    if(mall == NULL) return NULL;

    mall->triangulos = arreglo_crear();
    if(mall->triangulos == NULL)
    {
        free(mall);
        return NULL;
    }

    mall->triangulos->v = NULL;
    mall->triangulos->n = 0;
    return mall;
}

bool malla_agregar(malla_t *malla, triangulo_t *triangulo)
{
    return arreglo_agregar(malla->triangulos, triangulo);
}

static void _malla_destruir(malla_t *malla)
{
    arreglo_destruir(malla->triangulos, triangulo_destruir);
    free(malla);
}

void malla_destruir(void *malla)
{
    _malla_destruir((malla_t *)malla);
}

static float _malla_distancia(const malla_t *malla, vector_t o, vector_t d, vector_t *punto, vector_t *normal)
{
    float t_menor = INFINITO;
    float t_nuevo;
    vector_t p_nuevo, n_nueva, p_final, n_final;

    for(size_t i = 0; i < malla->triangulos->n; i++)
    {
        t_nuevo = triangulo_distancia(malla->triangulos->v[i], o, d, &p_nuevo, &n_nueva);
        if(t_nuevo < t_menor)
        {
            t_menor = t_nuevo;
            p_final = p_nuevo;
            n_final = n_nueva;
        }
    }
    if(t_menor == INFINITO) return INFINITO;

    *punto = p_final;
    *normal = n_final;
    return t_menor;
}

float malla_distancia(const void *malla, vector_t o, vector_t d, vector_t *punto, vector_t *normal)
{
    return _malla_distancia((malla_t *)malla, o, d, punto, normal);
}
