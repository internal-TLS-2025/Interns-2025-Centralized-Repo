#include <iostream>
#include <amqpcpp.h>
#include <amqpcpp/linux_tcp.h>

int main() {
    // Create the connection and channel to RabbitMQ server
    AMQP::TcpConnection connection(&handler, AMQP::Address("amqp://localhost"));
    AMQP::TcpChannel channel(&connection);

    // Declare the same queue "task_queue"
    channel.declareQueue("task_queue", AMQP::durable);

    // Define a consumer that will consume messages from the "task_queue"
    channel.consume("task_queue").onMessage([](const AMQP::Message &message, uint64_t deliveryTag, bool redelivered) {
        std::cout << "Received: " << message.message() << std::endl;
    });

    // Keep the consumer running to listen for messages
    while (true) {
        connection.process();
    }

    return 0;
}
