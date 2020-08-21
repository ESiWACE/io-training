#include <stdio.h>
#include <mpi.h>
int main(int argc, char *argv[])
{
    MPI_File fh;
    MPI_Status status;
    int buf[1000], rank;

    MPI_Init(0,0);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_File_open(MPI_COMM_SELF, "test.txt", MPI_MODE_CREATE | MPI_MODE_RDWR,
                   MPI_INFO_NULL, &fh);
    MPI_File_set_view(fh, 0, MPI_INT, MPI_INT, "native", MPI_INFO_NULL);
    MPI_File_write(fh, buf, 100, MPI_INT, &status);
    MPI_File_close(&fh);

    MPI_File_open(MPI_COMM_SELF, "test.txt", MPI_MODE_CREATE | MPI_MODE_RDWR,
                  MPI_INFO_NULL, &fh);
    MPI_File_set_view(fh, 0, MPI_INT, MPI_INT, "native", MPI_INFO_NULL);
    MPI_File_read(fh, buf, 100, MPI_INT, &status);
    MPI_File_close(&fh);
    MPI_Finalize();
    return 0;
}
