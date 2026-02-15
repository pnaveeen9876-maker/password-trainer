#include <iostream>
#include <string>
#include <winsock2.h>

#pragma comment(lib,"ws2_32.lib")

int main() {
    WSADATA wsa;
    SOCKET server_fd, client_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    // Start Winsock
    WSAStartup(MAKEWORD(2,2), &wsa);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    bind(server_fd, (struct sockaddr*)&address, sizeof(address));
    listen(server_fd, 3);

    std::cout << "Server running at http://localhost:8080\n";

    while (true) {
        client_socket = accept(server_fd, (struct sockaddr*)&address, &addrlen);

        std::string html =
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html\r\n\r\n"
            "<!DOCTYPE html>"
            "<html>"
            "<head>"
            "<title>CPP HTML UI</title>"
            "<script>"
            "function showMessage(){"
            "document.getElementById('result').innerHTML='Hello from C++ Server!';"
            "}"
            "</script>"
            "</head>"
            "<body style='font-family:Arial;text-align:center;margin-top:50px;'>"
            "<h1>Simple C++ HTML UI</h1>"
            "<button onclick='showMessage()' "
            "style='padding:10px 20px;font-size:18px;'>"
            "Show Message"
            "</button>"
            "<p id='result' style='margin-top:20px;font-size:20px;color:blue;'></p>"
            "</body>"
            "</html>";

        send(client_socket, html.c_str(), html.size(), 0);
        closesocket(client_socket);
    }

    WSACleanup();
    return 0;
}
