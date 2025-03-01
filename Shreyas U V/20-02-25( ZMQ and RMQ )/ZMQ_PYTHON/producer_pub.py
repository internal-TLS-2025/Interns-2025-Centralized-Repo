import zmq
import time

context = zmq.Context()
socket = context.socket(zmq.PUB)
socket.bind("tcp://127.0.0.1:5556")

print("Publisher started...")

try:
    time.sleep(1)  # Allow subscribers to connect
    while True:
        socket.send_string("topic1 Hello Subscribers!")
        print("Sent: topic1 Hello Subscribers!")
        time.sleep(1)  # Prevent high CPU usage
except KeyboardInterrupt:
    print("\nPublisher shutting down...")
finally:
    socket.close()
    context.term()

