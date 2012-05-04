#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUMTHRDS 4

int maxtemp;

pthread_mutex_t maxtemp_m;

void * getTemp(void *threadid) {
  int mytemp;

  while(1) {
    // simulate reading temp
    mytemp = (int) (random()%50);
    printf("Thread %d read a temp of %d\n", (int)threadid, mytemp);
    // check and update
    pthread_mutex_lock(&maxtemp_m);
    if(mytemp>maxtemp) {
      maxtemp=mytemp;
    }
    pthread_mutex_unlock(&maxtemp_m);
    sleep(1);
  }
}

int main (int argc, char *argv[]) {
  int i;
  pthread_t threads[NUMTHRDS];
  /* initialize the mutex */
  pthread_mutex_init(&maxtemp_m, NULL);
  /* Create threads to perform the dotproduct  */
  for(i=0;i<NUMTHRDS;i++) {
    pthread_create(&threads[i], NULL, getTemp, (void *)i); 
  }
  while(1) {
    sleep(5); 
    pthread_mutex_lock(&maxtemp_m);
    printf("Max temperature is now: %d\n", maxtemp);
    maxtemp=0;
    pthread_mutex_unlock(&maxtemp_m);
  }
}
