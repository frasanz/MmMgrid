CC=gcc 
CCFLAGS=-Wall -O3
Default=multigrid
multigrid: multigrid.c auxiliar.o memoria.o pantalla.o suavizadores.o normas.o
	$(CC) $(CCFLAGS) multigrid.c auxiliar.o memoria.o pantalla.o suavizadores.o normas.o -lm -o multigrid
memoria.o: memoria.c
	$(CC) -c $(CCFLAGS) memoria.c -o memoria.o
auxiliar.o: auxiliar.c
	$(CC) -c $(CCFLAGS) auxiliar.c -o auxiliar.o
pantalla.o: pantalla.c
	$(CC) -c $(CCFLAGS) pantalla.c -o pantalla.o
suavizadores.o: suavizadores.c
	$(CC) -c $(CCFLAGS) suavizadores.c -o suavizadores.o
normas.o: normas.c
	$(CC) -c $(CCFLAGS) normas.c -o normas.o
clean: 
	rm -f multigrid *.o

