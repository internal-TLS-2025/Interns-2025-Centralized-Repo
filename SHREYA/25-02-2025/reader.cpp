#include <iostream>
#include <fstream>
#include <unistd.h>
#include <string>

int main() {
    const char* pipeName = "./my_named_pipe";  // Path relative to current directory
    std::ifstream pipeStream;

    // Open the named pipe for reading (this will block until there is data to read)
    std::cout << "Opening pipe for reading: " << pipeName << std::endl;
    pipeStream.open(pipeName);

    if (!pipeStream) {
        std::cerr << "Failed to open the pipe for reading." << std::endl;
        return 1;
    }

    std::string message;
    std::getline(pipeStream, message);  // Reading the data from the pipe

    std::cout << "Received message: " << message << std::endl;

    // Close the pipe
    pipeStream.close();

    return 0;
}

