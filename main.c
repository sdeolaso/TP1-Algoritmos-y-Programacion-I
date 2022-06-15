#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>
#include "imagen.h"
#include "arreglo.h"
#include "objeto.h"
#include "luz.h"
#define INFINITO __FLT_MAX__
#define FOV 90
#define PI 3.14159265358979323846
#define ALFA 10
#define EPSILON 0.00001
#define MASK_BYTE 0xFF
#define SHIFT_1BYTE 8
#define SHIFT_2BYTE 16
#define SHIFT_3BYTE 24

color_t computar_intensidad(int profundidad, const arreglo_t *objetos, const arreglo_t *luces, color_t ambiente, vector_t o, vector_t d)
{
    // Empezamos con negro
    color_t color_final = {0, 0, 0};

    // Objetos
    int i_chocado;
    float t_nuevo, t_menor = INFINITO;
    vector_t p_nuevo, p_final, n_nueva, n_final;
    for(size_t i = 0; i < objetos->n; i++)
    {
        t_nuevo = objeto_distancia(objetos->v[i], o, d, &p_nuevo, &n_nueva);

        if (t_nuevo < t_menor) 
        {
            t_menor = t_nuevo; 
            i_chocado = i;
            p_final = p_nuevo;
            n_final = n_nueva;
        }
    }
    if(t_menor == INFINITO) return color_final;

    // Calculo de R
    vector_t n_por_2 = {n_final.x * 2, n_final.y * 2, n_final.z * 2};
    float prod_int_r = vector_producto_interno(d, n_final);
    vector_t aux_r = {n_por_2.x * prod_int_r, n_por_2.y * prod_int_r, n_por_2.z * prod_int_r};

    vector_t r = vector_normalizar(vector_resta(d, aux_r));

    // Luces
    vector_t d_luz;
    objeto_t *objeto_chocado = objetos->v[i_chocado];
    for(size_t l = 0; l < luces->n; l++)
    {
        luz_t *luz_actual = luces->v[l];
        if(luz_actual->es_puntual) d_luz = vector_normalizar(vector_resta(luz_actual->posicion, p_final));
        else d_luz = vector_normalizar(luz_actual->posicion);

        // Iteracion por las luces
        int delta_s = 1;
        vector_t p_prima_n = vector_interpolar_recta(p_final, n_final, EPSILON);
        for (size_t i = 0; i < objetos->n; i++)
        {
            t_nuevo = objeto_distancia(objetos->v[i], p_prima_n, d_luz, &p_nuevo, &n_nueva);
            if(luz_actual->es_puntual)
            {
                if(t_nuevo <= vector_norma(vector_resta(luz_actual->posicion, p_prima_n)))
                {
                    delta_s = 0;
                    break;
                }
            }
            else
            {
                if(t_nuevo < INFINITO)
                {
                    delta_s = 0;
                    break;
                }
            }
        }
        if(delta_s == 0) continue;

        // Calculo de componentes difuso y especular
        float componente_difuso = 0;
        float prod_int_dif = vector_producto_interno(d_luz, n_final);
        if(prod_int_dif > 0) componente_difuso = prod_int_dif * objeto_chocado->kd;

        float componente_especular = 0;
        float prod_int_esp = vector_producto_interno(d_luz, r);
        if(prod_int_esp > 0) componente_especular = objeto_chocado->ks * pow(prod_int_esp, ALFA);

        if(componente_difuso == 0 && componente_especular == 0) continue;

        // Suma al color final
        color_t color_aux = {componente_difuso + componente_especular, 
                             componente_difuso + componente_especular, 
                             componente_difuso + componente_especular};
        color_t intensidad_i = color_absorber(luz_actual->color, objeto_chocado->color);
        color_final = color_sumar(color_final, color_absorber(color_aux, intensidad_i), delta_s);
    }

    // Calculo de componente ambiental
    color_t intensidad_a = color_absorber(ambiente, objeto_chocado->color);
    color_final = color_sumar(color_final, intensidad_a, objeto_chocado->ka);

    // Recursividad (o retorno)
    if(profundidad == 0 || objeto_chocado->kr <= 0) return color_final;

    vector_t p_prima_r = vector_interpolar_recta(p_final, r, EPSILON);
    
    return color_sumar(color_final, computar_intensidad(profundidad - 1, objetos, luces, ambiente, p_prima_r, r), objeto_chocado->kr);
}

