#include <iostream>
#include <chrono>
#include <thread>
#include <rabbitmq-c/amqp.h>
#include <rabbitmq-c/tcp_socket.h>

int main() {
    // Create a connection state to RabbitMQ
    amqp_connection_state_t conn = amqp_new_connection();

    // Create a TCP socket
    amqp_socket_t *socket = amqp_tcp_socket_new(conn);
    if (!socket) {
        std::cerr << "Error creating socket." << std::endl;
        return 1;
    }

    // Open the socket
    int status = amqp_socket_open(socket, "localhost", 5672);
    if (status) {
        std::cerr << "Error opening socket." << std::endl;
        return 1;
    }

    // Log in with a heartbeat to prevent disconnection
    amqp_rpc_reply_t reply = amqp_login(conn, "/", 0, 131072, 60, AMQP_SASL_METHOD_PLAIN, "guest", "guest");
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

    // Declare a queue (ensure it's already created by the producer)
    amqp_queue_declare(conn, 1, amqp_cstring_bytes("info_queue"), 0, 1, 0, 1, amqp_empty_table);
    reply = amqp_get_rpc_reply(conn);
    if (reply.reply_type != AMQP_RESPONSE_NORMAL) {
        std::cerr << "Error declaring queue." << std::endl;
        return 1;
    }

    // Start consuming messages
    amqp_basic_consume(conn, 1, amqp_cstring_bytes("info_queue"), amqp_empty_bytes, 0, 1, 0, amqp_empty_table);
    reply = amqp_get_rpc_reply(conn);
    if (reply.reply_type != AMQP_RESPONSE_NORMAL) {
        std::cerr << "Error starting consumer." << std::endl;
        return 1;
    }

    std::cout << "Waiting for messages..." << std::endl;

    // Message consumption loop
    while (true) {
        amqp_rpc_reply_t get_reply;
        amqp_envelope_t envelope;

        // Try consuming a message
        get_reply = amqp_consume_message(conn, &envelope, NULL, 0);

        if (get_reply.reply_type == AMQP_RESPONSE_NORMAL) {
            // Successfully received a message
            std::cout << "Received message: " 
                      << std::string((char *)envelope.message.body.bytes, envelope.message.body.len) 
                      << std::endl;

            // Acknowledge the message
            amqp_basic_ack(conn, 1, envelope.delivery_tag, 0);
            amqp_destroy_envelope(&envelope);
        } else {
            std::cerr << "No messages available. Retrying in 1 second..." << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));  // Wait before retrying
        }
    }

    // Close connection (this part won't be reached due to the infinite loop, but kept for completeness)
    amqp_channel_close(conn, 1, AMQP_REPLY_SUCCESS);
    amqp_connection_close(conn, AMQP_REPLY_SUCCESS);
    amqp_destroy_connection(conn);

    return 0;
}

