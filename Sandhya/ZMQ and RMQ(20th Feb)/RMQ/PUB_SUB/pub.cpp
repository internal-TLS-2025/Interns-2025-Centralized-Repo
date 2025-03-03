#include <iostream>
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

    // Open the socket to RabbitMQ server (localhost:5672)
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

    // Declare a fanout exchange (this will broadcast messages to all bound queues)
    amqp_exchange_declare(conn, 1, amqp_cstring_bytes("logs"), amqp_cstring_bytes("fanout"),
                          0, 1, 0, 0, amqp_empty_table);
    reply = amqp_get_rpc_reply(conn);
    if (reply.reply_type != AMQP_RESPONSE_NORMAL) {
        std::cerr << "Error declaring exchange." << std::endl;
        return 1;
    }

    // Publish a message to the exchange
    std::string message = "Hello, this is a Pub/Sub message!";
    amqp_basic_publish(conn, 1, amqp_cstring_bytes("logs"), amqp_empty_bytes, 0, 0, NULL, amqp_cstring_bytes(message.c_str()));
    reply = amqp_get_rpc_reply(conn);
    if (reply.reply_type != AMQP_RESPONSE_NORMAL) {
        std::cerr << "Error publishing message." << std::endl;
        return 1;
    }

    std::cout << "Message sent: " << message << std::endl;

    // Close the channel and connection
    amqp_channel_close(conn, 1, AMQP_REPLY_SUCCESS);
    amqp_connection_close(conn, AMQP_REPLY_SUCCESS);
    amqp_destroy_connection(conn);

    return 0;
}
