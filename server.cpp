#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

int main() {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        std::cerr << "Socket creation failed\n";
        return 1;
    }

    sockaddr_in server{};
    server.sin_family = AF_INET;
    server.sin_port = htons(8080);
    server.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_socket, (sockaddr*)&server, sizeof(server)) < 0) {
        std::cerr << "Bind failed\n";
        return 1;
    }

    listen(server_socket, 5);

    std::cout << "Server running on port 8080...\n";

    while (true) {
        int client_socket = accept(server_socket, nullptr, nullptr);
        if (client_socket < 0) continue;

        const char* response =
"HTTP/1.1 200 OK\r\n"
"Content-Type: text/html\r\n\r\n"
"<!DOCTYPE html>"
"<html>"
"<head>"
"<meta name='viewport' content='width=device-width, initial-scale=1.0'>"
"<title>Password Trainer</title>"
"<style>"
"body {"
"  margin:0;"
"  font-family: 'Segoe UI', sans-serif;"
"  background: linear-gradient(135deg, #667eea, #764ba2);"
"  display:flex;"
"  justify-content:center;"
"  align-items:center;"
"  height:100vh;"
"}"
".card {"
"  background: rgba(255,255,255,0.1);"
"  backdrop-filter: blur(15px);"
"  padding:40px;"
"  border-radius:20px;"
"  text-align:center;"
"  color:white;"
"  box-shadow: 0 8px 32px rgba(0,0,0,0.3);"
"}"
"h1 { font-size:32px; margin-bottom:10px; }"
"p { opacity:0.8; }"
"button {"
"  margin-top:20px;"
"  padding:10px 20px;"
"  border:none;"
"  border-radius:10px;"
"  background:white;"
"  color:#764ba2;"
"  font-weight:bold;"
"  cursor:pointer;"
"  transition:0.3s;"
"}"
"button:hover {"
"  transform: scale(1.1);"
"}"
"</style>"
"</head>"
"<body>"
"<div class='card'>"
"<h1>🔐 Password Trainer</h1>"
"<p>Sharpen your password security skills</p>"
"<button onclick='alert(\"Feature Coming Soon 🚀\")'>Start Training</button>"
"</div>"
"</body>"
"</html>";


        send(client_socket, response, strlen(response), 0);
        close(client_socket);
    }

    close(server_socket);
    return 0;
}

