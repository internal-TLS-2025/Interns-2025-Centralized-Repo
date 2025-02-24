 ZeroMQ is a messaging library that simplifies distributed communication between applications. I chose it over plain TCP because it makes your code more reliable, easier to maintain, and faster to develop. By using ZeroMQ’s high-level REQ-REP pattern, we were able to create a robust client-server communication system with minimal effort, while handling things like error checking, retries, and message handling automatically
Server:

Listens for requests from clients, processes them, and sends back replies.
It runs in an infinite loop to continuously handle incoming requests.

Client:

Sends a request message to the server, waits for a reply, and prints the server's response.
It connects to the server, sends a request, and handles the reply in a straightforward way.
Both programs use ZeroMQ's REQ and REP sockets, where the client sends a request (REQ) and the server replies (REP). The recv() and send() functions are used to exchange messages between the client and server.
