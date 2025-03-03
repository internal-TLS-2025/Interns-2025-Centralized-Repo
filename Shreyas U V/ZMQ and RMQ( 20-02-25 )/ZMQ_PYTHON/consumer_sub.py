import zmq
import time

context = zmq.Context()
socket = context.socket(zmq.SUB)
socket.connect("tcp://127.0.0.1:5556")
socket.setsockopt_string(zmq.SUBSCRIBE, "topic1")  # Subscribe to topic "topic1"

print("Subscriber started...")

time.sleep(1)  # Allow connection setup

try:
    while True:
        message = socket.recv_string()
        print(f"Received: {message}")
except KeyboardInterrupt:
    print("\nSubscriber shutting down...")
finally:
    socket.close()
    context.term()

