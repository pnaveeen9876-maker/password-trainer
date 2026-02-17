#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>

int main() {
    int server_fd, client_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        std::cerr << "Socket failed\n";
        return 1;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        std::cerr << "Bind failed\n";
        return 1;
    }

    listen(server_fd, 3);

    std::cout << "Server running on port 8080\n";

    while (true) {
        client_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);

        std::string response =
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html\r\n\r\n"
            "<html><body><h1>Hello from Linux C++ Server!</h1></body></html>";

        send(client_socket, response.c_str(), response.size(), 0);
        close(client_socket);
    }

    close(server_fd);
    return 0;
}
