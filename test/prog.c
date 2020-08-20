#include <stdio.h>
#include <mpi.h>

 int main(int argc, char *argv[])
 {
 int rank, size;

 MPI_Init(&argc, &argv);

 MPI_Comm_rank(MPI_COMM_WORLD, &rank);
 MPI_Comm_size(MPI_COMM_WORLD, &size);

 // Get the number of processes
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // Get the name of the processor
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;
    MPI_Get_processor_name(processor_name, &name_len);

printf("Hola Mundo! Soy el %d de %d from processor %s \n", rank, size, processor_name);

MPI_Finalize();

return 0;
}
