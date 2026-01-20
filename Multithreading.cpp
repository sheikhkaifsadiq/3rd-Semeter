#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <chrono> // For simulated delays

using namespace std;

// Shared resources
int sharedCounter = 0;
mutex counterMutex; // Protects access to sharedCounter

// Function to be executed by threads
// Uses manual lock() and unlock()
void workerFunction(int id, int iterations) {
    for (int i = 0; i < iterations; ++i) {
        // 1. Lock the mutex before accessing shared data
        counterMutex.lock();
        
        // --- Critical Section Start ---
        int temp = sharedCounter;
        // Simulate a tiny processing delay to make race conditions more likely if mutex wasn't here
        this_thread::sleep_for(chrono::milliseconds(1)); 
        temp++;
        sharedCounter = temp;
        
        cout << "Thread " << id << " incremented counter to " << sharedCounter << endl;
        // --- Critical Section End ---

        // 2. Unlock the mutex immediately after
        counterMutex.unlock();
        
        // Sleep to let other threads run
        this_thread::sleep_for(chrono::milliseconds(10));
    }
}

// Function demonstrating std::lock_guard (RAII Style - Safer)
// lock_guard automatically locks on creation and unlocks when it goes out of scope
void safePrint(string message) {
    lock_guard<mutex> lock(counterMutex); // Locks 'counterMutex' now
    
    // Critical section is just this print statement
    cout << "[Main Thread Info]: " << message << endl;
    
} // 'lock' is destroyed here, automatically releasing the mutex

int main() {
    cout << "=== C++ Multithreading & Mutex Demo ===" << endl;

    const int numThreads = 5;
    const int iterationsPerThread = 5;
    vector<thread> threads;

    cout << "Launching " << numThreads << " threads..." << endl;
    cout << "Each thread will increment the counter " << iterationsPerThread << " times.\n" << endl;

    // 1. Create and Launch Threads
    for (int i = 0; i < numThreads; ++i) {
        // std::thread(function_name, args...)
        threads.push_back(thread(workerFunction, i + 1, iterationsPerThread));
    }

    // 2. Do some work in main thread while others run
    safePrint("Workers are running...");
    
    // 3. Wait for all threads to finish (Join)
    // If we don't join, the main program might exit before threads finish, causing a crash.
    for (auto& t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }

    cout << "\nAll threads finished execution." << endl;
    cout << "Final Counter Value: " << sharedCounter << endl;
    cout << "Expected Counter Value: " << (numThreads * iterationsPerThread) << endl;

    if (sharedCounter == (numThreads * iterationsPerThread)) {
        cout << "Result: SUCCESS (Thread-safe)" << endl;
    } else {
        cout << "Result: FAILURE (Race Condition Detected)" << endl;
    }

    return 0;
}
