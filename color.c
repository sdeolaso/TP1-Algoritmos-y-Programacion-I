#include "color.h"

color_t color_sumar(color_t c, color_t m, float p)
{
    color_t r;
    r.r = c.r + p * m.r;
    r.g = c.g + p * m.g;
    r.b = c.b + p * m.b;
    return r;
}

color_t color_absorber(color_t b, color_t c)
{
    color_t r;
    r.r = b.r * c.r;
    r.g = b.g * c.g;
    r.b = b.b * c.b;
    return r;
}
