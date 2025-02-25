
#include <zmq.hpp>
#include <iostream>
#include <unistd.h>  // For sleep function

int main() {
    // Initialize context (needed for ZeroMQ)
    zmq::context_t context(1);

    // Create a PUSH socket (Publisher)
    zmq::socket_t publisher(context, zmq::socket_type::pub);

    // Bind to TCP address (e.g., 127.0.0.1:5555)
    publisher.bind("tcp://127.0.0.1:5555");

    std::string message = "Hello, ZeroMQ!";

    while (true) {
        // Send message to the subscriber
        zmq::message_t zmq_msg(message.size());
        memcpy(zmq_msg.data(), message.c_str(), message.size());

        // Send the message with zmq::send_flags::none
        publisher.send(zmq_msg, zmq::send_flags::none);

        std::cout << "Sent: " << message << std::endl;

        // Wait a bit before sending the next message
        sleep(1);  // Sleep for 1 second
    }

    return 0;
}

