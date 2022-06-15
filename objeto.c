#include "objeto.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

distanciadores_t distanciadores[] = {esfera_distancia, malla_distancia, plano_distancia, triangulo_distancia};
destructores_t destructores[] = {esfera_destruir, malla_destruir, plano_destruir, triangulo_destruir};

static objeto_t *objeto_crear(float ka, float kd, float ks, float kr, color_t color, objeto_id_t id, void *geometria)
{
    if(geometria == NULL) return NULL;

    objeto_t *obj = malloc(sizeof(objeto_t));
    if(obj == NULL) return NULL;

    obj->ka = ka;
    obj->kd = kd;
    obj->ks = ks;
    obj->kr = kr;
    obj->color = color;
    obj->id = id;
    obj->geometria = geometria;

    return obj;
}

static malla_t *leer_STL(char *STL)
{
    FILE *f = fopen(STL, "rb");
    if(f == NULL) return NULL;

    malla_t *r = malla_crear();
    if(r == NULL)
    {
        fclose(f);
        return NULL;
    }

    char descartar[80];
    fread(descartar, 80, 1, f);

    uint32_t n;
    fread(&n, sizeof(uint32_t), 1, f);
    for(size_t i = 0; i < n; i++)
    {
        float v[12];
        fread(v, sizeof(float) * 12, 1, f);
        fread(descartar, 2, 1, f);

        triangulo_t *nuevo = triangulo_crear(&((vector_t){v[0], v[1], v[2]}), 
                                            (vector_t){v[3], v[4], v[5]},
                                            (vector_t){v[6], v[7], v[8]},
                                            (vector_t){v[9], v[10], v[11]});
        if(nuevo == NULL)
        {
            malla_destruir(r);
            fclose(f);
            return NULL;
        }

        if(!malla_agregar(r, nuevo))
        {
            malla_destruir(r);
            fclose(f);
            return NULL;  
        }
    }
    fclose(f);
    return r;
}

objeto_t *objeto_crear_esfera(vector_t centro, float radio, color_t color, float ka, float kd, float ks, float kr)
{
    return objeto_crear(ka, kd, ks, kr, color, ESFERA, esfera_crear(centro, radio));
}

objeto_t *objeto_crear_malla(char *STL, color_t color, float ka, float kd, float ks, float kr)
{
    return objeto_crear(ka, kd, ks, kr, color, MALLA, leer_STL(STL));
}

objeto_t *objeto_crear_plano(vector_t normal, vector_t punto, color_t color, float ka, float kd, float ks, float kr)
{
    return objeto_crear(ka, kd, ks, kr, color, PLANO, plano_crear(normal, punto));
}

objeto_t *objeto_crear_triangulo(vector_t p1, vector_t p2, vector_t p3, color_t color, float ka, float kd, float ks, float kr)
{
    return objeto_crear(ka, kd, ks, kr, color, TRIANGULO, triangulo_crear(NULL, p1, p2, p3));
}

float objeto_distancia(const objeto_t *objeto, vector_t o, vector_t d, vector_t *punto, vector_t *normal)
{
    return distanciadores[objeto->id](objeto->geometria, o, d, punto, normal);
}

static void _objeto_destruir(objeto_t *objeto)
{
    destructores[objeto->id](objeto->geometria);
    free(objeto);
}

void objeto_destruir(void *objeto)
{
    _objeto_destruir((objeto_t *)objeto);
}
