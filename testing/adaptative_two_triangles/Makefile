CC=gcc
CCFLAGS=-Wall -O3 
INCLUDES=-lm
Default = madt

madt: madt.c io.o multigrid.o operator.o multigrid_kernel.o aux_multigrid.o 
	$(CC) $(CCFLAGS) madt.c io.o multigrid.o operator.o multigrid_kernel.o aux_multigrid.o $(INCLUDES) -o madt
io.o: io.c
	$(CC) $(CCFLAGS) io.c -c 
multigrid.o: multigrid.c
	$(CC) $(CCFLAGS) multigrid.c -c
multigrid_kernel.o: multigrid_kernel.c
	$(CC) $(CCFLAGS) multigrid_kernel.c -c
operator.o: operator.c
	$(CC) $(CCFLAGS) operator.c -c
aux_multigrid.o: aux_multigrid.c
	$(CC) $(CCFLAGS) aux_multigrid.c -c
clean:
	rm -rf madt *.o
