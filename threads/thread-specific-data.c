#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

// this is older way of doing things, thread local storage is the new hotness
pthread_key_t key;

void array_destructor(void* arr) {
    free(arr);
    printf("Array freed for a threadn");
}

void* thread_function(void *arg) {
    int* my_array = (int *) malloc(10 * sizeof(int));

    // Use pthread_setspecific to associate a specific value with the 
    // key for the calling thread.
    pthread_setspecific(key, my_array);

    for (int i = 0; i < 10; i++) {
        my_array[i] = i;
    }

    for (int i = 0; i < 10; i++) {
        printf("%d ", my_array[i]);
    }
    printf("\n");

    return NULL;
}

int main() {
    pthread_t thread1, thread2;

    // The pthread_key_create function creates a key that is globally available to all threads but maps to 
    // thread-specific data. Each thread can associate a thread-specific value with this key
    pthread_key_create(&key, array_destructor);

    pthread_create(&thread1, NULL, thread_function, NULL);
    pthread_create(&thread2, NULL, thread_function, NULL);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    pthread_key_delete(key);

    return 0;

}
