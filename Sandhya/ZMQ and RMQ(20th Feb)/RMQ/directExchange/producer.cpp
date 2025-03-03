#include <iostream>

#include <string>

#include <rabbitmq-c/amqp.h>

#include <rabbitmq-c/tcp_socket.h>
 
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
 
    // Declare a durable queue

    amqp_queue_declare(conn, 1, amqp_cstring_bytes("persistent_queue"), 0, 1, 0, 0, amqp_empty_table);

    reply = amqp_get_rpc_reply(conn);

    if (reply.reply_type != AMQP_RESPONSE_NORMAL) {

        std::cerr << "Error declaring queue." << std::endl;

        return 1;

    }
 
    std::string message;

    std::cout << "Enter messages to send (type 'exit' to quit):" << std::endl;
 
    while (true) {

        std::cout << "> ";

        std::getline(std::cin, message);
 
        if (message == "exit") break;
 
        amqp_basic_properties_t props;

        props._flags = AMQP_BASIC_CONTENT_TYPE_FLAG | AMQP_BASIC_DELIVERY_MODE_FLAG;

        props.content_type = amqp_cstring_bytes("text/plain");

        props.delivery_mode = 2; // Persistent message
 
        // Publish message to the queue

        amqp_basic_publish(conn, 1, amqp_empty_bytes, amqp_cstring_bytes("persistent_queue"),

                           0, 0, &props, amqp_cstring_bytes(message.c_str()));
 
        std::cout << "Message sent: " << message << std::endl;

    }
 
    // Cleanup and close connection

    amqp_channel_close(conn, 1, AMQP_REPLY_SUCCESS);

    amqp_connection_close(conn, AMQP_REPLY_SUCCESS);

    amqp_destroy_connection(conn);
 
    return 0;

}

 