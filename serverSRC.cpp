//SERVER

//includes
#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h>
#include <iostream>
#include <string>


//initializes socket array to store connections and counter to keep track of connection amount
SOCKET Connections[100];
int ConnectionCounter = 0;

//initializes array of string username list
std::string usernameList[100];


//receives messages from client
void ClientHandlerThread(int index) {
	char buffer[256];
	std::string username = usernameList[index];
	while (true) {
		recv(Connections[index], buffer, sizeof(buffer), NULL);
		for (int i = 1; i < ConnectionCounter + 1; i++) {
			if (i == index) {
				continue;
			}
			send(Connections[i], (char*)&username, sizeof(username), NULL);
			send(Connections[i], buffer, sizeof(buffer), NULL);
		}
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
	addr.sin_addr.s_addr = inet_addr("127.0.0.1"); //current IP is set to localhost for easy testing, can set to desired IP here
	addr.sin_port = htons(1111);
	addr.sin_family = AF_INET;


	//sets socket to listen for incoming connections
	SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL);
	bind(sListen, (SOCKADDR*)&addr, sizeof(addr));
	listen(sListen, SOMAXCONN);


	//creates new connections
	SOCKET newConnection;

	//supports 100 connections
	for (int i = 1; i < 101; i++) {
		newConnection = accept(sListen, (SOCKADDR*)&addr, &sizeofaddr);
		if (newConnection == 0) {
			std::cout << "Failed to accept the client's connection" << std::endl;
		}
		else {
			//sets newConnection to corresponding spot in Connections array, increments ConnectionCounter
			std::cout << "Client connected" << std::endl;
			Connections[i] = newConnection;
			ConnectionCounter++;

			//receives username, couts username alongside connection number
			recv(Connections[i], (char*)&usernameList[i], sizeof(usernameList[i]), NULL);
			std::cout << "Connection " << i << " has selected the username: " << usernameList[i] << std::endl;

			//creates new thread for client connection
			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandlerThread, (LPVOID)(i), NULL, NULL);
		}
	}


	//unnecessary but useful for debugging
	system("pause");

	return 0;
}