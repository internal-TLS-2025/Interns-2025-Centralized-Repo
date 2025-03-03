#include <iostream>
#include <rabbitmq-c/amqp.h>
#include <rabbitmq-c/tcp_socket.h>

int main() {
    // Create a connection state to RabbitMQ
    amqp_connection_state_t conn;
    conn = amqp_new_connection();

    // Create a TCP socket to communicate with RabbitMQ
    amqp_socket_t *socket = amqp_tcp_socket_new(conn);
    if (!socket) {
        std::cerr << "Error creating socket." << std::endl;
        return 1;
    }

    // Open the socket to RabbitMQ (default: localhost:5672)
    int status = amqp_socket_open(socket, "localhost", 5672);
    if (status) {
        std::cerr << "Error opening socket." << std::endl;
        return 1;
    }

    // Log in to RabbitMQ server (default username/password: guest/guest)
    amqp_rpc_reply_t reply = amqp_login(conn, "/", 0, 131072, 0, AMQP_SASL_METHOD_PLAIN, "guest", "guest");
    if (reply.reply_type != AMQP_RESPONSE_NORMAL) {
        std::cerr << "Login failed." << std::endl;
        return 1;
    }

    // Open a channel
    amqp_channel_open(conn, 1);
    reply = amqp_get_rpc_reply(conn);
    if (reply.reply_type != AMQP_RESPONSE_NORMAL) {
        std::cerr << "Error opening channel." << std::endl;
        return 1;
    }

    // Declare a Fanout Exchange
    amqp_table_t arguments = amqp_empty_table;
    amqp_exchange_declare(conn, 1, amqp_cstring_bytes("logs"), amqp_cstring_bytes("fanout"),
                          0, 1, 0, 0, arguments); // Fanout exchange
    reply = amqp_get_rpc_reply(conn);
    if (reply.reply_type != AMQP_RESPONSE_NORMAL) {
        std::cerr << "Error declaring exchange." << std::endl;
        return 1;
    }

    // Declare a queue
    amqp_queue_declare(conn, 1, amqp_cstring_bytes("info_queue"), 0, 1, 0, 1, amqp_empty_table);
    reply = amqp_get_rpc_reply(conn);
    if (reply.reply_type != AMQP_RESPONSE_NORMAL) {
        std::cerr << "Error declaring queue." << std::endl;
        return 1;
    }

    // Bind the queue to the fanout exchange
    amqp_queue_bind(conn, 1, amqp_cstring_bytes("info_queue"), amqp_cstring_bytes("logs"),
                    amqp_empty_bytes, amqp_empty_table); // No routing key for fanout
    reply = amqp_get_rpc_reply(conn);
    if (reply.reply_type != AMQP_RESPONSE_NORMAL) {
        std::cerr << "Error binding queue." << std::endl;
        return 1;
    }

    // Start consuming messages from the queue
    amqp_basic_consume(conn, 1, amqp_cstring_bytes("info_queue"), amqp_empty_bytes, 0, 1, 0, amqp_empty_table);
    reply = amqp_get_rpc_reply(conn);
    if (reply.reply_type != AMQP_RESPONSE_NORMAL) {
        std::cerr << "Error starting consumer." << std::endl;
        return 1;
    }

    std::cout << "Waiting for messages..." << std::endl;

    // Consume messages
    while (true) {
        amqp_rpc_reply_t get_reply;
        amqp_envelope_t envelope;

        // Get the next message
        get_reply = amqp_consume_message(conn, &envelope, NULL, 0);

        if (get_reply.reply_type == AMQP_RESPONSE_NORMAL) {
            // Print the message content
            std::cout << "Received message: " << std::string((char *)envelope.message.body.bytes, envelope.message.body.len) << std::endl;

            // Acknowledge the message
            amqp_basic_ack(conn, 1, envelope.delivery_tag, 0);
        } else {
            std::cerr << "Error receiving message." << std::endl;
            break;
        }

        // Envelope cleanup
        amqp_destroy_envelope(&envelope);
    }

    // Close the channel and connection
    amqp_channel_close(conn, 1, AMQP_REPLY_SUCCESS);
    amqp_connection_close(conn, AMQP_REPLY_SUCCESS);
    amqp_destroy_connection(conn);

    return 0;
}