void crear_escena(arreglo_t *o, arreglo_t *l)
{
    assert(arreglo_agregar(l, luz_crear((vector_t){0, 1, 0}, (color_t){.2, .2, .2}, false)));
    assert(arreglo_agregar(l, luz_crear((vector_t){-2, 10, 2.5}, (color_t){.9, 0, 0}, true)));
    assert(arreglo_agregar(l, luz_crear((vector_t){2, 10, 2.5}, (color_t){0, .9, 0}, true)));
    assert(arreglo_agregar(l, luz_crear((vector_t){0, 10, 5}, (color_t){0, 0, .9}, true)));

    assert(arreglo_agregar(o, objeto_crear_esfera((vector_t){0, 1, 2.4}, .3, (color_t){1, 1, 1}, 1, 1, .3, .3)));
    assert(arreglo_agregar(o, objeto_crear_esfera((vector_t){0, -.4, 3}, 1, (color_t){1, 1, 1}, 1, 1, .3, .3)));

    assert(arreglo_agregar(o, objeto_crear_esfera((vector_t){-2, -.6, 3}, .3, (color_t){1, 0, 0}, 1, .8, .3, .3)));
    assert(arreglo_agregar(o, objeto_crear_esfera((vector_t){-1.73, -.6, 2}, .3, (color_t){1, 1, 0}, 1, 1, .3, .3)));
    assert(arreglo_agregar(o, objeto_crear_esfera((vector_t){-1, -.6, 1.26}, .3, (color_t){0, 1, 0}, 1, 1, .3, .3)));
    assert(arreglo_agregar(o, objeto_crear_esfera((vector_t){0, -.6, 1}, .3, (color_t){1, 1, 1}, 1, 1, .3, .3)));
    assert(arreglo_agregar(o, objeto_crear_esfera((vector_t){1, -.6, 1.26}, .3, (color_t){0, 1, 1}, 1, 1, .3, .3)));
    assert(arreglo_agregar(o, objeto_crear_esfera((vector_t){1.73, -.6, 2}, .3, (color_t){0, 0, 1}, 1, 1, .3, .3)));
    assert(arreglo_agregar(o, objeto_crear_esfera((vector_t){2, -.6, 3}, .3, (color_t){1, 0, 1}, 1, 1, .3, .3)));

    assert(arreglo_agregar(o, objeto_crear_esfera((vector_t){-3, 2.5, 4.3}, .3, (color_t){1, 1, 1}, 1, 0, .3, .3)));
    assert(arreglo_agregar(o, objeto_crear_esfera((vector_t){-2, 2.5, 4.3}, .3, (color_t){1, 1, 1}, 1, .16, .3, .3)));
    assert(arreglo_agregar(o, objeto_crear_esfera((vector_t){-1, 2.5, 4.3}, .3, (color_t){1, 1, 1}, 1, .33, .3, .3)));
    assert(arreglo_agregar(o, objeto_crear_esfera((vector_t){0, 2.5, 4.3}, .3, (color_t){1, 1, 1}, 1, .5, .3, .3)));
    assert(arreglo_agregar(o, objeto_crear_esfera((vector_t){1, 2.5, 4.3}, .3, (color_t){1, 1, 1}, 1, .66, .3, .3)));
    assert(arreglo_agregar(o, objeto_crear_esfera((vector_t){2, 2.5, 4.3}, .3, (color_t){1, 1, 1}, 1, .83, .3, .3)));
    assert(arreglo_agregar(o, objeto_crear_esfera((vector_t){3, 2.5, 4.3}, .3, (color_t){1, 1, 1}, 1, 1, .3, .3)));

    assert(arreglo_agregar(o, objeto_crear_esfera((vector_t){-3, 1.5, 4}, .3, (color_t){1, 1, 1}, 0, 1, .3, .3)));
    assert(arreglo_agregar(o, objeto_crear_esfera((vector_t){-2, 1.5, 4}, .3, (color_t){1, 1, 1}, .16, 1, .3, .3)));
    assert(arreglo_agregar(o, objeto_crear_esfera((vector_t){-1, 1.5, 4}, .3, (color_t){1, 1, 1}, .33, 1, .3, .3)));
    assert(arreglo_agregar(o, objeto_crear_esfera((vector_t){0, 1.5, 4}, .3, (color_t){1, 1, 1}, .5, 1, .3, .3)));
    assert(arreglo_agregar(o, objeto_crear_esfera((vector_t){1, 1.5, 4}, .3, (color_t){1, 1, 1}, .66, 1, .3, .3)));
    assert(arreglo_agregar(o, objeto_crear_esfera((vector_t){2, 1.5, 4}, .3, (color_t){1, 1, 1}, .83, 1, .3, .3)));
    assert(arreglo_agregar(o, objeto_crear_esfera((vector_t){3, 1.5, 4}, .3, (color_t){1, 1, 1}, 1, 1, .3, .3)));

    /*
    // Luces
    arreglo_agregar(l, luz_crear((vector_t){0, 1, 0}, (color_t){.9, .5, .5}, false));
    arreglo_agregar(l, luz_crear((vector_t){-3, 3.5, 7.5}, (color_t){.7, .7, .7}, true));
    arreglo_agregar(l, luz_crear((vector_t){1, -3, 7.5}, (color_t){.7, .7, .7}, true));
    
    // Planos
    arreglo_agregar(o, objeto_crear_plano((vector_t){1, 0, 0}, (vector_t){-6, 0, 0}, (color_t){0.11, .07, .03}, .5, 1, .3, .1)); //izquierda
	arreglo_agregar(o, objeto_crear_plano((vector_t){-1, 0, 0}, (vector_t){6, 0, 0}, (color_t){0.11, .07, .03}, .5, 1, .3, .1)); //derecha
	arreglo_agregar(o, objeto_crear_plano((vector_t){0, 0, -1}, (vector_t){0, 0, 16}, (color_t){.17, .19, 0.1}, .3, 1, .3, .6)); //trasera
	arreglo_agregar(o, objeto_crear_plano((vector_t){0, 1, 0}, (vector_t){0, -3.5, 0}, (color_t){.32, .82, .96}, .3, .3, .5, .6)); //subpiso

	// Plataforma altura
	arreglo_agregar(o, objeto_crear_triangulo((vector_t){-6, -3 ,7}, (vector_t){6, -3, 7}, (vector_t){-6, -2, 7}, (color_t){.17, .19, 0.1}, .9, 1, 0, 0));
	arreglo_agregar(o, objeto_crear_triangulo((vector_t){6, -2, 7}, (vector_t){-6, -2, 7}, (vector_t){6, -3, 7}, (color_t){.17, .19, 0.1}, .9, 1, 0, 0));
    
	// Plataforma base
	arreglo_agregar(o, objeto_crear_triangulo((vector_t){-7, -2, 7}, (vector_t){8, -2, 7}, (vector_t){-7, -2, 17}, (color_t){.88, .56, .25}, .9, 1, .8, .1));
	arreglo_agregar(o, objeto_crear_triangulo((vector_t){8, -2, 17}, (vector_t){-7, -2, 17},(vector_t){8, -2, 7}, (color_t){.88, .56, .25}, .9, 1, .8, .1));

	// Rampa
	arreglo_agregar(o, objeto_crear_triangulo((vector_t){-6, 3, 10}, (vector_t){-6, 3, 16}, (vector_t){4, -2, 10}, (color_t){.88, .56, .25}, .3, 1, .3, .3));
	arreglo_agregar(o, objeto_crear_triangulo((vector_t){4, -2, 16}, (vector_t){4, -2, 10},(vector_t){-6, 3, 16}, (color_t){.88, .56, .25}, .3, 1, .3, .3));

	// Triangulo rampa
	arreglo_agregar(o, objeto_crear_triangulo((vector_t){-6, -2, 10}, (vector_t){4, -2, 10},(vector_t){-6, 3, 10}, (color_t){.17, .19, 0.1}, .3, 1, .8, .1));

    // Piedra
    arreglo_agregar(o, objeto_crear_esfera((vector_t){-1, 2.5, 13}, 2, (color_t){0.16, 0.16, 0.12}, .66, 1, .3, .3));
    
    // STL (Crash)
    arreglo_agregar(o, objeto_crear_malla("Pantalones.stl", (color_t){.13, .07, .42}, .66, 1, .3, .3));
    arreglo_agregar(o, objeto_crear_malla("Cresta.stl", (color_t){.96, .2, .02}, .66, 1, .3, .3));
    arreglo_agregar(o, objeto_crear_malla("Pecho.stl", (color_t){.98, .33, .04}, .66, 1, .3, .3));
    arreglo_agregar(o, objeto_crear_malla("Panza.stl", (color_t){.96, .43, .01}, .66, 1, .3, .3));
    arreglo_agregar(o, objeto_crear_malla("Ojos.stl", (color_t){1, 1, 1}, .66, 1, .3, .3));
    arreglo_agregar(o, objeto_crear_malla("Boca.stl", (color_t){.91, .53, .6}, .66, 1, .3, .3));
    arreglo_agregar(o, objeto_crear_malla("Negro.stl", (color_t){0, 0, 0}, .66, 1, .3, .3));
    */
}

