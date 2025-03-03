#include <iostream>
#include <rabbitmq-c/amqp.h>
#include <rabbitmq-c/tcp_socket.h>
#include <cstring>

void die_on_amqp_error(amqp_rpc_reply_t reply, const char* message) {
    if (reply.reply_type != AMQP_RESPONSE_NORMAL) {
        std::cerr << message << std::endl;
        exit(1);
    }
}

int main() {
    // Create a connection state
    amqp_connection_state_t conn = amqp_new_connection();
    amqp_socket_t *socket = amqp_tcp_socket_new(conn);

    if (!socket) {
        std::cerr << "Error creating socket." << std::endl;
        return 1;
    }

    // Open connection to RabbitMQ server (localhost:5672)
    if (amqp_socket_open(socket, "localhost", 5672)) {
        std::cerr << "Error opening socket." << std::endl;
        return 1;
    }

    // Login to RabbitMQ (default: guest/guest)
    die_on_amqp_error(amqp_login(conn, "/", 0, 131072, 0, AMQP_SASL_METHOD_PLAIN, "guest", "guest"), "Login failed");

    // Open a channel
    amqp_channel_open(conn, 1);
    die_on_amqp_error(amqp_get_rpc_reply(conn), "Error opening channel");

    // Declare a fanout exchange named "logs"
    amqp_exchange_declare(conn, 1, amqp_cstring_bytes("logs"), amqp_cstring_bytes("fanout"),
                          0, 1, 0, 0, amqp_empty_table);
    die_on_amqp_error(amqp_get_rpc_reply(conn), "Error declaring exchange");

    // Declare a temporary queue (auto-delete when consumer disconnects)
    amqp_queue_declare_ok_t *queue = amqp_queue_declare(conn, 1, amqp_empty_bytes, 0, 0, 1, 0, amqp_empty_table);
    die_on_amqp_error(amqp_get_rpc_reply(conn), "Error declaring queue");

    amqp_bytes_t queue_name = amqp_bytes_malloc_dup(queue->queue);

    // Bind queue to the fanout exchange "logs"
    amqp_queue_bind(conn, 1, queue_name, amqp_cstring_bytes("logs"), amqp_empty_bytes, amqp_empty_table);
    die_on_amqp_error(amqp_get_rpc_reply(conn), "Error binding queue");

    // Consume messages from the queue
    amqp_basic_consume(conn, 1, queue_name, amqp_empty_bytes, 0, 1, 0, amqp_empty_table);
    die_on_amqp_error(amqp_get_rpc_reply(conn), "Error starting consumer");

    std::cout << "Consumer started. Waiting for messages...\n";

    while (true) {
        amqp_rpc_reply_t res;
        amqp_envelope_t envelope;

        res = amqp_consume_message(conn, &envelope, NULL, 0);
        if (res.reply_type == AMQP_RESPONSE_NORMAL) {
            std::string message((char*)envelope.message.body.bytes, envelope.message.body.len);
            std::cout << "Received: " << message << std::endl;

            amqp_destroy_envelope(&envelope);
        } else {
            std::cerr << "Error receiving message." << std::endl;
            break;
        }
    }

    // Clean up
    amqp_channel_close(conn, 1, AMQP_REPLY_SUCCESS);
    amqp_connection_close(conn, AMQP_REPLY_SUCCESS);
    amqp_destroy_connection(conn);

    return 0;
}

