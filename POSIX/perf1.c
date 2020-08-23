#include <mpi.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

// Compile with:
// mpicc perf1.c -O3 -o perf1

// This example writes all bytes with one IO call
#define DATA_SIZE 4096

int main(int argc, char ** argv){
  MPI_Init(& argc, & argv);

  // assume we do some compute...

  int data[DATA_SIZE];
  for(int i=0; i < DATA_SIZE; i++){
    data[i] = i;
  }

  // open the file, the time could be included in the measurement
  int fd = open("testfile1.bin", O_CREAT | O_TRUNC | O_WRONLY, S_IWUSR | S_IRUSR);
  if(fd == -1){
    printf("Error opening the file: %s\n", strerror(errno));
    exit(1);
  }
  // now start timer for IO
  double t_start = MPI_Wtime();

  // writing with POSIX is a bit cumbersome due to error checking
  size_t count = DATA_SIZE * sizeof(int);
  size_t pos = 0;
  while(count > 0){
    ssize_t ret = write(fd, ((char*) data) + pos, count);
    if (ret > 0){
      pos += ret;
      count -= ret;
    }else{
      printf("Error in write: %s\n", strerror(errno));
      break;
    }
  }

  double t_diff = MPI_Wtime() - t_start;
  printf("Measured %es %.3f MiB/s\n", t_diff, DATA_SIZE/t_diff/1024/1024);
  close(fd);

  MPI_Finalize();
  return 0;
}
