#ifndef COLOR_H
#define COLOR_H

typedef struct color {
    float r, g, b;
} color_t;

color_t color_sumar(color_t c, color_t m, float p);
color_t color_absorber(color_t b, color_t c);

#endif
