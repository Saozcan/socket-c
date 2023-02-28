//
// Created by saozcan on 28.02.2023.
//

#include <iostream>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
using namespace std;

const int PORT = 8080;

int main() {
    int sock = 0;
    sockaddr_in serv_addr;
    char buffer[1024] = {0};

    // Creating a socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        cout << "Socket creation error" << endl;
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        cout << "Invalid address/ Address not supported" << endl;
        return -1;
    }

    // Connect to the server
    if (connect(sock, (sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        cout << "Connection failed" << endl;
        return -1;
    }

    // Loop to send and receive messages
    while (true) {
        memset(buffer, 0, sizeof(buffer));
        cout << "Enter message: ";
        cin.getline(buffer, sizeof(buffer));

        // Send message to the server
        send(sock, buffer, strlen(buffer), 0);

        // Receive message from the server
        int valread = recv(sock, buffer, 1024, 0);
        buffer[valread] = '\0';
        cout << "Server: " << buffer << endl;
    }

    return 0;
}
