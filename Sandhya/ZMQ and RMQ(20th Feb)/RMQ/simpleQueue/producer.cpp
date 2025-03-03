#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>

void producer()
{
    // Simulate some work before producing the message
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // Message to be sent to the consumer
    std::string message = "Hello from Producer";

    // Write the message to a file
    std::ofstream outFile("message.txt");
    if (outFile.is_open())
    {
        outFile << message << std::endl;
        std::cout << "Produced: " << message << std::endl;
        outFile.close();
    }
    else
    {
        std::cerr << "Error: Could not open file for writing!" << std::endl;
    }
}

int main()
{
    // Create and start the producer thread
    std::thread producerThread(producer);

    // Wait for the producer thread to finish
    producerThread.join();

    return 0;
}
