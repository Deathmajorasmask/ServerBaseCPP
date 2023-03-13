#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib, "ws2_32.lib")

#include <iostream>
#include <WinSock2.h>
#include <string>

int main()
{
    std::cout << "Attemping to create a server!\n";
    SOCKET wsocket;
    SOCKET new_wsocket;
    WSADATA wsaData;
    struct sockaddr_in server;
    int server_len;
    int BUFFER_SIZE = 37020;
    // initialize WSAStartup function(winsock.h)
    if (WSAStartup(MAKEWORD(2,2), &wsaData)!= 0) {
        std::cout << "Could not initialize wsaData \n";
    }
    
    // Create socket
    wsocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // int, type, protocol
    if (wsocket == INVALID_SOCKET) {
        std::cout << "Could not create server \n";
    }
    
    // Bind socket to adress
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1"); // localhost
    server.sin_port = htons(8080); // port
    server_len = sizeof(server);

    if (bind(wsocket, (SOCKADDR *)&server, server_len) != 0) {
        std::cout << "Could not bind socket \n";
    }

    //listen to address
    if (listen(wsocket, 20) !=0) {
        std::cout << "Could not start listening \n";
    }

    std::cout << "Listening on 127.0.0.1:8080 \n";

    int bytes = 0;
    while (true) {
        // accept client request
        new_wsocket = accept(wsocket, (SOCKADDR *)&server, &server_len); 
        if (new_wsocket == INVALID_SOCKET) {
            std::cout << "Coult not accept \n";
        }

        // Read request
        char buff[30720] = { 0 };
        bytes = recv(new_wsocket, buff, BUFFER_SIZE, 0);
        if (bytes < 0) {
            std::cout << "Could not read client request";
        }

        std::string serverMessage = "HTTP/1.1 200 OK\nContent-type: text/html\nContent_Length: ";
        std::string response = "<html><h1>Hello World !</h1></html>";
        serverMessage.append(std::to_string(response.size()));
        serverMessage.append("\n\n");
        serverMessage.append(response);

        int bytesSent = 0;
        int totalBytesSent = 0;
        while (totalBytesSent < (serverMessage.size())) {
            bytesSent = send(new_wsocket, serverMessage.c_str(), serverMessage.size(),0);
            if (bytesSent < 0 ) {
                std::cout << "Could not send response \n";
            }
            totalBytesSent += bytesSent;
        }
        std::cout << "Send response to client \n";
        closesocket(new_wsocket);
    }
    closesocket(new_wsocket);
    WSACleanup();

    return 0;
}
