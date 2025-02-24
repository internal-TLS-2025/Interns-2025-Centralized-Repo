import pika

connection = pika.BlockingConnection(pika.ConnectionParameters('localhost'))
channel = connection.channel()
channel.queue_declare(queue='task_queue', durable=True)

for i in range(10):
	message = f"Message {i}"

	channel.basic_publish(exchange='', routing_key = 'task_queue', body = message,
	properties = pika.BasicProperties(delivery_mode = 2))

	print (f"Sent: {message}")

connection.close()
