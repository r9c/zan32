#include "server.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>

#pragma comment(lib, "ws2_32.lib")

void startServer(const std::string& totpCode) {
    WSADATA wsaData;
    SOCKET serverSocket, clientSocket;
    sockaddr_in serverAddr{}, clientAddr{};
    int clientSize = sizeof(clientAddr);

    WSAStartup(MAKEWORD(2, 2), &wsaData);
    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(4444);

    bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));
    listen(serverSocket, 1);

    std::cout << "\n[Waiting for connection...]\n";

    clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientSize);

    // Receive peer's TOTP code
    char buffer[7] = {};
    recv(clientSocket, buffer, 6, 0);

    if (std::string(buffer) == totpCode) {
        std::cout << "[Connected] TOTP matched.\n";

        // Chat loop
        char msg[1024];
        while (true) {
            int bytes = recv(clientSocket, msg, sizeof(msg) - 1, 0);
            if (bytes <= 0) break;
            msg[bytes] = '\0';
            std::cout << "Friend: " << msg << "\n";

            std::cout << "You: ";
            std::string response;
            std::getline(std::cin, response);
            send(clientSocket, response.c_str(), response.size(), 0);
        }
    } else {
        std::cout << "[Rejected] Wrong TOTP.\n";
    }

    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();
}
