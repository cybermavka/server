#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

const int PORT = 8080;

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    std::string response = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello World!";

    // Create a socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        std::cerr << "socket failed" << std::endl;
        return 1;
    }

    // Set socket options
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        std::cerr << "setsockopt failed" << std::endl;
        return 1;
    }

    // Bind the socket to a port
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        std::cerr << "bind failed" << std::endl;
        return 1;
    }

    // Start listening for connections
    if (listen(server_fd, 3) < 0) {
        std::cerr << "listen failed" << std::endl;
        return 1;
    }

    std::cout << "Server listening on port " << PORT << std::endl;

    // Accept incoming connections
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        std::cerr << "accept failed" << std::endl;
        return 1;
    }

    // Read data from the client
    int num_bytes = read(new_socket, buffer, 1024);
    std::cout << "Received " << num_bytes << " bytes: " << buffer << std::endl;

    // Send a response to the client
    send(new_socket, response.c_str(), response.size(), 0);
    std::cout << "Response sent: " << response << std::endl;

    // Clean up
    close(new_socket);
    close(server_fd);

    return 0;
}
