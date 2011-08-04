CC=gcc
CCFLAGS=-Wall -O3

Default = madt

madt: madt.c io.o multigrid.o operator.o
	$(CC) $(CCFLAGS) madt.c io.o multigrid.o operator.o -o madt
io.o: io.c
	$(CC) $(CCFLAGS) io.c -c 
multigrid.o: multigrid.c
	$(CC) $(CCFLAGS) multigrid.c -c
operator.o: operator.c
	$(CC) $(CCFLAGS) operator.c -c
clean:
	rm -rf madt *.o
