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
    int server_fd, new_socket;
    sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};

    // Creating a socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        cout << "Socket creation error" << endl;
        return -1;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind the socket to the specified address and port
    if (bind(server_fd, (sockaddr*)&address, sizeof(address)) < 0) {
        cout << "Bind failed" << endl;
        return -1;
    }

    // Listen for incoming connections
    if (listen(server_fd, 3) < 0) {
        cout << "Listen failed" << endl;
        return -1;
    }

    // Accept incoming connections and echo messages back to the client
    while (true) {
        if ((new_socket = accept(server_fd, (sockaddr*)&address, (socklen_t*)&addrlen)) < 0) {
            cout << "Accept failed" << endl;
            return -1;
        }

        int valread;
        while ((valread = recv(new_socket, buffer, 1024, 0)) > 0) {
            buffer[valread] = '\0';
            cout << "Client: " << buffer << endl;
            send(new_socket, buffer, strlen(buffer), 0);
            memset(buffer, 0, sizeof(buffer));
        }
    }

    return 0;
}
