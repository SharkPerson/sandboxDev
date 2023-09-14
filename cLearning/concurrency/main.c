#include <stdio.h>

#include <pthread.h>

#define TIMES_TO_COUNT  1000

#define NC "\e[0m"
#define YELLOW "\e[1;33m"

void *thread_routine(void *data) {
    pthread_t tid;
    unsigned int *count;

    count = (unsigned int*)data;

    printf("Count: %u", *count);

    // pthread_self is the PID of this thread.
    tid = pthread_self();
    printf("%sThread [%ld]: The heaviest burden is to exist without living.%s\n", YELLOW, tid, NC);
    for (int i = 0; i < TIMES_TO_COUNT; i++) {
        count += 1; 
    }
    printf("Count: %u", *count);
    return NULL; // The thread end here.
}

int main(void) {
    pthread_t tid1;
    pthread_t tid2;

    unsigned int num = 0;

    // Create the first thread that will execute its thread_routine function.
    pthread_create(&tid1, NULL, thread_routine, &num);
    printf("Main: Created first thread [%ld]\n", tid1);
    pthread_create(&tid2, NULL, thread_routine, &num);
    printf("Main: Created first thread [%ld]\n", tid2);
    
    // Threads share their process memory. Each thread has its own stack but can be accessed with a pointer.
    // The heap and any open file descriptors are shared between threads. This can result in synchronisation errors.
    return 0;
}
