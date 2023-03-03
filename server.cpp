//
// Created by saozcan on 28.02.2023.
//

#include <iostream>
#include <string.h>
#include "fcntl.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <poll.h>

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

    const int enable = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
        std::cout << ("setsockopt(SO_REUSEADDR) failed") << std::endl;


    // Bind the socket to the specified address and port
    if (bind(server_fd, (sockaddr*)&address, sizeof(address)) < 0) {
        cout << "Bind failed" << endl;
        return -1;
    }

    // Listen for incoming connections
    if (listen(server_fd, 3) < 0) { // en fazla 3 istek alabilirim
        cout << "Listen failed" << endl;
        return -1;
    }
    unsigned int index = 0;
    struct pollfd pollfd[1024];

    fcntl(server_fd, F_SETFL, O_NONBLOCK); // dinleme yaptığım server nonblock oldu

    // Accept incoming connections and echo messages back to the client
    while (true) {

        while (true) {

            if ((new_socket = accept(server_fd, (sockaddr*)&address, (socklen_t*)&addrlen)) < 0) {
                break;
            }

            pollfd[index].fd = new_socket;
            pollfd[index].events = POLLIN; // ekstra için | kullanılabilir.
            index++;

        }

        if (index > 0) {
            int pollReturn = poll(pollfd, index, 1000);
            if (pollReturn == 0) {
                continue;
            } // -1 hiç bekleme süresi koymaz

            std::cout << index << std::endl;

            for (int i = 0; i < index; i++) {
                int requestCount = 0;
                if (pollfd[i].revents & POLLIN) {
                    int valread;
                    while ((valread = recv(pollfd[i].fd, buffer, 1024, MSG_DONTWAIT)) > 0) { //MSG_DONT... non blcok için
                        buffer[valread] = '\0';
                        cout << "Client: " << buffer << endl;
                        send(pollfd[i].fd, buffer, strlen(buffer), 0);
                        memset(buffer, 0, sizeof(buffer));
                    }
                    requestCount++;
                    if (valread == 0 ) {
                        pollfd[i].fd = -1;
                        pollfd[i].events = 0; // If there is no messages it will clean it
                    }
//                    pollfd[i].fd = -1; // -1 ignore yani temizlemiş olur.
                }
                if (requestCount == pollReturn)
                    break ;
            }




//
//        int valread;
//        while ((valread = recv(new_socket, buffer, 1, 0)) > 0) {
//            buffer[valread] = '\0';
//            cout << "Client: " << buffer << endl;
//            send(new_socket, buffer, strlen(buffer), 0);
//            memset(buffer, 0, sizeof(buffer));
//        }
        }
    }

    return 0;
}
