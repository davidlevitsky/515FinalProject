#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>


void probeArray(char *buffer, int stride, int buf_size) {
  int num_passes = 1 << 10;
  int pass;
  int access = 1; 
  // Access array using stride parameter.
  for (pass = 0; pass < num_passes; pass++) {
      if (access >= buf_size) {
        return;
      }
        ++buffer[access];
        // Stride is the offset we use for the memory access.
        // Passed in from the user.
        // If it's bigger than 4096 it will cause page faults,
        // which will slow the system down significantly.
        access += stride;
  }

}


int main(int argc, char *argv[]) {
  int c;
  FILE *file;
  int i;
  int counter = 0;
  // Provide user with option to control memory access.
  // We recommend using 4096 to cause page faults.
  int stride = atoi(argv[1]);
  
  // Hardcoded size of the large file that we are reading.
  // Text of Sherlock Holmes.
  int file_size = 6488396;
  // Allocate a giant array on the heap
  char *buffer = malloc(file_size);

  file = fopen("large_file.txt", "r");

  // Read file in one byte at a time
  if (file) {
    while ((c = getc(file)) != EOF) {
      buffer[counter++] = c;
      probeArray(buffer, stride, file_size);
      // Use write call instead of print because this is a syscall.
      // Will cause the program to switch from user to kernel privilege
      // Overhead from KPTI should cause a slowdown here. 
      write(1, &c, 1);
    }
    
    fclose(file);
  }
  
  printf("%s", "Finished benchmark.");
}