void computar_imagen(imagen_t *imagen, int profundidad, const arreglo_t *objetos, const arreglo_t *luces)
{
    color_t ambiente = {.05, .05, .05};
    vector_t origen = {0, 0, 0};

    size_t ancho, alto;
    imagen_dimensiones(imagen, &ancho, &alto);

    float vz = ancho / 2 / tan(FOV/ 2 * PI / 180);
    int porcentaje_anterior = -5;
    putchar('\n');
    for(int vy = 0; vy < alto; vy++)
    {
        for(int vx = 0; vx < ancho; vx++) 
        {   
            int vx_prima = vx - (ancho/2);
            int vy_prima = vy - (alto/2);
            color_t aux = computar_intensidad(profundidad, objetos, luces, ambiente, origen, vector_normalizar((vector_t){vx_prima, vy_prima, vz}));
            imagen_set_pixel(imagen, vx, vy, aux);
        }

        int porcentaje = 100 * vy/ancho;
        if(porcentaje >= porcentaje_anterior + 5)
        {
            printf("%d%%\n", porcentaje);
            porcentaje_anterior = porcentaje;
        }
    }
}

void color_escribir_PPM(FILE *f, color_t c)
{
    int rojo = (c.r >= 1 ? 255 : c.r * 255);
    int verde = (c.g >= 1 ? 255 : c.g * 255);
    int azul = (c.b >= 1 ? 255 : c.b * 255);
    fprintf(f, "%d %d %d", rojo, verde, azul);
}

