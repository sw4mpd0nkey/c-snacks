#include <pthread.h>
#include <stdio.h>

// when compiling remember to pass the -pthread flag into gcc
// e.g: gcc -o spawn-thread spawn-thread.c -pthread

#define THREAD_COUNT 10

// Thread function to execute.
void *thread_function(void *arg) {
    printf("Hello from the thread!\n");
    return NULL;
}

int main() {
    pthread_t threads[THREAD_COUNT];
    
    for (int i = 0; i < THREAD_COUNT; i++) {
        /* Create the thread;
         * int pthread_create(pthread_t *restrict thread,
                          const pthread_attr_t *_Nullable restrict attr,
                          typeof(void *(void *_Nullable)) *start_routine,
                          void *_Nullable restrict arg);
            NOTE - if you need to pass many args wrap tnem in a strut
        */
        if (pthread_create(&threads[i], NULL, thread_function, NULL) != 0) {
            perror("Failed to create thread");
            return -1;
        }  
       
        // pthread_join blocks, pthread_detach doesm't
        for (int i = 0; i < THREAD_COUNT; i++) {
            pthread_join(&threads[i], NULL);   
        }
    }

    return 0;
}