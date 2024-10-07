#ifndef WIFI_SEND_H
#define WIFI_SEND_H

#include <iostream>
#include <stdexcept>
#include <array>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

class WifiSender {
private:
    static constexpr const char* ESP32_IP = "192.168.4.1";
    static constexpr int ESP32_PORT = 80;
    static constexpr int DATA_SIZE = 3;
    static constexpr int BYTES_PER_INT = sizeof(int);

    WSADATA wsaData;
    SOCKET sock;
    sockaddr_in addr;

    void initializeWinsock() {
        int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (result != 0) {
            throw std::runtime_error("WSAStartup failed: " + std::to_string(result));
        }
    }

    void createSocket() {
        sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock == INVALID_SOCKET) {
            throw std::runtime_error("Failed to create socket: " + std::to_string(WSAGetLastError()));
        }
    }

    void setupAddress() {
        addr.sin_family = AF_INET;
        addr.sin_port = htons(ESP32_PORT);
        if (inet_pton(AF_INET, ESP32_IP, &addr.sin_addr) <= 0) {
            throw std::runtime_error("Invalid address / Address not supported");
        }
    }

    void connectToESP32() {
        if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) {
            throw std::runtime_error("Failed to connect to ESP32 board: " + std::to_string(WSAGetLastError()));
        }
    }

public:
    WifiSender() : sock(INVALID_SOCKET) {
        try {
            initializeWinsock();
            createSocket();
            setupAddress();
        }
        catch (...) {
            cleanup();
            throw;
        }
    }

    ~WifiSender() {
        cleanup();
    }

    void send(const std::array<int, DATA_SIZE>& data) {
        try {
            connectToESP32();

            std::array<char, DATA_SIZE * BYTES_PER_INT> raw;
            for (int i = 0; i < DATA_SIZE; ++i) {
                *reinterpret_cast<int*>(&raw[i * BYTES_PER_INT]) = data[i];
            }

            if (::send(sock, raw.data(), raw.size(), 0) == SOCKET_ERROR) {
                throw std::runtime_error("Failed to send data to ESP32 board: " + std::to_string(WSAGetLastError()));
            }

            closesocket(sock);
            createSocket();  // Prepare for the next connection
        }
        catch (...) {
            closesocket(sock);
            createSocket();  // Prepare for the next connection
            throw;
        }
    }

private:
    void cleanup() {
        if (sock != INVALID_SOCKET) {
            closesocket(sock);
        }
        WSACleanup();
    }
};

#endif // WIFI_SEND_H
