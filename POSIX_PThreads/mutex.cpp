#include <pthread.h>
#include <iostream>

#define NUM_THREADS 4

// Shared variable
int shared_variable = 0;

// Mutex for protecting the shared variable
pthread_mutex_t mutex;

void* add_thread_id(void* threadid) {
    long tid = (long)threadid;

    // Lock the mutex before modifying the shared variable
    pthread_mutex_lock(&mutex);

    // Read the current value of the shared variable
    int current_value = shared_variable;
    std::cout << "Thread #" << tid << " read shared_variable: " << current_value << std::endl;

    // Add thread ID to the shared variable
    shared_variable = current_value + tid;
    std::cout << "Thread #" << tid << " updated shared_variable to: " << shared_variable << std::endl;

    // Unlock the mutex after modifying the shared variable
    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];
    int rc;
    long t;

    // Initialize the mutex
    pthread_mutex_init(&mutex, NULL);

    for (t = 0; t < NUM_THREADS; t++) {
        std::cout << "In main: creating thread " << t << std::endl;
        rc = pthread_create(&threads[t], NULL, add_thread_id, (void*)t);
        if (rc) {
            std::cerr << "ERROR; return code from pthread_create() is " << rc << std::endl;
            exit(-1);
        }
    }

    // Wait for all threads to complete
    for (t = 0; t < NUM_THREADS; t++) {
        pthread_join(threads[t], NULL);
    }

    // Destroy the mutex
    pthread_mutex_destroy(&mutex);

    std::cout << "Final value of shared_variable: " << shared_variable << std::endl;

    // Exit the main thread
    pthread_exit(NULL);
}
