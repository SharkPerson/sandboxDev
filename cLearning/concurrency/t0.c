#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>

void *mythread(void *arg) {

}

int main (int argc, char* argv[]) {
    if (argc != 1) {
        fprintf(stderr, "Usage: main\n");
        exit(1);
    }

    pthread_t t1, t2;

    // Initialising a mutex
    pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
    // or use the init function
    int rc = pthread_mutex_init(&lock, NULL);
    assert(rc == 0); // Always check for success
    printf("Main: begin\n");
    
    pthread_create(&t1, NULL, mythread, "A");
    pthread_create(&t2, NULL, mythread, "B");
    // Main thread will wait for these two thread to finish before continuing.
    //pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("Main: end\n");
    return 0;
}
