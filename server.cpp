#include <iostream>
#include <string>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <cstdlib>

int main() {
    int server_fd, client_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;

    int port = std::stoi(getenv("PORT"));
    address.sin_port = htons(port);

    bind(server_fd, (struct sockaddr*)&address, sizeof(address));
    listen(server_fd, 10);

    while (true) {
        client_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);

        std::string html =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n\r\n"

        "<!DOCTYPE html>"
        "<html><head>"
        "<meta name='viewport' content='width=device-width, initial-scale=1.0'>"
        "<title>Password Strength Checker</title>"

        "<link rel='stylesheet' href='https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.5.0/css/all.min.css'>"

        "<style>"
        "body{font-family:Arial;background:linear-gradient(135deg,#667eea,#764ba2);"
        "height:100vh;display:flex;justify-content:center;align-items:center;margin:0;}"

        ".container{background:white;padding:40px;border-radius:15px;width:400px;"
        "box-shadow:0 15px 30px rgba(0,0,0,0.2);text-align:center;}"

        "h2{margin-bottom:25px;}"

        ".password-container{position:relative;width:100%;}"

        ".password-container input{width:100%;padding:12px 45px 12px 12px;"
        "font-size:16px;border-radius:8px;border:1px solid #ccc;"
        "box-sizing:border-box;}"

        ".password-container i{position:absolute;right:15px;top:50%;"
        "transform:translateY(-50%);cursor:pointer;color:#666;}"

        "button{margin-top:20px;padding:10px 20px;font-size:16px;"
        "border:none;border-radius:8px;background:#667eea;color:white;"
        "cursor:pointer;transition:0.3s;}"

        "button:hover{background:#5a67d8;}"

        ".result{margin-top:20px;font-weight:bold;}"

        ".suggestions{margin-top:15px;text-align:left;font-size:14px;}"
        "</style>"
        "</head>"

        "<body>"
        "<div class='container'>"
        "<h2>Password Strength Checker</h2>"

        "<div class='password-container'>"
        "<input type='password' id='password' placeholder='Enter password'>"
        "<i class='fa-solid fa-eye' id='togglePassword'></i>"
        "</div>"

        "<button onclick='checkPassword()'>Check Strength</button>"

        "<div class='result' id='strengthText'></div>"
        "<div class='suggestions' id='suggestions'></div>"

        "</div>"

        "<script>"
        "const togglePassword=document.getElementById('togglePassword');"
        "const password=document.getElementById('password');"

        "togglePassword.addEventListener('click',function(){"
        "const type=password.type==='password'?'text':'password';"
        "password.type=type;"
        "this.classList.toggle('fa-eye');"
        "this.classList.toggle('fa-eye-slash');});"

        "function checkPassword(){"
        "let val=password.value;"
        "let score=0;"
        "let suggestions=[];"

        "if(val.length>=8){score++;}else{suggestions.push('Use at least 8 characters');}"
        "if(/[A-Z]/.test(val)){score++;}else{suggestions.push('Add uppercase letter');}"
        "if(/[0-9]/.test(val)){score++;}else{suggestions.push('Add number');}"
        "if(/[^A-Za-z0-9]/.test(val)){score++;}else{suggestions.push('Add special character');}"

        "let text='';"
        "if(score<=1){text='Weak Password';}"
        "else if(score<=3){text='Medium Password';}"
        "else{text='Strong Password';}"

        "document.getElementById('strengthText').innerHTML="
        "'Score: '+score+'/4 - '+text;"

        "if(suggestions.length>0){"
        "document.getElementById('suggestions').innerHTML="
        "'<b>Suggestions:</b><ul><li>'+suggestions.join('</li><li>')+'</li></ul>';"
        "}else{"
        "document.getElementById('suggestions').innerHTML="
        "'<b style=\"color:green;\">Excellent Password!</b>';"
        "}"
        "}"
        "</script>"

        "</body></html>";

        send(client_socket, html.c_str(), html.size(), 0);
        close(client_socket);
    }

    return 0;
}
