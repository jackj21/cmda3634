CC := mpicc

INCLUDES :=
LIBS := -lm

all: test_print_vector test_distributed_axpy time_distributed_axpy

test_print_vector:
	$(CC) $(INCLUDES) -o test_print_vector test_print_vector.c vector.c $(LIBS)

test_distributed_axpy:
	$(CC) $(INCLUDES) -o test_distributed_axpy test_distributed_axpy.c vector.c $(LIBS)

time_distributed_axpy:
	$(CC) $(INCLUDES) -o time_distributed_axpy time_distributed_axpy.c vector.c $(LIBS)

clean:
	rm -f test_print_vector
	rm -f test_distributed_axpy
	rm -f time_distributed_axpy
	rm -f *.o
