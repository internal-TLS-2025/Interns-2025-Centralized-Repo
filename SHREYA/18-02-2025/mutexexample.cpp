#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <atomic>

// A simple singleton instance
class Singleton {
private:
    static Singleton* instance;
    static std::mutex mutex_instance;

    // Private constructor to prevent direct instantiation
    Singleton() {
        std::cout << "Singleton instance created!" << std::endl;
    }

public:
    // Deleted copy constructor and assignment operator to avoid copies
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

    // Function to get the unique instance
    static Singleton* getInstance() {
        std::lock_guard<std::mutex> lock(mutex_instance);  // Lock to ensure thread safety
        if (instance == nullptr) {
            instance = new Singleton();
        }
        return instance;
    }

    void doWork(int thread_id) {
        std::cout << "Thread " << thread_id << " is working on Singleton instance." << std::endl;
    }
};

// Initialize the static instance pointer to nullptr
Singleton* Singleton::instance = nullptr;
std::mutex Singleton::mutex_instance;

// A shared resource
std::atomic<int> sharedResource(0);

// Mutex to protect shared resource
std::mutex resourceMutex;

// Function to increment shared resource in a thread-safe manner
void incrementResource(int thread_id) {
    std::lock_guard<std::mutex> lock(resourceMutex);  // Ensure exclusive access to shared resource
    ++sharedResource;
    std::cout << "Thread " << thread_id << " incremented shared resource. New value: " << sharedResource << std::endl;
}

// Function that simulates some work with mutex locking
void worker(int thread_id) {
    std::cout << "Thread " << thread_id << " started." << std::endl;

    // Access the Singleton instance
    Singleton* singleton = Singleton::getInstance();
    singleton->doWork(thread_id);

    // Simulate work by incrementing a shared resource
    incrementResource(thread_id);

    std::cout << "Thread " << thread_id << " finished." << std::endl;
}

int main() {
    const int numThreads = 5;

    // Create and launch multiple threads
    std::vector<std::thread> threads;

    for (int i = 0; i < numThreads; ++i) {
        threads.push_back(std::thread(worker, i + 1));
    }

    // Join all threads
    for (auto& t : threads) {
        t.join();
    }

    std::cout << "Final value of shared resource: " << sharedResource << std::endl;

    return 0;
}
