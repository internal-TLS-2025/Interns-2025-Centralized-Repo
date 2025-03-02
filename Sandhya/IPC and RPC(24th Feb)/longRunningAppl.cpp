#include <iostream>
#include <thread>
#include <chrono>
#include<unistd.h>
int main() {
    std::cout << "Long-running application started, PID: " << getpid() << std::endl;

    while (true) {
        // Sleep for some time to simulate work
        std::this_thread::sleep_for(std::chrono::seconds(10));
        std::cout << "Still running..." << std::endl;
    }

    return 0;
}
