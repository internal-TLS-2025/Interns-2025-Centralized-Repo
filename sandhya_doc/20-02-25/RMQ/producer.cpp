#include <iostream>
#include <amqpcp.h>
#include <amqpcpp/linux_tcp.h>

int main() {
    // Create the connection and channel to RabbitMQ server
    AMQP::TcpConnection connection(&handler, AMQP::Address("amqp://localhost"));
    AMQP::TcpChannel channel(&connection);

    // Declare a queue called "task_queue"
    channel.declareQueue("task_queue", AMQP::durable);

    // Send a message to the "task_queue"
    std::string message = "Hello, RabbitMQ!";
    channel.publish("", "task_queue", message);

    std::cout << "Sent: " << message << std::endl;

    return 0;
}
