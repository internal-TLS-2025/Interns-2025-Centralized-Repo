#include <iostream>
#include <rabbitmq-c/amqp.h>
#include <rabbitmq-c/tcp_socket.h>
 
void consume_message() {
    amqp_connection_state_t conn = amqp_new_connection();
    amqp_socket_t *socket = amqp_tcp_socket_new(conn);
 
    if (amqp_socket_open(socket, "localhost", 5672)) {
        std::cerr << "Failed to open connection to RabbitMQ!" << std::endl;
        return;
    }
 
    amqp_login(conn, "/", 0, 131072, 0, AMQP_SASL_METHOD_PLAIN, "guest", "guest");
    amqp_channel_open(conn, 1);
    amqp_get_rpc_reply(conn);
 
    amqp_bytes_t queue_name = amqp_cstring_bytes("task_queue");
 
    amqp_basic_consume(
        conn, 1, queue_name, amqp_empty_bytes, 0, false, false, amqp_empty_table);
 
    amqp_rpc_reply_t res;
    amqp_envelope_t envelope;
    
    while (true) {
        amqp_maybe_release_buffers(conn);
        res = amqp_consume_message(conn, &envelope, nullptr, 0);
 
        if (res.reply_type == AMQP_RESPONSE_NORMAL) {
            std::string message((char*)envelope.message.body.bytes, envelope.message.body.len);
            std::cout << " [x] Received '" << message << "'" << std::endl;
 
            // Acknowledge the message
            amqp_basic_ack(conn, 1, envelope.delivery_tag, false);
        }
 
        amqp_destroy_envelope(&envelope);
    }
 
    amqp_channel_close(conn, 1, AMQP_REPLY_SUCCESS);
    amqp_connection_close(conn, AMQP_REPLY_SUCCESS);
    amqp_destroy_connection(conn);
}
 
int main() {
    consume_message();
    return 0;
}
 