void escribir_PPM(const imagen_t *imagen, FILE *f)
{
    size_t ancho, alto;
    imagen_dimensiones(imagen, &ancho, &alto);

    fprintf(f, "P3\n%zd %zd\n255\n", ancho, alto);

    for(int vy = alto - 1; vy > -1; vy--)
    {       
        for(int vx = 0; vx < ancho; vx++) 
        {
            color_escribir_PPM(f, imagen_get_pixel(imagen, vx, vy));
            fprintf(f, "\n");
        }
    }
}

void escribir_int16_little_endian(FILE *f, int16_t v)
{
    int8_t u[2] = {v & MASK_BYTE, 
                  (v >> SHIFT_1BYTE) & MASK_BYTE};
    fwrite(u, sizeof(int8_t), 2, f);
}

void escribir_int32_little_endian(FILE *f, int32_t v)
{
    int8_t u[4] = {v & MASK_BYTE, 
                  (v >> SHIFT_1BYTE) & MASK_BYTE, 
                  (v >> SHIFT_2BYTE) & MASK_BYTE, 
                  (v >> SHIFT_3BYTE) & MASK_BYTE};
    fwrite(u, sizeof(int8_t), 4, f);
}

void color_escribir_BMP(FILE *f, color_t c)
{
    uint8_t u[3] = {(c.b >= 1 ? 255 : c.b * 255),
                    (c.g >= 1 ? 255 : c.g * 255),
                    (c.r >= 1 ? 255 : c.r * 255)};
    fwrite(u, sizeof(uint8_t), 3, f);
}

