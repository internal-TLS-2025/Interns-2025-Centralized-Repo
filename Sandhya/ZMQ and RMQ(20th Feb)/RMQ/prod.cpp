#include <iostream>
#include <rabbitmq-c/amqp.h>
#include <rabbitmq-c/tcp_socket.h>
 
void publish_message(const std::string &message) {
    amqp_connection_state_t conn = amqp_new_connection();
    amqp_socket_t *socket = amqp_tcp_socket_new(conn);
    
    if (amqp_socket_open(socket, "localhost", 5672)) {
        std::cerr << "Failed to open RabbitMQ connection!" << std::endl;
        return;
    }
 
    amqp_login(conn, "/", 0, 131072, 0, AMQP_SASL_METHOD_PLAIN, "guest", "guest");
    amqp_channel_open(conn, 1);
    amqp_get_rpc_reply(conn);
 
    amqp_bytes_t queue_name = amqp_cstring_bytes("task_queue");
 
    amqp_basic_properties_t props;
    props._flags = AMQP_BASIC_DELIVERY_MODE_FLAG;
    props.delivery_mode = 2; // Persistent message
 
    amqp_basic_publish(
        conn, 1, amqp_empty_bytes, queue_name, 0, 0,
        &props, amqp_cstring_bytes(message.c_str()));
 
    std::cout << " [x] Sent '" << message << "'" << std::endl;
 
    amqp_channel_close(conn, 1, AMQP_REPLY_SUCCESS);
    amqp_connection_close(conn, AMQP_REPLY_SUCCESS);
    amqp_destroy_connection(conn);
}
 
int main() {
    std::string message = "Hello, RabbitMQ!";
    publish_message(message);
    return 0;
}
