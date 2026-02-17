#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <cstdlib>

int main() {
    int server_fd, client_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        std::cerr << "Socket failed\n";
        return 1;
    }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;

    int port = std::stoi(getenv("PORT"));  // Required for Render
    address.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        std::cerr << "Bind failed\n";
        return 1;
    }

    listen(server_fd, 10);

    std::cout << "Server running on port " << port << "\n";

    while (true) {
        client_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);

        std::string html =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n\r\n"
        "<!DOCTYPE html>"
        "<html>"
        "<head>"
        "<meta name='viewport' content='width=device-width, initial-scale=1.0'>"
        "<title>Password Strength Checker</title>"

        "<link rel='stylesheet' href='https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.5.0/css/all.min.css'>"

        "<style>"
        "body{font-family:Arial;background:linear-gradient(135deg,#667eea,#764ba2);"
        "height:100vh;display:flex;justify-content:center;align-items:center;margin:0;}"

        ".container{background:white;padding:30px;border-radius:10px;"
        "width:350px;text-align:center;box-shadow:0 10px 25px rgba(0,0,0,0.2);}"

        ".password-container{position:relative;width:100%;}"

        ".password-container input{width:100%;padding:10px 40px 10px 10px;font-size:16px;}"

        ".password-container i{position:absolute;right:10px;top:50%;"
        "transform:translateY(-50%);cursor:pointer;color:gray;}"

        ".strength{margin-top:15px;font-weight:bold;}"

        ".progress-bar{height:8px;background:#ddd;margin-top:10px;"
        "border-radius:5px;overflow:hidden;}"

        ".progress{height:100%;width:0%;transition:0.3s;}"
        "</style>"
        "</head>"

        "<body>"
        "<div class='container'>"
        "<h2>Password Strength Checker</h2>"

        "<div class='password-container'>"
        "<input type='password' id='password' placeholder='Enter password'>"
        "<i class='fa-solid fa-eye' id='togglePassword'></i>"
        "</div>"

        "<div class='progress-bar'><div class='progress' id='progress'></div></div>"
        "<div class='strength' id='strengthText'></div>"
        "</div>"

        "<script>"
        "const password=document.getElementById('password');"
        "const strengthText=document.getElementById('strengthText');"
        "const progress=document.getElementById('progress');"
        "const togglePassword=document.getElementById('togglePassword');"

        "togglePassword.addEventListener('click',function(){"
        "const type=password.getAttribute('type')==='password'?'text':'password';"
        "password.setAttribute('type',type);"
        "this.classList.toggle('fa-eye');"
        "this.classList.toggle('fa-eye-slash');});"

        "password.addEventListener('input',function(){"
        "let val=password.value;let strength=0;"
        "if(val.length>=8)strength++;"
        "if(/[A-Z]/.test(val))strength++;"
        "if(/[0-9]/.test(val))strength++;"
        "if(/[^A-Za-z0-9]/.test(val))strength++;"

        "if(strength<=1){strengthText.innerHTML='Weak';progress.style.width='33%';progress.style.background='red';}"
        "else if(strength==2||strength==3){strengthText.innerHTML='Medium';progress.style.width='66%';progress.style.background='orange';}"
        "else{strengthText.innerHTML='Strong';progress.style.width='100%';progress.style.background='green';}"
        "});"
        "</script>"

        "</body></html>";

        send(client_socket, html.c_str(), html.size(), 0);
        close(client_socket);
    }

    return 0;
}
