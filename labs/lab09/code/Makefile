all: timing test_c_read_write test_c_mpi_read_write test_mpi_read_write

timing:
	mpicc -o timing timing.c vector.c

test_c_read_write:
	mpicc -o test_c_read_write test_c_read_write.c vector.c

test_c_mpi_read_write:
	mpicc -o test_c_mpi_read_write test_c_mpi_read_write.c vector.c

test_mpi_read_write:
	mpicc -o test_mpi_read_write test_mpi_read_write.c vector.c

clean:
	rm -f timing
	rm -f test_c_read_write
	rm -f test_c_mpi_read_write
	rm -f test_mpi_read_write
