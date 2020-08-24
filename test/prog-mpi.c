#include <stdio.h>
#include <mpi.h>

#define DATA_SIZE 4096

int main(int argc, char *argv[])
{
    MPI_File fh;
    int buf[DATA_SIZE], rank;
    MPI_Init(0,0);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_File_open(MPI_COMM_WORLD, "testfile.bin", MPI_MODE_CREATE|MPI_MODE_WRONLY, MPI_INFO_NULL, &fh);
    double t_start = MPI_Wtime();     // starts timer for IO
    if (rank == 0)
    // writing with MPI
    MPI_File_write(fh, buf, DATA_SIZE, MPI_INT, MPI_STATUS_IGNORE);
    double t_diff = MPI_Wtime() - t_start;
    printf("Measured %es %.3f at MiB/s\n", t_diff, DATA_SIZE/t_diff/1024/1024);
    MPI_File_close(&fh);
    MPI_Finalize();
    return 0;
}
