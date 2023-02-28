# IRC

```c++
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

```

Clien için yukarıdaki simple fonksiyon kullanılabilir. 

8080 portu dinlenecektiri localhost kullanılarak. 

### sockaddr_in

`sockaddr_in` is a structure defined in the C and C++ socket programming libraries that is used to represent an Internet Protocol (IP) socket address. The structure is defined in the `<netinet/in.h>` header file.

The `sockaddr_in` structure contains the following members:

- `sin_family`: Specifies the address family (should be set to `AF_INET` for IP).
- `sin_port`: Specifies the port number to use for the socket, in network byte order.
- `sin_addr`: Specifies the IP address of the socket, in network byte order.
- `sin_zero`: Padding to make the structure the same size as `sockaddr`.

```c++
c
#include <netinet/in.h>

const char* IP_ADDRESS = "127.0.0.1";
const int PORT = 8080;

// Create a socket address structure
sockaddr_in server_address;
server_address.sin_family = AF_INET;
server_address.sin_port = htons(PORT);
inet_pton(AF_INET, IP_ADDRESS, &server_address.sin_addr);
```

In this example, the `inet_pton()` function is used to convert the IP address from a string format to the network byte order used by the `sin_addr` member of the `sockaddr_in` structure. The `htons()` function is used to convert the port number to network byte order. The resulting `sockaddr_in` structure can be passed to the socket functions to specify the address and port of the socket.



### Functions

1. `socket()`: This function creates a new socket and returns a file descriptor that can be used to refer to the socket in subsequent function calls. The function takes three arguments: the domain (e.g. `AF_INET` for IPv4), the type (e.g. `SOCK_STREAM` for TCP), and the protocol (usually set to 0, which will select the default protocol for the given domain and type).
2. `bind()`: This function associates a socket with a specific address and port. The function takes the socket file descriptor, a pointer to a `sockaddr` structure that specifies the address and port to bind to, and the size of the `sockaddr` structure.
3. `listen()`: This function places a socket in a listening state, allowing it to accept incoming connections. The function takes the socket file descriptor and the maximum number of pending connections that can be queued up for the socket.
4. `accept()`: This function blocks until a client connects to the server and returns a new socket file descriptor for the connection. The function takes the socket file descriptor for the listening socket and a pointer to a `sockaddr` structure that will be filled in with the address and port of the client.
5. `connect()`: This function initiates a connection to a remote server. The function takes the socket file descriptor, a pointer to a `sockaddr` structure that specifies the remote server address and port, and the size of the `sockaddr` structure.
6. `send()` and `recv()`: These functions are used to send and receive data over a connected socket. The functions take the socket file descriptor, a buffer to send or receive data, the length of the buffer, and any additional flags.
7. `sendto()` and `recvfrom()`: These functions are used to send and receive data over a datagram socket (e.g. UDP). The functions take the socket file descriptor, a buffer to send or receive data, the length of the buffer, any additional flags, and a pointer to a `sockaddr` structure that specifies the destination or source address.
8. `close()`: This function closes a socket and releases any resources associated with it. The function takes the socket file descriptor.

### inet_pton

`inet_pton` is a function in the C and C++ socket programming libraries that is used to convert an IPv4 or IPv6 address from a string format to a binary format that can be used by the `sockaddr_in` or `sockaddr_in6` structures respectively. The function is defined in the `<arpa/inet.h>` header file.

The function takes two arguments: the address family (either `AF_INET` for IPv4 or `AF_INET6` for IPv6) and a pointer to a string that contains the IP address to convert. The function returns 1 if the conversion was successful, 0 if the address string was invalid, or -1 if an error occurred.

Here's an example of how to use `inet_pton` to convert an IPv4 address string to binary format:

```c++
c
#include <arpa/inet.h>

const char* IP_ADDRESS = "192.168.1.1";
struct sockaddr_in server_address;

// Convert the IP address string to binary format
if (inet_pton(AF_INET, IP_ADDRESS, &server_address.sin_addr) <= 0) {
    perror("inet_pton failed");
    exit(EXIT_FAILURE);
}
```

In this example, `inet_pton` is used to convert the string "192.168.1.1" to binary format and store it in the `sin_addr` member of the `sockaddr_in` structure. If the conversion fails, the function will return a value less than or equal to zero and an error message will be printed to the console using the `perror` function.



### What should you know 

Here are some additional things you may want to know about socket programming:

1. Blocking vs Non-Blocking Sockets: Sockets can be set to either blocking or non-blocking mode. In blocking mode, socket functions will block until the operation is complete (e.g. `send()` will block until all the data has been sent). In non-blocking mode, socket functions will return immediately with an error if the operation would block.
2. Select and Poll Functions: To handle multiple sockets at once, you can use the `select()` or `poll()` functions. These functions allow you to wait for activity on one or more sockets and handle it when it occurs.
3. Sockets and Threading: Sockets can be used in a multi-threaded program to handle multiple clients concurrently. However, you need to be careful to properly synchronize access to shared data and avoid race conditions.
4. Network Byte Order: When sending data over the network, you need to be aware of byte order. Most network protocols use big-endian byte order (also known as network byte order), which means that the most significant byte comes first. You can use the `htonl()`, `htons()`, `ntohl()`, and `ntohs()` functions to convert between host byte order and network byte order.
5. Socket Options: Socket options allow you to control the behavior of sockets. For example, you can set the `SO_REUSEADDR` option to allow a server to bind to a port that is still in use by a previous instance of the server.
6. Error Handling: Socket functions can fail for many reasons, such as network errors or invalid arguments. It's important to properly handle errors and gracefully handle failures.
7. Security: When writing socket programs, it's important to be aware of security issues such as buffer overflows, denial of service attacks, and man-in-the-middle attacks. You should use secure coding practices and follow best practices for network security.

These are just a few of the things you may want to know about socket programming. As with any programming topic, there is always more to learn!



