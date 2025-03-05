#!/bin/bash

echo "Starting RabbitMQ Setup..."

# Check if RabbitMQ is running
if ! sudo systemctl is-active --quiet rabbitmq-server; then
    echo "RabbitMQ is not running. Starting it now..."
    sudo systemctl start rabbitmq-server
    sleep 5  # Wait for RabbitMQ to start
fi

# Verify RabbitMQ status
if ! sudo rabbitmqctl status >/dev/null 2>&1; then
    echo "Error: RabbitMQ is not running or not accessible."
    exit 1
fi

# Declare an exchange
rabbitmqadmin declare exchange name=logs type=fanout
echo "Exchange 'logs' declared."

# Declare a queue
rabbitmqadmin declare queue name=my_queue
echo "Queue 'my_queue' declared."

# Bind the queue to the exchange
rabbitmqadmin declare binding source=logs destination=my_queue
echo "Queue 'my_queue' bound to exchange 'logs'."

# Publish test messages
rabbitmqadmin publish exchange=logs routing_key="" payload="Test Message 1"
rabbitmqadmin publish exchange=logs routing_key="" payload="Test Message 2"
echo "Messages published."

# Purge the queue (remove all messages)
sudo rabbitmqctl purge_queue my_queue
echo "Queue 'my_queue' purged."

# Retrieve up to 10 messages from the queue
rabbitmqadmin get queue=my_queue count=10
echo "Retrieved messages from 'my_queue'."

echo "RabbitMQ Setup Completed!"
