all: test_linear_function test_quadratic_function

test_linear_function:
	mpicc -o test_linear_function test_linear_function.c vector.c

test_quadratic_function:
	mpicc -o test_quadratic_function test_quadratic_function.c vector.c

clean:
	rm -rf *.o
	rm -f test_linear_function
	rm -f test_quadratic_function
