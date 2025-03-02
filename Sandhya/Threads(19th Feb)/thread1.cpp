#include <iostream>
#include <thread>
#include<chrono>

using namespace std;

// A simple function to be run on a thread
void printNumbers(int n) {
    for (int i = 1; i <= n; ++i) {
        std::cout << "Number: " << i << std::endl;
        std::this_thread::sleep_for(chrono::seconds(5));
    }
}

int main() {
    // Create a thread that runs the printNumbers function
    std::thread myThread(printNumbers, 5);

    // Main thread can perform its own task
    for (int i = 1; i <= 3; ++i) {
        std::cout << "Main Thread: " << i << std::endl;
    }

    // Wait for the myThread to finish
    myThread.join();

    std::cout << "Thread has finished execution." << std::endl;
    return 0;
}
