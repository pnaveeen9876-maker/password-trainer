#include <iostream>
#include <cstring>
#include <cstdlib>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

int main() {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        std::cerr << "Socket creation failed\n";
        return 1;
    }

    // Get PORT from Render
    int port = 8080;
    char* env_port = getenv("PORT");
    if (env_port != nullptr) {
        port = std::stoi(env_port);
    }

    sockaddr_in server{};
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_socket, (sockaddr*)&server, sizeof(server)) < 0) {
        std::cerr << "Bind failed\n";
        return 1;
    }

    listen(server_socket, 5);

    std::cout << "Server running on port " << port << "...\n";

    while (true) {
        int client_socket = accept(server_socket, nullptr, nullptr);
        if (client_socket < 0) continue;

        const char* response =
"HTTP/1.1 200 OK\r\n"
"Content-Type: text/html; charset=UTF-8\r\n\r\n"

"<!DOCTYPE html>"
"<html>"
"<head>"
"<meta name='viewport' content='width=device-width, initial-scale=1.0'>"
"<title>Password Strength Checker</title>"

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
"  width:350px;"
"  box-shadow: 0 8px 32px rgba(0,0,0,0.3);"
"}"

"input {"
"  width:100%;"
"  padding:10px;"
"  margin-top:15px;"
"  border:none;"
"  border-radius:10px;"
"  outline:none;"
"}"

"button {"
"  margin-top:15px;"
"  padding:10px 20px;"
"  border:none;"
"  border-radius:10px;"
"  background:white;"
"  color:#764ba2;"
"  font-weight:bold;"
"  cursor:pointer;"
"}"

".bar-container {"
"  margin-top:15px;"
"  width:100%;"
"  height:10px;"
"  background:rgba(255,255,255,0.3);"
"  border-radius:10px;"
"}"

".bar {"
"  height:10px;"
"  width:0%;"
"  border-radius:10px;"
"  transition:0.3s;"
"}"

"#result { margin-top:10px; font-weight:bold; }"
"#suggestions { font-size:14px; margin-top:5px; }"

"</style>"
"</head>"

"<body>"

"<div class='card'>"
"<h2>🔐 Password Strength Checker</h2>"
"<input type='password' id='password' placeholder='Enter password'>"
"<button onclick='checkStrength()'>Check Strength</button>"

"<div class='bar-container'>"
"<div class='bar' id='strengthBar'></div>"
"</div>"

"<p id='result'></p>"
"<p id='suggestions'></p>"
"</div>"

"<script>"
"function checkStrength() {"
"  let password = document.getElementById('password').value;"
"  let score = 0;"
"  let suggestions = [];"

"  if (password.length >= 8) score++;"
"  else suggestions.push('Use at least 8 characters');"

"  if (/[A-Z]/.test(password)) score++;"
"  else suggestions.push('Add uppercase letters');"

"  if (/[0-9]/.test(password)) score++;"
"  else suggestions.push('Add numbers');"

"  if (/[^A-Za-z0-9]/.test(password)) score++;"
"  else suggestions.push('Add special characters');"

"  let strengthText = '';"
"  let bar = document.getElementById('strengthBar');"

"  if (score <= 1) {"
"    strengthText = 'Weak';"
"    bar.style.width = '25%';"
"    bar.style.background = 'red';"
"  }"
"  else if (score == 2 || score == 3) {"
"    strengthText = 'Medium';"
"    bar.style.width = '60%';"
"    bar.style.background = 'orange';"
"  }"
"  else {"
"    strengthText = 'Strong';"
"    bar.style.width = '100%';"
"    bar.style.background = 'limegreen';"
"  }"

"  document.getElementById('result').innerHTML = "
"    'Strength: ' + strengthText + ' (' + score + '/4)';"

"  document.getElementById('suggestions').innerHTML = "
"    suggestions.length > 0 ? "
"    'Suggestions: ' + suggestions.join(', ') : "
"    'Excellent password!';"
"}"
"</script>"

"</body>"
"</html>";

        send(client_socket, response, strlen(response), 0);
        close(client_socket);
    }

    close(server_socket);
    return 0;
}
