#include "client.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>

#pragma comment(lib, "ws2_32.lib")

void connectToHost(const std::string& ip, const std::string& totpCode) {
    WSADATA wsaData;
    SOCKET sock;
    sockaddr_in serverAddr{};

    WSAStartup(MAKEWORD(2, 2), &wsaData);
    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(4444);
    inet_pton(AF_INET, ip.c_str(), &serverAddr.sin_addr);

    if (connect(sock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "[!] Failed to connect.\n";
        WSACleanup();
        return;
    }

    // Send TOTP
    send(sock, totpCode.c_str(), 6, 0);

    std::cout << "[Connected]\n";

    // Chat loop
    char msg[1024];
    while (true) {
        std::cout << "You: ";
        std::string input;
        std::getline(std::cin, input);
        send(sock, input.c_str(), input.size(), 0);

        int bytes = recv(sock, msg, sizeof(msg) - 1, 0);
        if (bytes <= 0) break;
        msg[bytes] = '\0';
        std::cout << "Friend: " << msg << "\n";
    }

    closesocket(sock);
    WSACleanup();
}
