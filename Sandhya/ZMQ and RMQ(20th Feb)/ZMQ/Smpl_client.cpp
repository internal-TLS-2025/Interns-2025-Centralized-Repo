#include <zmq.hpp>
#include <iostream>
using namespace std;

int main() {
    // Create a context for the client to communicate
    zmq::context_t context(1);

    // Create a REQ (Request) socket for the client
    zmq::socket_t socket(context, ZMQ_REQ);

    // Connect the client socket to the server
    socket.connect("tcp://10.113.53.85:5555");

    // Create a request message to send to the server
    string request = "Hello from client!";
    zmq::message_t request_msg(request.data(), request.size());

    // Send the request to the server
    socket.send(request_msg, zmq::send_flags::none);

    // Create a message to receive the reply from the server
    zmq::message_t reply;

    // Receive the reply from the server and check the result
    zmq::recv_result_t result = socket.recv(reply, zmq::recv_flags::none);

    // Handle the result of the receive operation
    if (!result) {
        cerr << "Error receiving reply: " << zmq_strerror(zmq_errno()) << endl;
        return 1;  // Exit with an error code
    }

    // Print the server's reply
    cout << "Received reply: " << reply.to_string() << endl;

    return 0;
}
