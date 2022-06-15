all: TP1

TP1: main.o imagen.o arreglo.o objeto.o luz.o esfera.o malla.o plano.o triangulo.o color.o vector.o
	gcc -Wall -pedantic -std=c99 -g main.o imagen.o arreglo.o objeto.o luz.o esfera.o malla.o plano.o triangulo.o color.o vector.o -o TP1 -lm

main.o: main.c imagen.h arreglo.h objeto.h luz.h color.h vector.h
	gcc -Wall -pedantic -std=c99 -g -c main.c
	
imagen.o: imagen.c imagen.h color.h
	gcc -Wall -pedantic -std=c99 -g -c imagen.c

arreglo.o: arreglo.c arreglo.h 
	gcc -Wall -pedantic -std=c99 -g -c arreglo.c

luz.o: luz.c luz.h color.h
	gcc -Wall -pedantic -std=c99 -g -c luz.c

objeto.o: objeto.c objeto.h esfera.h malla.h plano.h triangulo.h color.h vector.h
	gcc -Wall -pedantic -std=c99 -g -c objeto.c

esfera.o: esfera.c esfera.h vector.h
	gcc -Wall -pedantic -std=c99 -g -c esfera.c

malla.o: esfera.c malla.h arreglo.h triangulo.h vector.h 
	gcc -Wall -pedantic -std=c99 -g -c malla.c

plano.o: esfera.c plano.h vector.h
	gcc -Wall -pedantic -std=c99 -g -c plano.c

triangulo.o: triangulo.c triangulo.h vector.h
	gcc -Wall -pedantic -std=c99 -g -c triangulo.c

color.o: color.c color.h
	gcc -Wall -pedantic -std=c99 -g -c color.c

vector.o: vector.c vector.h
	gcc -Wall -pedantic -std=c99 -g -c vector.c

clean:
	rm *.o
	rm TP1
