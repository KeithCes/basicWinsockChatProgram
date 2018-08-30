//Client

//includes
#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h>
#include <iostream>
#include <string>


//initializes socket called connection
SOCKET Connection;

//initializes string username
std::string username;


//receives messages from other clients
void ClientThread() {
	char buffer[256];
	std::string username;
	while (true) {
		recv(Connection, (char*)&username, sizeof(username), NULL);
		recv(Connection, buffer, sizeof(buffer), NULL);
		std::cout << username << ": " << buffer << std::endl;
	}
}


//main function
int main() {
	//checks to make sure Winsock starts up without errors 
	WSAData wsaData;
	WORD DllVersion = MAKEWORD(2, 1);
	if (WSAStartup(DllVersion, &wsaData) != 0) {
		MessageBoxA(NULL, "Winsock startup failed", "Error", MB_OK | MB_ICONERROR);
		exit(1);
	}


	//sets up socket with address, port number, and IP config
	SOCKADDR_IN addr;
	int sizeofaddr = sizeof(addr);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(1111);
	addr.sin_family = AF_INET;


	//sets socket to try and connect, if cannot connect displays error and closes
	Connection = socket(AF_INET, SOCK_STREAM, NULL);
	if (connect(Connection, (SOCKADDR*)&addr, sizeofaddr) != 0) {
		MessageBoxA(NULL, "Failed to connect", "Error", MB_OK);
		return 0;
	}
	else {
		//if connected displays affirmative message and prompts user for username input
		std::cout << "CONNECTED!" << std::endl;
		std::cout << "Please enter a username: ";
		std::cin >> username;

		//sends username over to server and creates a thread for client
		send(Connection, (char*)&username, sizeof(username), NULL);
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientThread, NULL, NULL, NULL);

		//creates buffer, checks for input from client on loop
		char buffer[256];
		while (true) {
			std::cin.getline(buffer, sizeof(buffer));
			send(Connection, buffer, sizeof(buffer), NULL);
			Sleep(10);
		}

	}


	//unnecessary but useful for debugging
	system("pause");

	return 0;
}