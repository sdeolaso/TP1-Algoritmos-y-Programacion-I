#include "vector.h"

vector_t vector_resta(vector_t a, vector_t b)
{
    vector_t r;
    r.x = a.x - b.x;
    r.y = a.y - b.y;
    r.z = a.z - b.z;
    return r;
}

float vector_producto_interno(vector_t a, vector_t b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

float vector_norma(vector_t a)
{
    return sqrt(vector_producto_interno(a, a));
}

vector_t vector_interpolar_recta(vector_t o, vector_t d, float t)
{
    vector_t r;
    r.x = o.x + t * d.x;
    r.y = o.y + t * d.y;
    r.z = o.z + t * d.z;
    return r;
}

vector_t vector_normalizar(vector_t a)
{
    vector_t r;
    float norma = vector_norma(a);
    r.x = a.x / norma;
    r.y = a.y / norma;
    r.z = a.z / norma;
    return r;
}

vector_t vector_producto_vectorial(vector_t a, vector_t b)
{
    vector_t r;
    r.x = (a.y * b.z) - (a.z * b.y);
    r.y = (a.z * b.x) - (a.x * b.z);
    r.z = (a.x * b.y) - (a.y * b.x);
    return r;
}
