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

    // Get the message from the user
    std::string message;
    std::cout << "Enter the message to send: ";
    std::getline(std::cin, message); // Get a full line of input (including spaces)

    // Publish a message (no routing key for fanout)
    amqp_basic_publish(conn, 1, amqp_cstring_bytes("logs"), amqp_empty_bytes, 0, 0, NULL, amqp_cstring_bytes(message.c_str()));
    reply = amqp_get_rpc_reply(conn);
    if (reply.reply_type != AMQP_RESPONSE_NORMAL) {
        std::cerr << "Error publishing message." << std::endl;
        return 1;
    }

    std::cout << "Message published successfully!" << std::endl;

    // Close the channel and connection
    amqp_channel_close(conn, 1, AMQP_REPLY_SUCCESS);
    amqp_connection_close(conn, AMQP_REPLY_SUCCESS);
    amqp_destroy_connection(conn);

    return 0;
}
