#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <chrono>

using namespace std;

// Global shared resource
int counter = 0;
std::mutex counterMutex; // Mutex for synchronizing access to counter

// Function that increments the counter
void incrementCounter(int threadID) {
    for (int i = 0; i < 5; ++i) {
        // Lock guard for mutex for safe access to the counter
        {
            std::lock_guard<std::mutex> lock(counterMutex);
            ++counter; // Increment the counter
            cout << "Thread " << threadID << " incremented counter to " << counter << endl;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Simulate some processing time
    }
}

int main() {
    const int numThreads = 5;
    std::vector<std::thread> threads;

    // Create and launch threads
    for (int i = 0; i < numThreads; ++i) {
        threads.emplace_back(incrementCounter, i + 1);
    }

    // Joining all threads
    for (auto& t : threads) {
        if (t.joinable()) {
            t.join(); // Wait for the thread to finish
        }
    }

    cout << "Final counter value: " << counter << endl;

    return 0;
}
