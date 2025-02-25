#include <zmq.hpp>
#include <iostream>

int main() {
    // Initialize context (needed for ZeroMQ)
    zmq::context_t context(1);

    // Create a SUB socket (Subscriber)
    zmq::socket_t subscriber(context, zmq::socket_type::sub);

    // Connect to the publisher's address
    subscriber.connect("tcp://127.0.0.1:5555");

    // Subscribe to all messages (using new set() method)
    subscriber.set(zmq::sockopt::subscribe, "");

    while (true) {
        // Receive message from the publisher (using new recv() method with flags)
        zmq::message_t message;

        // Check the return value to ensure the message was received successfully
        zmq::recv_result_t result = subscriber.recv(message, zmq::recv_flags::none);

        if (result) {
            // Message received successfully
            std::string msg_str(static_cast<char*>(message.data()), message.size());
            std::cout << "Received: " << msg_str << std::endl;
        } else {
            // Failed to receive message, handle error here (optional)
            std::cerr << "Failed to receive message" << std::endl;
        }
    }

    return 0;
}

