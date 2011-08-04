CC=gcc
CCFLAGS=-Wall -O3

Default = madt

madt: madt.c io.o multigrid.o
	$(CC) $(CCFLAGS) madt.c io.o multigrid.o -o madt
io.o: io.c
	$(CC) $(CCFLAGS) io.c -c 
multigrid.o: multigrid.c
	$(CC) $(CCFLAGS) multigrid.c -c
clean:
	rm -rf madt *.o
