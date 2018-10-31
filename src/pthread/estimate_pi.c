//
// Created by gzq on 18-7-12.
//
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define min(a, b) (((a) < (b)) ? (a) : (b))

int seed;
double rand_double(double a, double b);

long long total_count = 100000000;
int thread_count = 1;
long long total_hit_number = 0;
pthread_mutex_t *mutex;

double rand_double(double a, double b) {
    return (rand_r(&seed) / (double) RAND_MAX) * abs((b - a)) + min(a, b);
}

void *func(void *rank) {
    long long my_count = total_count / thread_count;
    long long my_hit_count = 0;
    for (int i = 0; i < my_count; i++) {
        double x = rand_double(-1, 1);
        double y = rand_double(-1, 1);
        if (x * x + y * y <= 1) {
            my_hit_count++;
        }
    }
    printf("in thread %ld ,  my total number = %lld , my hit number = %lld , estimate_pi = %lf\n", (long) rank,
           my_count, my_hit_count, (double) my_hit_count * 4 / my_count);
    pthread_mutex_lock(mutex);
    total_hit_number += my_hit_count;
    pthread_mutex_unlock(mutex);
};

int main() {
    seed = 123;
    mutex = malloc(sizeof(pthread_mutex_t) * 1);
    pthread_mutex_init(mutex, NULL);
    pthread_t *pthread_handles;
    pthread_handles = malloc(sizeof(pthread_t) * thread_count);
    time_t t1 = time(NULL);
    for (long i = 0; i < thread_count; i++) {
        pthread_create(&pthread_handles[i], NULL, func, (void *) i);
    }
    printf("main thread compete...\n");
    for (int i = 0; i < thread_count; i++) {
        pthread_join(pthread_handles[i], NULL);
    }
    time_t t2 = time(NULL);
    printf("cost time : %ld s\n", t2 - t1);
    printf("in main thread , total number = %lld , hit number = %lld , estimate_pi = %lf\n", total_count,
           total_hit_number, (double) total_hit_number * 4 / total_count);
    pthread_mutex_destroy(mutex);
    free(mutex);
    free(pthread_handles);
    return EXIT_SUCCESS;
}
