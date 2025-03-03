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

    std::cout << "Producer started. Type messages:\n";

    std::string message;
    while (true) {
        std::getline(std::cin, message);
        if (message == "exit") break; // Type 'exit' to stop producer

        // Publish message to "logs" exchange
        amqp_basic_publish(conn, 1, amqp_cstring_bytes("logs"), amqp_empty_bytes,
                           0, 0, NULL, amqp_cstring_bytes(message.c_str()));

        std::cout << "Sent: " << message << std::endl;
    }

    // Clean up
    amqp_channel_close(conn, 1, AMQP_REPLY_SUCCESS);
    amqp_connection_close(conn, AMQP_REPLY_SUCCESS);
    amqp_destroy_connection(conn);

    return 0;
}

