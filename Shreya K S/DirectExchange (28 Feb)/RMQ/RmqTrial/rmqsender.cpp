#include <iostream>
#include <SimpleAmqpClient/SimpleAmqpClient.h>
#include <amqp.h>
#include <amqp_framing.h>

int main() {
    // Create a connection
    amqp_connection_state_t conn;
    conn = amqp_new_connection();

    // Define the server and connection parameters
    const char *hostname = "localhost";
    int port = 5672;
    amqp_socket_t *socket = amqp_tcp_socket_new(conn);
    if (!socket) {
        std::cerr << "Creating TCP socket failed!" << std::endl;
        return 1;
    }

    // Open the connection to the server
    if (amqp_socket_open(socket, hostname, port)) {
        std::cerr << "Opening TCP socket failed!" << std::endl;
        return 1;
    }

    // Create a channel
    amqp_channel_t channel = 1;
    amqp_basic_properties_t props;
    amqp_basic_properties_clear(&props);

    // Declare an exchange
    const char *exchange = "direct_logs";
    const char *routing_key = "info";

    amqp_exchange_declare(conn, channel, amqp_cstring_bytes(exchange),
                          amqp_cstring_bytes("direct"), 0, 0, 0, 0);

    // Send a message
    const char *message = "Hello, RabbitMQ!";
    amqp_basic_publish(conn, channel, amqp_cstring_bytes(exchange),
                       amqp_cstring_bytes(routing_key), 0, 0, &props,
                       amqp_cstring_bytes(message));

    std::cout << "Message Sent: " << message << std::endl;

    // Close connection
    amqp_channel_close(conn, channel, AMQP_REPLY_SUCCESS);
    amqp_connection_close(conn, AMQP_REPLY_SUCCESS);
    amqp_destroy_connection(conn);

    return 0;
}

