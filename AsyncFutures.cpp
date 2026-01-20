#include <iostream>
#include <future> // For std::async, std::future
#include <thread> // For std::this_thread
#include <chrono> // For std::chrono
#include <vector>

using namespace std;

// A function that simulates a complex calculation
// It takes time to complete and returns a result
int complexCalculation(int id, int durationSeconds) {
    cout << "  [Task " << id << "] Started..." << endl;
    
    // Simulate heavy work
    this_thread::sleep_for(chrono::seconds(durationSeconds));
    
    cout << "  [Task " << id << "] Finished." << endl;
    return id * 100; // Return some calculated value
}

int main() {
    cout << "=== C++ Async & Futures Demo ===" << endl;

    // --- 1. Launching a simple asynchronous task ---
    cout << "\n1. Launching Task 1 in the background..." << endl;
    
    // std::async launches the function in a separate thread (using std::launch::async)
    // It returns a std::future<int> which will eventually hold the result
    future<int> result1 = async(launch::async, complexCalculation, 1, 2);

    cout << "   Main thread is free to do other work while Task 1 runs." << endl;
    cout << "   (Doing main thread work...)" << endl;
    this_thread::sleep_for(chrono::milliseconds(500));

    // --- 2. Waiting for the result ---
    cout << "   Waiting for Task 1 result..." << endl;
    
    // .get() blocks the main thread until the result is ready
    int value1 = result1.get(); 
    
    cout << "   Got Result from Task 1: " << value1 << endl;

    // --- 3. Handling multiple futures ---
    cout << "\n2. Launching multiple tasks simultaneously..." << endl;
    
    // Launch 3 tasks at once
    future<int> f1 = async(launch::async, complexCalculation, 10, 1); // 1 sec
    future<int> f2 = async(launch::async, complexCalculation, 20, 3); // 3 sec
    future<int> f3 = async(launch::async, complexCalculation, 30, 2); // 2 sec

    cout << "   All tasks launched. Waiting for them to complete..." << endl;

    // We can retrieve them in any order, but .get() is blocking.
    // Task 1 (1s) will finish first, then Task 3 (2s), then Task 2 (3s).
    // The total wait time will be roughly equal to the longest task (3s), not the sum (6s).
    
    cout << "   Result 10: " << f1.get() << endl;
    cout << "   Result 30: " << f3.get() << endl;
    cout << "   Result 20: " << f2.get() << endl;

    // --- 4. Polling for status (Non-blocking check) ---
    cout << "\n3. Polling future status..." << endl;
    
    future<int> fLong = async(launch::async, complexCalculation, 99, 3);
    
    cout << "   Task 99 started. Checking status every 500ms:" << endl;

    // wait_for checks if the future is ready without blocking indefinitely
    while (fLong.wait_for(chrono::milliseconds(500)) != future_status::ready) {
        cout << "   ... Task 99 is still running ..." << endl;
    }

    cout << "   Task 99 is ready! Result: " << fLong.get() << endl;

    return 0;
}
