#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <mpi.h>
#include "vector.h"


// M = total number of entries in global array
// P = number of processors in communicator over which to distribute the array
// k = rank of this process
// Mk will be overwritten with local number of entries
// r0 will be overwritten with start index of local subset


/*
* Computes properties of the local subvector of a specific rank
*
* Arguments:
*   N: Global vector size
*   P: Total processors
*   k: rank to get the size of
*   Nk: (return) Size of local subvector on rank k
*   r0: (return) Index of first local entry in global coordinates
*
* Returns:
*   error code, 0 for success, 1 for failure
*/
int compute_subvector_size(int N, int P, int k, int* Nk, int* r0){

    *Nk = N/P;
    *r0 = (*Nk)*k;

    if (k == P-1){
        *Nk += N%P;
    }

    return 0;
}


/*
* Allocator for a vector
*
* Arguments:
*   v: Vector to allocate
*   N: Global vector size
*   padding: Width of ghost region
*   comm: Communicator over which the vector is partitioned
*
* Returns:
*   error code, 0 for success, 1 for failure
*/
int allocate_Vector(Vector* v, int N, int padding, MPI_Comm comm){

    int rank, size;
    MPI_Comm_size(comm, &size);
    MPI_Comm_rank(comm, &rank);

    int N_local, r0;
    int flag = 0;
    flag = compute_subvector_size(N, size, rank, &N_local, &r0);
    v->padding = padding;

    v->N_local = N_local;
    v->N_padded = N_local + 2*padding;
    v->N_global = N;

    v->r0 = r0;

    v->comm = comm;

    v->data = (float*)malloc((v->N_padded)*sizeof(float));
    if(v->data == NULL){
        fprintf(stderr, "Error allocating vector. \n");
    return 1;
    }

    return flag;
}


/*
* Initializer for a vector
*
* Arguments:
*   v: Vector to initialize
*
* Returns:
*   error code, 0 for success, 1 for failure
*/
int initialize_Vector(Vector* v){

    memset(v->data, 0, v->N_padded*sizeof(float));

    return 0;
}


/*
* Deallocator for a vector
*
* Arguments:
*   v: Vector to deallocate
*
* Returns:
*   error code, 0 for success, 1 for failure
*/
int deallocate_Vector(Vector* v){

    v->padding = 0;

    v->N_local = 0;
    v->N_padded = 0;
    v->N_global = 0;

    v->r0 = -1;

    v->comm = MPI_COMM_NULL;

    free(v->data);
    v->data = NULL;

    return 0;
}


/*
* Fills a vector with random uniform [0,1] values.
*
* Fills local values only, not padded values.
*
* Arguments:
*   v: Vector to fill
*
* Returns:
*   error code, 0 for success, 1 for failure
*/

int rand_fill_Vector(Vector *v){

    // check that N of vector is reasonable
    if(v->N_local < 1)
        return 1;

    // loop through and fill vector with random numbers 0 to 1
    for(int i=0; i < v->N_local; i++){
        v->data[i] = (float)rand() / (float)RAND_MAX;
    }
    return 0;
}


/*
* Compares two vectors to see if they have exactly the same contents
*
* Arguments:
*   v: Vector to compare
*   u: Vector to compare
*
* Returns:
*   1 for match, 0 for no match
*/

int compare_Vector(const Vector* v, const Vector* u){

    int rank, n_procs;

    MPI_Comm_rank(v->comm, &rank);
    MPI_Comm_size(v->comm, &n_procs);

    if(v->N_padded != u->N_padded){
        printf("rank %d: V size != U size -- %d vs %d\n", rank, v->N_padded, u->N_padded);
        return 0;
    }

    for(int i=0; i<v->N_padded; i++){
        if (v->data[i] != u->data[i]){
            printf("rank %d: V[%d]=%f does not match U[%d]=%f\n", rank, i, v->data[i], i, u->data[i]);
            return 0;
        }
    }

    return 1;
}


/*
* Printer for a vector
*
* Arguments:
*   v: Vector to print
*
* Returns:
*   error code, 0 for success, 1 for failure
*/
int print_Vector(const Vector* v){

    int rank, n_procs;

    MPI_Comm_rank(v->comm, &rank);
    MPI_Comm_size(v->comm, &n_procs);

    char output[50000];
    memset(output, 0, 50000);

    int k = 0;
    for(int j=0; j < v->N_padded; j++){
        k += sprintf(output+k, "%.10f\n", v->data[j]);
    }
    sprintf(output+k, "\n");

    MPI_Barrier(v->comm);

    for(int i=0; i<n_procs; i++){
        if (rank == i){
            printf("%s", output);
        }
        MPI_Barrier(v->comm);
    }

    usleep(300);
    fflush(stdout);
    MPI_Barrier(v->comm);
}


