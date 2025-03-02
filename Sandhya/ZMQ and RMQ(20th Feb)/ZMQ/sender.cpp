#include <zmq.hpp>
#include <string>
#include <iostream>
#include <thread>
#include <chrono>

int main() {
    // Create a ZeroMQ context
    zmq::context_t context(1);

    // Create a PUSH socket
    zmq::socket_t push_socket(context, ZMQ_PUSH);
    push_socket.bind("tcp://*:5555"); // Bind to a TCP socket

    std::cout << "Sender is ready, sending messages..." << std::endl;

    // Sending multiple messages
    for (int i = 0; i < 10; ++i) {
        std::string message = "Message " + std::to_string(i);
        zmq::message_t zmq_message(message.data(), message.size());
        push_socket.send(zmq_message, zmq::send_flags::none);

        std::cout << "Sent: " << message << std::endl;
        
        // Sleep for a bit to simulate work
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}
