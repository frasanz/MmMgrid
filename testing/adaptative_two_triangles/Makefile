CC=gcc
CCFLAGS=-Wall -O3 -g
INCLUDES=-lm
Default = madt

madt: madt.c io.o multigrid.o operator.o multigrid_kernel.o aux_multigrid.o smoothers.o smooth_check.o \
      restrict.o interpolate.o
	$(CC) $(CCFLAGS) madt.c io.o multigrid.o operator.o multigrid_kernel.o \
	                 aux_multigrid.o smooth_check.o smoothers.o restrict.o interpolate.o $(INCLUDES) -o madt
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
smoothers.o: smoothers.c
	$(CC) $(CCFLAGS) smoothers.c -c
smooth_check.o: smooth_check.c
	$(CC) $(CCFLAGS) smooth_check.c -c
restrict.o: restrict.c
	$(CC) $(CCFLAGS) restrict.c -c
interpolate.o: interpolate.c
	$(CC) $(CCFLAGS) interpolate.c -c
clean:
	rm -rf madt *.o original_sub_mesh restrict_sub_mesh interpolate_sub_mesh *out