/*
* Read a vector from disk.  Acts as an allocator.
*
* Single processor, C implementation.
*
* Arguments:
*   v: Vector to read into
*   filename: filename to read from
*
* Returns:
*   error code, 0 for success, 1 for failure
*/

int c_read_Vector(Vector* v, const char* filename){

    FILE* f;
    unsigned int n_data;
    int n_read_data;

    // Open the file
    f = fopen(filename, "rb");
    if (f == NULL) {
        fprintf(stderr, "Error opening file %s for read.\n", filename);
        return 1;
    }

    // Read the header to get how big the array is
    n_read_data = fread(&n_data, sizeof(unsigned int), 1, f);
    if (n_read_data != 1) {
        fprintf(stderr, "Error reading file %s header.\n", filename);
        return 1;
    }

    allocate_Vector(v, n_data, 0, MPI_COMM_SELF);

    // Read the vector in
    n_read_data = fread(v->data, sizeof(float), n_data, f);
    if (n_read_data != n_data) {
        fprintf(stderr, "Error reading file %s data.\n", filename);
        return 1;
    }

    // Close the file
    fclose(f);
}


/*
* Write a vector to disk.
*
* Single processor, C implementation.
*
* Arguments:
*   v: Vector to write
*   filename: filename to write to
*
* Returns:
*   error code, 0 for success, 1 for failure
*/

int c_write_Vector(Vector* v, const char* filename){

    FILE* f;
    unsigned int n_data = v->N_padded;
    int n_written_data;

    // Open the file
    f = fopen(filename, "wb");
    if (f == NULL) {
        fprintf(stderr, "Error opening file %s for write.\n", filename);
        return 1;
    }

    // Write the header
    n_written_data = fwrite(&n_data, sizeof(unsigned int), 1, f);
    if (n_written_data != 1) {
        fprintf(stderr, "Error writing file %s header.\n", filename);
        return 1;
    }

    allocate_Vector(v, n_data, 0, MPI_COMM_SELF);

    // Write the vector out
    n_written_data = fwrite(v->data, sizeof(float), n_data, f);
    if (n_written_data != n_data) {
        fprintf(stderr, "Error writing file %s data.\n", filename);
        return 1;
    }

    // Close the file
    fclose(f);
    return 0;
}


/*
* Read a vector from disk.  Acts as an allocator.
*
* Parallel, C+MPI implementation.
*
* Arguments:
*   v: Vector to read into
*   filename: filename to read from
*   comm: communicator to parallelize over
*
* Returns:
*   error code, 0 for success, 1 for failure
*/

int c_mpi_read_Vector(Vector* v, const char* filename, MPI_Comm comm){

    // <student>
    // Open the file
	MPI_Status status;
	int rank, size;
	MPI_Comm_size(comm, &size);
	MPI_Comm_rank(comm, &rank);

	FILE* f;
	unsigned int n_data;	
	int n_read_data;
	
	if (rank == 0) {
		f = fopen(filename, "rb");
		if (f == NULL) {
			fprintf(stderr, "Error opening file %s for read.\n", filename);
			return 1;
		}
				
    	// Read the header information
		n_read_data = fread(&n_data, sizeof(unsigned int), 1, f);
		if (n_read_data != 1) {
			fprintf(stderr, "Error reading file %s header.\n", filename);
			return 1;
		}
			
    	// Share the total vector size with everyone.
		MPI_Bcast(&n_data, 1, MPI_UNSIGNED, 0, comm);
		
		// Read the local data
		allocate_Vector(v, n_data, 0, comm);
		n_read_data = fread(v->data, sizeof(float), v->N_padded, f);

		if (n_read_data != v->N_padded) {
			fprintf(stderr, "Error reading file %s data.\n", filename);
			return 1;
		}
		
		int share_offset;	
    	// Get the max size of the receive buffer
		compute_subvector_size(v->N_global, size, size-1, &n_data, &share_offset);
		float* buffer = malloc(n_data*sizeof(float));

    	// Read and share everyone else's data
		for (int p=1; p<size; p++) {
			compute_subvector_size(v->N_global, size, p, &n_data, &share_offset);
			n_read_data = fread(buffer, sizeof(float), n_data, f);
			if (n_read_data != n_data) {
				fprintf(stderr, "Error reading file %s data.\n", filename);
				free(buffer);
				return 1;
			}
			MPI_Send(buffer, n_data, MPI_FLOAT, p, 0, comm); 
		}
		free(buffer);

    	// Close the file
		fclose(f);
	}

    // Get my local data from the process doing the reading
	else {
		allocate_Vector(v, n_data, 0, comm);
		MPI_Recv(v->data, n_data, MPI_FLOAT, 0, 0, comm, &status);
	}	
    // </student>
    return 0;
}


