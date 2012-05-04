#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#define NUM_THREADS     5

void *PrintHello(void *threadid) {
  long i, j;
  double k=745.0;
  printf("\nThread %d starting!\n", threadid);
  for(i=0; i<100000; i++) {
    for(j=0; j<1000*(int)threadid; j++) {
      k=k*i/j;
    }
  }
  printf("\nThread %d ending!\n", threadid);
  pthread_exit(NULL);
}

int main (int argc, char *argv[]) {
  long l;
  pthread_t threads[NUM_THREADS];
  int rc, t, *ret;
  for(t=0; t<NUM_THREADS; t++){
    printf("Creating thread %d\n", t);
    rc = pthread_create(&threads[t], NULL, PrintHello, (void *)t);
    if (rc){
      printf("ERROR; return code from pthread_create() %d\n", rc);
      exit(-1);
    }
  }
  printf("Main finished\n");
  pthread_exit(NULL);
}
