//
// Created by gzq on 18-7-12.
//

#include "thread_create_test.h"
#include <stdio.h>
#include <stdlib.h>
#include "../../util/time_utils/time_utils.h"

void *func(void *rank) {}

int main(int argc, char **argv) {
  if (argc > 1) {
    thread_count = atoi(argv[1]);
  } else {
    thread_count = 100;
  };
  pthread_t *pthreadz_handles;
  pthread_handles = malloc(sizeof(pthread_t) * thread_count);
  click_s();
  for (long i = 0; i < thread_count; i++) {
    pthread_create(&pthread_handles[i], NULL, func, (void *)i);
  }
  for (int i = 0; i < thread_count; i++) {
    pthread_join(pthread_handles[i], NULL);
  }
  click_e();
  printf("thread count = %d , cost time = %ld ms average time = %lf \n",
         thread_count, milli_interval(),
         (double)milli_interval() / thread_count);
  free(pthread_handles);
}