/*
* Write a vector to disk.
*
* Parallel, C+MPI implementation.
*
* Arguments:
*   v: Vector to write
*   filename: filename to write to
*
* Returns:
*   error code, 0 for success, 1 for failure
*/

int c_mpi_write_Vector(Vector* v, const char* filename){

    MPI_Status status;
    int rank, size;
    MPI_Comm_size(v->comm, &size);
    MPI_Comm_rank(v->comm, &rank);

    FILE* f;
    unsigned int n_data = v->N_padded;
    int n_written_data;


    // Write the header information
    if (rank == 0){

        f = fopen(filename, "wb");
        if (f == NULL) {
            fprintf(stderr, "Error opening file %s for write.\n", filename);
            return 1;
        }

        // Write the header
        n_written_data = fwrite(&v->N_global, sizeof(unsigned int), 1, f);
        if (n_written_data != 1) {
            fprintf(stderr, "Error writing file %s header.\n", filename);
            return 1;
        }

        // Write the local data
        n_written_data = fwrite(v->data, sizeof(float), n_data, f);
        if (n_written_data != n_data) {
            fprintf(stderr, "Error writing file %s data.\n", filename);
            return 1;
        }

        int share_offset;
        // Get the max size of the receive buffer
        compute_subvector_size(v->N_global, size, size-1, &n_data, &share_offset);
        float* buffer = malloc(n_data*sizeof(float));

        // Write everyone else's data
        for(int p=1; p<size; p++){
            compute_subvector_size(v->N_global, size, p, &n_data, &share_offset);

            MPI_Recv(buffer, n_data, MPI_FLOAT, p,  0,  MPI_COMM_WORLD, &status);
            n_written_data = fwrite(buffer, sizeof(float), n_data, f);
            if (n_written_data != n_data) {
                fprintf(stderr, "Error writing file %s data for rank %d.\n", filename, p);
                return 1;
            }

        }

        free(buffer);

        // Close the file
        fclose(f);
    }
    else{
        // Share local data with the process doing the writing
        MPI_Send(v->data, n_data, MPI_FLOAT, 0,  0,  MPI_COMM_WORLD);
    }

    return 0;
}


/*
* Read a vector from disk.  Acts as an allocator.
*
* Parallel, MPI I/O implementation.
*
* Arguments:
*   v: Vector to read into
*   filename: filename to read from
*   comm: communicator to parallelize over
*
* Returns:
*   error code, 0 for success, 1 for failure
*/

int mpi_read_Vector(Vector* v, const char* filename, MPI_Comm comm){

    int rank, size;
    MPI_Comm_size(comm, &size);
    MPI_Comm_rank(comm, &rank);

    MPI_File f;
    MPI_Status status;
    MPI_Offset offset;
    unsigned int n_data;
    int n_read_data;

    // Header is hard-coded for this type.
    unsigned int header_size = sizeof(unsigned int);

    // Read the header to get total vector size
    if (rank == 0){

        FILE* f = fopen(filename, "rb");
        if (f == NULL) {
            fprintf(stderr, "Error opening file %s for read.\n", filename);
            return 1;
        }

        // Read the header to get how big the array is
        n_read_data = fread(&n_data, sizeof(unsigned int), 1, f);
        if (n_read_data != 1) {
            fprintf(stderr, "Error reading file %s header.\n", filename);
            return 1;
        }
        fclose(f);
    }

    // Share the total vector size with everyone.
    MPI_Bcast(&n_data, 1, MPI_UNSIGNED, 0, comm);
    allocate_Vector(v, n_data, 0, comm);

    // Open the file
    MPI_File_open(MPI_COMM_WORLD, filename, MPI_MODE_RDONLY, MPI_INFO_NULL, &f);

    // Compute offset for my write
    offset = header_size + sizeof(float)*v->r0;

    // Read data from file in parallel
    MPI_File_read_at_all(f, offset, v->data, v->N_padded, MPI_FLOAT, &status);

    // Verify that required data has been read
    MPI_Get_count(&status, MPI_FLOAT, &n_read_data);
    if (n_read_data != v->N_padded) {
        fprintf(stderr, "Error reading file %s data for rank %d.  Got %d expected %d.\n", filename, rank, n_read_data, v->N_padded);
        return 1;
    }

    // Close the file
    MPI_File_close(&f);

    return 0;
}


/*
* Write a vector to disk.
*
* Parallel, MPI I/O implementation.
*
* Arguments:
*   v: Vector to write
*   filename: filename to write to
*
* Returns:
*   error code, 0 for success, 1 for failure
*/

int mpi_write_Vector(Vector* v, const char* filename){


    // <student>
	int rank, size;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Write the header
	
    // Open the file
    // Compute offset for my write
    // Write data to file in parallel
    // Verify that required data has been written
    // Close the file

    // </student>

    return 0;
}


