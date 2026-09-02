#include <pthread.h>
#include <stdio.h>

#define THREAD_COUNT 10

pthread_spinlock_t counter_lock;
int counter = 0;

/*

    NOTE: BE SURE YOU KNOW WHEN TO uSE SPINLOCK! If you don't use
    a mutex (prob)
*/

// Thread function to execute.
void *thread_target(void *vargs) {

    pthread_spin_lock(&counter_lock); 
    for (int i = 0; i < 1000000; i++) {
        counter++;
    }
    
    pthread_spin_unlock(&counter_lock);
    
    printf("Counter is %d\n", counter);

}

int main() {
    pthread_t threads[THREAD_COUNT];

    pthread_spin_init(&counter_lock, 0);
    
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