#include <zmq.hpp>
#include <string>
#include <iostream>

int main() {
    // Create a ZeroMQ context
    zmq::context_t context(1);

    // Create a PULL socket
    zmq::socket_t pull_socket(context, ZMQ_PULL);
    pull_socket.connect("tcp://localhost:5555"); // Connect to the PUSH socket

    std::cout << "Receiver is ready, waiting for messages..." << std::endl;

    while (true) {
        zmq::message_t zmq_message;
        auto result = pull_socket.recv(zmq_message, zmq::recv_flags::none);

        // Check if the message was received successfully
        if (result) {
            std::string received_message(static_cast<char*>(zmq_message.data()), zmq_message.size());
            std::cout << "Received: " << received_message << std::endl;
        } else {
            std::cerr << "Error receiving message." << std::endl;
        }
    }

    return 0;
}
