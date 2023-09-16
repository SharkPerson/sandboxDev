#include <stdio.h>

#include <pthread.h>

#define TIMES_TO_COUNT  1000

#define NC "\e[0m"
#define YELLOW "\e[33m"
#define BYELLOW "\e[1;33m"
#define RED "\e[31m"
#define GREEN "\e[32m"

typedef struct s_counter {
    pthread_mutex_t count_mutex;
    unsigned int count;
} t_counter;

void *thread_routine(void *data) {
    pthread_t tid;
    t_counter *counter;

    tid = pthread_self();
    counter = (t_counter *)data;

    pthread_mutex_lock(&counter->count_mutex);
    printf("%sThread [%ld]: Count at thread start = %u.%s\n", YELLOW, tid, counter->count, NC);
    pthread_mutex_unlock(&counter->count_mutex);

    // pthread_self is the PID of this thread.
    //printf("%sThread [%ld]: The heaviest burden is to exist without living.%s\n", YELLOW, tid, NC);
    for (int i = 0; i < TIMES_TO_COUNT; i++) {
        pthread_mutex_lock(&counter->count_mutex);
        counter->count++; 
        pthread_mutex_unlock(&counter->count_mutex);
    } 

    pthread_mutex_lock(&counter->count_mutex);
    printf("%sThread [%ld]: Final count = %u.%s\n", YELLOW, tid, counter->count, NC);
    pthread_mutex_unlock(&counter->count_mutex);
    return NULL; // The thread end here.
}

int main(void) {
    pthread_t tid1;
    pthread_t tid2;

    t_counter counter;
    counter.count = 0;
    pthread_mutex_init(&counter.count_mutex, NULL);

    // Create the first thread that will execute its thread_routine function.
    pthread_create(&tid1, NULL, thread_routine, &counter);
    printf("Main: Created first thread [%ld]\n", tid1);
    pthread_create(&tid2, NULL, thread_routine, &counter);
    printf("Main: Created first thread [%ld]\n", tid2);

    // Locks the mutex and the calling thread becomes the owner.
    // If the mutex is already locked by another thread, it stops executing the calling thread until the mutex is unlocked.
    // pthread_mutex_lock(&counter.count_mutex);
    // Does not check the mutex is already locked and that the calling thread is the owner.
    // A mutex could get unlocked by a thread that did not lock it in the first place.
    // So may get "Lock order violation" errors.
    // pthread_mutex_unlock(&counter.count_mutex);
    // Destroys an unlocked mutex. Only checks that the mutex is alrady unlocked.
    // pthread_mutex_destroy(&counter.count_mutex);

    // These joins only guarantees full success if the first thread completes before the second one starts.
    // Data race when the second thread might access the data while the first one is, then they both increment once, but the value only gets incremented once instead of twice.
    pthread_join(tid1, NULL);
    printf("Main: Joined the first thread [%ld]\n", tid1);
    pthread_join(tid2, NULL);
    printf("Main: Joined the second thread [%ld]\n", tid2);

    if (counter.count != (2 * TIMES_TO_COUNT)) {
        printf("%sMain: ERROR ! Total count is %u%s\n", RED, counter.count, NC);
    }
    else {
        printf("%sMain: OK. Total count is %u%s\n", GREEN, counter.count, NC);
    }
    // Threads share their process memory. Each thread has its own stack but can be accessed with a pointer.
    // The heap and any open file descriptors are shared between threads. This can result in synchronisation errors.
    pthread_mutex_destroy(&counter.count_mutex);
    return 0;
}
