#include <pthread.h>
#include <iostream>

#define NUM_THREADS 4

using namespace std;

// main function to call from each thread
// void* meaning returns a generic pointer to anything that can be typecasted
void *print_hello_world(void *threadid) { // get as input a long int signifying thread id 
    long tid = (long)threadid; // cast into tid 
    cout << "Hello World from thread #" << tid << endl; 
    pthread_t thread_id_within = pthread_self();
    cout << "Internal thread ID = " << thread_id_within << " from thread with arg " << tid << endl;
    pthread_exit(NULL); // exit the thread
}

int main() {
    pthread_t threads[NUM_THREADS]; //defining NUM_THREADS threads
    int rc; // return code
    long t; // thread number
    for(t = 0; t < NUM_THREADS; t++) {
        cout << "In main: creating thread " << t << endl;
        rc = pthread_create(&threads[t], // pass &threads i.e address of t'th thread
                            NULL, // attributes for new thread
                            print_hello_world, // function that thread executes
                            (void *)t); //argument passed to new thread 
        if (rc) {
            cerr << "ERROR; return code from pthread_create() is " << rc << endl;
            exit(-1);
        }
    }

    // Wait for all threads to complete
    for(t = 0; t < NUM_THREADS; t++) {
        pthread_join(threads[t], NULL); //wait for each thread to complete!     
    }

    // Exit the main thread
    pthread_exit(NULL);
}
