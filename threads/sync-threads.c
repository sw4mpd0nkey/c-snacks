#include <pthread.h>
#include <stdio.h>

// when compiling remember to pass the -pthread flag into gcc
// e.g: gcc -o spawn-thread spawn-thread.c -pthread

#define THREAD_COUNT 10

int counter = 0;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

// Thread function to execute.
void *thread_target(void *vargs) {

    /*
        uses a mutex to block so only one thread can do this critical
        sectoin at a time. this is very unperformant tho, since there 
        is only ever 1 thread that gets to pass and 9 in starvation
    */
    pthread_mutex_lock(&lock);
    
    for (int i = 0; i < 1000000; i++) {
        counter++;
    }
    
    pthread_mutex_unlock(&lock);
    
    printf("Counter is %d\n", counter);

    return NULL;
}

int main() {
    pthread_t threads[THREAD_COUNT];

    pthread_mutex_init(&lock, NULL);
    
    for (int i = 0; i < THREAD_COUNT; i++) {

        if (pthread_create(&threads[i], NULL, thread_target, NULL) != 0) {
            perror("Failed to create thread");
            return -1;
        }  
    }
    
    // pthread_join blocks, pthread_detach doesm't
    for (int i = 0; i < THREAD_COUNT; i++) {
        pthread_join(threads[i], NULL);   
    }
    
    return 0;
}