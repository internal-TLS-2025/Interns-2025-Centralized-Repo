#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>

void consumer()
{
    std::string message;

    // Wait for the producer to write the message to the file
    while (true)
    {
        std::ifstream inFile("message.txt");
        if (inFile.is_open())
        {
            // Read the message from the file
            std::getline(inFile, message);
            inFile.close();

            // If a message is found, consume it
            if (!message.empty())
            {
                std::cout << "Consumed: " << message << std::endl;
                break;  // Exit after consuming the message
            }
        }
        else
        {
            std::cerr << "Error: Could not open file for reading!" << std::endl;
        }

        // Wait for a short period before checking again
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

int main()
{
    // Create and start the consumer thread
    std::thread consumerThread(consumer);

    // Wait for the consumer thread to finish
    consumerThread.join();

    return 0;
}
