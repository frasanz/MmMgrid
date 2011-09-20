CC=gcc
CCFLAGS=-Wall -pg
Default=multigrid

multigrid: multigrid.c  memoria.o pantalla.o auxiliares.o operador.o suavizador.o
	$(CC) $(CCFLAGS) multigrid.c memoria.o pantalla.o auxiliares.o operador.o suavizador.o -lm -o multigrid
memoria.o: memoria.c
	$(CC) -c $(CCFLAGS) memoria.c -lm -o  memoria.o
pantalla.o: pantalla.c
	$(CC) -c $(CCFLAGS) pantalla.c -lm -o pantalla.o
auxiliares.o: auxiliares.c
	$(CC) -c $(CCFLAGS) auxiliares.c -lm -o auxiliares.o
operador.o: operador.c
	$(CC) -c $(CCFLAGS) operador.c -lm -o operador.o
suavizador.o : suavizador.c
	$(CC) -c $(CCFLAGS) suavizador.c -lm -o suavizador.o
clean:
	rm -f *.o multigrid
