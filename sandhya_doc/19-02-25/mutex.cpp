#include <iostream>
#include <thread>
#include <mutex>

using namespace std;

// Create a mutex for synchronous output
mutex outputMutex;

// A simple function to be run on a thread
void printNumbers(int n) {
    for (int i = 1; i <= n; ++i) {
        lock_guard<mutex> guard(outputMutex); // Automatically locks and unlocks
        cout << "Number: " << i << endl;
    }
}

int main() {
    // Create a thread that runs the printNumbers function
    thread myThread(printNumbers, 5);

    // Main thread can perform its own task
    for (int i = 1; i <= 3; ++i) {
        {
            lock_guard<mutex> guard(outputMutex); // Automatically locks and unlocks
            cout << "Main Thread: " << i << endl;
        }
    }

    // Wait for the myThread to finish
    if (myThread.joinable()) {
        myThread.join(); // Ensure the thread is joined safely
    }

    {
        lock_guard<mutex> guard(outputMutex); // Lock before final output
        cout << "Thread has finished execution." << endl;
    }

    return 0;
}
