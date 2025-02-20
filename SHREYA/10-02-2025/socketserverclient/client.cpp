	#include <iostream>
   #include <string>
   #include <cstring>
   #include <sys/types.h>
   #include <sys/socket.h>
   #include <netinet/in.h>
   #include <arpa/inet.h> // For inet_pton
   #include <unistd.h>

   #define PORT 5555   // Define the port number
   #define BUFFER_SIZE 1024

   int main() {
       int sock = 0;
       struct sockaddr_in serv_addr;
       char buffer[BUFFER_SIZE] = {0};

       // Creating socket
       if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
           std::cerr << "Socket creation error" << std::endl;
           return -1;
       }

       serv_addr.sin_family = AF_INET; // IPv4
       serv_addr.sin_port = htons(PORT); // Convert port number to network byte order

       // Convert IPv4 address from text to binary
       if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
           std::cerr << "Invalid address / Address not supported" << std::endl;
           return -1;
       }

       // Connecting to the server
       if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
           std::cerr << "Connection failed" << std::endl;
           return -1;
       }

       // Send a message to the server
       std::string message = "Hello from client!";
       send(sock, message.c_str(), message.size(), 0);
       std::cout << "Message sent to the server" << std::endl;

       // Read the server's response
       int valread = read(sock, buffer, BUFFER_SIZE);
       std::cout << "Message from server: " << buffer << std::endl;

       // Clean up
       close(sock);
       
       return 0;
   }