void escribir_BMP(const imagen_t *imagen, FILE *f)
{
    size_t ancho, alto;
    imagen_dimensiones(imagen, &ancho, &alto);

    int resto = (ancho * 3) % 4;
    int scanline;
    if(resto == 0) scanline = ancho * 3;    
    else scanline = (ancho * 3) + (4 - resto);

    fwrite("BM", sizeof(char), 2, f);
    escribir_int32_little_endian(f, 14 + 40 + (alto * scanline));
    escribir_int16_little_endian(f, 0);
    escribir_int16_little_endian(f, 0);
    escribir_int32_little_endian(f, 54);

    escribir_int32_little_endian(f, 40);
    escribir_int32_little_endian(f, ancho);
    escribir_int32_little_endian(f, alto);
    escribir_int16_little_endian(f, 1);
    escribir_int16_little_endian(f, 24);
    escribir_int32_little_endian(f, 0);
    escribir_int32_little_endian(f, 0);
    escribir_int32_little_endian(f, 0);
    escribir_int32_little_endian(f, 0);
    escribir_int32_little_endian(f, 0);
    escribir_int32_little_endian(f, 0);

    for(int vy = 0; vy < alto; vy++)
    {       
        for(int vx = 0; vx < ancho; vx++) color_escribir_BMP(f, imagen_get_pixel(imagen, vx, vy));
        if(resto != 0) fwrite(&((int){0}), sizeof(int8_t), 4 - resto, f);
    }
}

bool tiene_extension(const char *archivo, const char extension[])
{
    size_t i;
    for(i = 0; archivo[i] != '.' && archivo[i] != '\0'; i++);

    if(archivo[i] == '\0') return false;

    if(strncmp(archivo + i, extension, strlen(extension))) return false;

    return true;
}

int main(int argc, char *argv[])
{
    if(argc != 5)
    {
        fprintf(stderr, "Uso: %s [ancho] [alto] [profundidad] [nombrearchivo]\n", argv[0]);
        return 1;
    }

    size_t ancho = atoi(argv[1]);
    size_t alto = atoi(argv[2]);
    imagen_t *imagen = imagen_crear(ancho, alto);
    if(imagen == NULL)
    {
        fprintf(stderr, "Ocurrio un error de memoria\n");
        return 1;
    }
    arreglo_t *luces = arreglo_crear();
    if(luces == NULL)
    {
        imagen_destruir(imagen);
        fprintf(stderr, "Ocurrio un error de memoria\n");
        return 1;
    }
    arreglo_t *objetos = arreglo_crear();
    if(objetos == NULL)
    {
        imagen_destruir(imagen);
        arreglo_destruir(luces, luz_destruir);
        fprintf(stderr, "Ocurrio un error de memoria\n");
        return 1;
    }

    if(tiene_extension(argv[4], ".ppm"))
    {
        FILE *fo = fopen(argv[4], "wt");
        if(fo == NULL)
        {
            imagen_destruir(imagen);
            arreglo_destruir(luces, luz_destruir);
            arreglo_destruir(objetos, objeto_destruir);
            fprintf(stderr, "Error al abrir el archivo de escritura\n");
            return 1;
        }

        crear_escena(objetos, luces);

        computar_imagen(imagen, atoi(argv[3]), objetos, luces);

        escribir_PPM(imagen, fo);
        
        fclose(fo);
    }
    else if(tiene_extension(argv[4], ".bmp"))
    {
        FILE *fo = fopen(argv[4], "wb");
        if(fo == NULL)
        {
            imagen_destruir(imagen);
            arreglo_destruir(luces, luz_destruir);
            arreglo_destruir(objetos, objeto_destruir);
            fprintf(stderr, "Error al abrir el archivo de escritura\n");
            return 1;
        }

        crear_escena(objetos, luces);

        computar_imagen(imagen, atoi(argv[3]), objetos, luces);

        escribir_BMP(imagen, fo);
        
        fclose(fo);
    }
    else
    {
        imagen_destruir(imagen);
        arreglo_destruir(luces, luz_destruir);
        arreglo_destruir(objetos, objeto_destruir);
        fprintf(stderr, "Formato de archivo no reconocido: %s\n", argv[4]);
        return 1;
    }
    printf("100%%!\n\n");
    imagen_destruir(imagen);
    arreglo_destruir(luces, luz_destruir);
    arreglo_destruir(objetos, objeto_destruir);

    return 0;
}
