#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>  // for inet_pton()

#pragma comment(lib, "ws2_32.lib")

using namespace std;

int Send(int in[]) {
	// Initialize Winsock
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		cout << "WSAStartup failed: " << iResult << endl;
		return 1;
	}

	// Create a socket
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) {
		cout << "Failed to create socket: " << WSAGetLastError() << endl;
		WSACleanup();
		return 1;
	}

	// Specify the IP address and port of the ESP32 board
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(80);
	inet_pton(AF_INET, "192.168.4.1", &addr.sin_addr); //<-- Change IP of ESP32/Destination 

	// Connect to the ESP32 board
	if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) {
		cout << "Failed to connect to ESP32 board: " << WSAGetLastError() << endl;
		closesocket(sock);
		WSACleanup();
		return 1;
	}

	// Send data to the ESP32 board
	char raw[12]; // 3 integers, each 4 bytes
	for (int i = 0; i < 3; i++) {
		*((int*)(raw + i * 4)) = in[i];
	}
	if (send(sock, raw, 12, 0) == SOCKET_ERROR) {
		cout << "Failed to send data to ESP32 board: " << WSAGetLastError() << endl;
		closesocket(sock);
		WSACleanup();
		return 1;
	}

	// Close the socket
	closesocket(sock);
	WSACleanup();

	return 0;
}