#include <iostream>
#include <fstream>
#include <unistd.h>

int main() {
    const char* pipeName = "./my_named_pipe";  // Path relative to current directory
    std::ofstream pipeStream;

    // Open the named pipe for writing
    std::cout << "Opening pipe for writing: " << pipeName << std::endl;
    pipeStream.open(pipeName);

    if (!pipeStream) {
        std::cerr << "Failed to open the pipe for writing." << std::endl;
        return 1;
    }

    // Write a message to the pipe
    pipeStream << "Hello from the writer program!" << std::endl;
    std::cout << "Message written to pipe." << std::endl;

    // Close the pipe
    pipeStream.close();

    return 0;
}

