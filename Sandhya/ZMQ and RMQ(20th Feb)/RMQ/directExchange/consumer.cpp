#include <iostream>

#include <rabbitmq-c/amqp.h>

#include <rabbitmq-c/tcp_socket.h>

#include <chrono>

#include <thread>
 
int main() {

    // Create a connection to RabbitMQ

    amqp_connection_state_t conn = amqp_new_connection();

    amqp_socket_t *socket = amqp_tcp_socket_new(conn);
 
    if (!socket) {

        std::cerr << "Error creating socket." << std::endl;

        return 1;

    }
 
    if (amqp_socket_open(socket, "localhost", 5672)) {

        std::cerr << "Error opening socket. Is RabbitMQ running?" << std::endl;

        return 1;

    }
 
    // Login to RabbitMQ

    amqp_rpc_reply_t reply = amqp_login(conn, "/", 0, 131072, 0, AMQP_SASL_METHOD_PLAIN, "guest", "guest");

    if (reply.reply_type != AMQP_RESPONSE_NORMAL) {

        std::cerr << "Login failed." << std::endl;

        return 1;

    }
 
    amqp_channel_open(conn, 1);

    reply = amqp_get_rpc_reply(conn);

    if (reply.reply_type != AMQP_RESPONSE_NORMAL) {

        std::cerr << "Error opening channel." << std::endl;

        return 1;

    }
 
    // Declare the same durable queue

    amqp_queue_declare(conn, 1, amqp_cstring_bytes("persistent_queue"), 0, 1, 0, 0, amqp_empty_table);

    reply = amqp_get_rpc_reply(conn);

    if (reply.reply_type != AMQP_RESPONSE_NORMAL) {

        std::cerr << "Error declaring queue." << std::endl;

        return 1;

    }
 
    // Start consuming messages (auto-ack disabled)

    amqp_basic_consume(conn, 1, amqp_cstring_bytes("persistent_queue"), amqp_empty_bytes, 0, 0, 0, amqp_empty_table);

    reply = amqp_get_rpc_reply(conn);

    if (reply.reply_type != AMQP_RESPONSE_NORMAL) {

        std::cerr << "Error starting consumer." << std::endl;

        return 1;

    }
 
    std::cout << "Waiting for messages..." << std::endl;
 
    while (true) {

        amqp_rpc_reply_t get_reply;

        amqp_envelope_t envelope;
 
        get_reply = amqp_consume_message(conn, &envelope, NULL, 0);
 
        if (get_reply.reply_type == AMQP_RESPONSE_NORMAL) {

            std::string received_message((char *)envelope.message.body.bytes, envelope.message.body.len);

            std::cout << "Received: " << received_message << std::endl;
 
            // Acknowledge the message (confirm receipt)

            amqp_basic_ack(conn, 1, envelope.delivery_tag, 0);

            amqp_destroy_envelope(&envelope);

        } else {

            std::cerr << "Waiting for new messages..." << std::endl;

            std::this_thread::sleep_for(std::chrono::seconds(1)); // Prevent busy loop

        }

    }
 
    amqp_channel_close(conn, 1, AMQP_REPLY_SUCCESS);

    amqp_connection_close(conn, AMQP_REPLY_SUCCESS);

    amqp_destroy_connection(conn);
 
    return 0;

}

 