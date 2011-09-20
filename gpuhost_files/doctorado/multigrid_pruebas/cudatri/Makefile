CC=nvcc
CFLAGS=-arch sm_13
LDFLAGS=
SOURCES=multigrid.cu auxiliares.cu operador.cu io.cu
OBJECTS=$(SOURCES:.cu=.o)
EXECUTABLE=multigrid

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJECTS) -o $@

%.o : %.cu
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@

clean:
	rm -rf *.o multigrid


