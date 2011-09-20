CC=gcc
CFLAGS=-Wall -O3 -fopenmp
all:prueba1 paralelo
prueba1: prueba1.o -lgsl -lgslcblas -lm
paralelo: paralelo.o -lgsl -lgslcblas -lm
clean:
	rm paralelo prueba1
