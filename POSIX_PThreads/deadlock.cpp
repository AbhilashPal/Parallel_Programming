#include <pthread.h>
#include <iostream>

#define NUM_THREADS 2

// Shared variable
int shared_variable = 0;

struct ThreadArgs {
    long thread_id;
    int additional_value;
};

void* add_thread_id(void* threadid, void* sumvalue) {
    long tid = (long)threadid;
    int sum = (int)sumvalue;

    // Read the current value of the shared variable
    int current_value = shared_variable;
    std::cout << "Thread #" << tid << " read shared_variable: " << current_value << std::endl;

    // Add thread ID to the shared variable
    shared_variable = current_value + sum;
    std::cout << "Thread #" << tid << " updated shared_variable to: " << shared_variable << std::endl;

    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];
    int rc;
    long t;
    std::vector<ThreadArgs> thread_args(NUM_THREADS);

    // Fill thread_args vector with thread IDs and additional values
    for (long i = 0; i < NUM_THREADS; ++i) {
        thread_args[i].thread_id = i;
        thread_args[i].additional_value = 10;  // Example additional value
    }

     // Create threads in random order
    for (const auto& args : thread_args) {
        std::cout << "In main: creating thread " << args.thread_id << std::endl;
        rc = pthread_create(&threads[args.thread_id], NULL, add_thread_id, (void*)&args);
        if (rc) {
            std::cerr << "ERROR; return code from pthread_create() is " << rc << std::endl;
            exit(-1);
        }
    }

    // Wait for all threads to complete
    for (t = 0; t < NUM_THREADS; t++) {
        pthread_join(threads[t], NULL);
    }

    std::cout << "Final value of shared_variable: " << shared_variable << std::endl;

    // Exit the main thread
    pthread_exit(NULL);
}
