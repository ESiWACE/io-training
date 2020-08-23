#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

// Compile with:
// mpicc perf1.c -O3 -o perf1
// gcc perf1.c -O3 -o perf1

// This example now writes every byte with one IO call

#define DATA_SIZE 4096

int main(int argc, char ** argv){

  // assume we do some compute...

  int data[DATA_SIZE];
  for(int i=0; i < DATA_SIZE; i++){
    data[i] = i;
  }

  // open the file, the time could be included in the measurement
  FILE *fd = fopen("testfile2222.txt", "w");
  if(fd == NULL){
    printf("Error opening the file: %s\n", strerror(errno));
    exit(1);
  }
  // now start timer for IO
  clock_t t;
  t = clock();

  // writing without parallel I/O
  for(size_t pos=0; pos < DATA_SIZE * sizeof(int); pos+= sizeof(int)){
    ssize_t ret = fwrite(((char*) data) + pos, 1, sizeof(int), fd);
    if (ret != sizeof(int)){
      printf("Error in write: %s\n", strerror(errno));
      break;
    }
  }

  t = clock() - t;
  double t_diff = ((double)t)/CLOCKS_PER_SEC; // calculate the elapsed time
  printf("\nMeasured %es %.3f MiB/s\n", t_diff, DATA_SIZE/t_diff/1024/1024);
  fclose(fd);

  return 0;
}
