#include <pthread.h>
#include <stdio.h>

// when compiling remember to pass the -pthread flag into gcc
// e.g: gcc -o spawn-thread spawn-thread.c -pthread

#define THREAD_COUNT 10

int counter = 0;

// Thread function to execute.
void *thread_target(void *vargs) {

    // critical section
    //mutex, semaphores
    for (int i = 0; i < 1000000; i++) {
        counter++;
    }

    printf("Counter is %d\n", counter);
}

int main() {
    pthread_t threads[THREAD_COUNT];
    
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