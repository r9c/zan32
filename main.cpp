#include <iostream>
#include <thread>
#include <chrono>
#include "totp.h"
#include "server.h"
#include "client.h"
#include "encryption.h"

int main() {
    std::cout << "[1] Host a chat session\n";
    std::cout << "[2] Join a chat session\n";
    std::cout << "Select mode: ";
    int choice;
    std::cin >> choice;
    std::cin.ignore(); // make sure std::getline works after std::cin

    if (choice == 1) {
        // HOST
        std::string secret = generateBase32Secret();
        std::cout << "\n≡ Your TOTP Secret: " << secret << "\n\n";

        // Start TOTP code refresh in a separate thread
        std::thread codeThread([&]() {
            while (true) {
                std::string code = getTOTPCode(secret);
                int seconds = secondsUntilNextCode();
                std::cout << "TOTP Code: " << code << " | Refreshes in: " << seconds << "s\r";
                std::cout.flush();
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
        });

        // Start server using current TOTP code
        startServer(getTOTPCode(secret));

        codeThread.join(); // Wait for the thread to finish (if ever)
    }
    else if (choice == 2) {
        // JOIN
        std::string ip, code;
        std::cout << "Enter friend's IP: ";
        std::cin >> ip;
        std::cout << "Enter 6-digit TOTP code: ";
        std::cin >> code;
        std::cin.ignore();

        connectToHost(ip, code);
    }
    else {
        std::cerr << "Invalid choice.\n";
        return 1;
    }

    return 0;
}
