#include <zmq.hpp>
#include <iostream>
using namespace std;

int main() {
    // Create a context to handle the socket
    zmq::context_t context(1);

    // Create a REP (Reply) socket for the server
    zmq::socket_t socket(context, ZMQ_REP);

    // Bind the socket to an endpoint (in this case, "tcp://*:5555")
    socket.bind("tcp://10.113.53.85:5555");

    while (true) {
        // Create a message to receive a request from the client
        zmq::message_t request;

        // Receive the request and check the result
        zmq::recv_result_t result = socket.recv(request, zmq::recv_flags::none);

        // Handle the result of the receive operation
        if (!result) {
            cerr << "Error receiving message: " << zmq_strerror(zmq_errno()) << endl;
            continue;  // Skip the current iteration and continue listening for new requests
        }

        // Print the request received
        cout << "Received request: " << request.to_string() << endl;

        // Send a reply to the client
        string reply = "Hello from server!";
        zmq::message_t reply_msg(reply.data(), reply.size());
        socket.send(reply_msg, zmq::send_flags::none);
    }

    return 0;
}
