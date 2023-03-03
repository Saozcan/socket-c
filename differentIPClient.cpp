#include <iostream>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
using namespace std;

const char* IP_ADDRESS = "127.0.0.1";
const int PORT = 8080;

int main() {
    int sock = 0, valread;
    sockaddr_in serv_addr;
    char buffer[1024] = {0};
    const char* hello = "Hello from client";

    // Create a socket file descriptor
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        cout << "Socket creation error" << endl;
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, IP_ADDRESS, &serv_addr.sin_addr) <= 0) {
        cout << "Invalid address/ Address not supported" << endl;
        return -1;
    }

    // Connect to the server
    if (connect(sock, (sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        cout << "Connection failed" << endl;
        return -1;
    }

    // Send messages to the server
    while (true) {
        cout << "Enter message: ";
        string message;
        getline(cin, message);
        const char* msg = message.c_str();
        send(sock, msg, strlen(msg), 0);
        cout << "Message sent" << endl;

        valread = read(sock, buffer, 1024);
        buffer[valread] = '\0';
        cout << "Server: " << buffer << endl;
    }

    return 0;
}
