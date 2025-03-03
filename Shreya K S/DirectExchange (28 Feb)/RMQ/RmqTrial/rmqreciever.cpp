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
    const char *queue = "info_queue";

    // Declare a queue
    amqp_queue_declare(conn, channel, amqp_cstring_bytes(queue), 0, 1, 0, 0, amqp_empty_table);

    // Bind the queue to the exchange with the routing key
    amqp_queue_bind(conn, channel, amqp_cstring_bytes(queue),
                    amqp_cstring_bytes("direct_logs"), amqp_cstring_bytes("info"),
                    amqp_empty_table);

    // Start consuming the messages
    amqp_basic_consume(conn, channel, amqp_cstring_bytes(queue), amqp_empty_bytes, 0, 1, 0, amqp_empty_table);

    while (true) {
        amqp_rpc_reply_t res;
        amqp_envelope_t envelope;

        // Receive messages from the queue
        res = amqp_consume_message(conn, &envelope, NULL, 0);
        if (res.reply_type != AMQP_RESPONSE_NORMAL) {
            std::cerr << "Failed to receive a message!" << std::endl;
            break;
        }

        // Print the message content
        std::cout << "Received: " << (char*)envelope.message.body.bytes << std::endl;

        // Acknowledge the message
        amqp_destroy_envelope(&envelope);
    }

    // Close connection
    amqp_channel_close(conn, channel, AMQP_REPLY_SUCCESS);
    amqp_connection_close(conn, AMQP_REPLY_SUCCESS);
    amqp_destroy_connection(conn);

    return 0;
}

