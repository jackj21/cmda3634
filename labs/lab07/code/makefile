all: demo_axpy \
     test_reduction \
     demo_inner_product \
     test_broadcast \
     demo_normalize \
     time_normalize

demo_axpy:
	mkdir -p bin
	mpicc -o bin/demo_axpy demo_axpy.c vector.c -lm

test_reduction:
	mkdir -p bin
	mpicc -o bin/test_reduction test_reduction.c vector.c -lm

demo_inner_product:
	mkdir -p bin
	mpicc -o bin/demo_inner_product demo_inner_product.c vector.c -lm

test_broadcast:
	mkdir -p bin
	mpicc -o bin/test_broadcast test_broadcast.c vector.c -lm

demo_normalize:
	mkdir -p bin
	mpicc -o bin/demo_normalize demo_normalize.c vector.c -lm

time_normalize:
	mkdir -p bin
	mpicc -o bin/time_normalize time_normalize.c vector.c -lm

clean:
	rm -f bin/*
