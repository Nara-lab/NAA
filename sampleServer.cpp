#include "stdafx.h"
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#pragma comment(lib, "Ws2_32.lib")

#define IP_ADDRESS "JVPLCPU02"
#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT 2020

char *data_buffer;
int data_size;
int main()
{
WSADATA wsaData;
	int nret;
	bool return_value = true;
	int socket_return_value;
	int socket_send_return_value;
	char recvbuf[DEFAULT_BUFLEN];
	int recvbuflen = DEFAULT_BUFLEN;
  struct sockaddr_in socket_structure; //TCP/IP socket structure
	SOCKET	theClient;
	SOCKET listeningSocket;

	memset(&socket_structure, 0, sizeof(struct sockaddr_in));
	socket_structure.sin_family = AF_INET;
	socket_structure.sin_addr.s_addr = INADDR_ANY;
	socket_structure.sin_port = htons(DEFAULT_PORT);
	nret = WSAStartup(0x101, &wsaData);
	if (nret != 0) {
		printf("WSAStartup failed with error: %d\n", nret);
		return 1;
	}listeningSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == listeningSocket)
	{
		printf("\n Create_Socket:::Error: %u", WSAGetLastError());
		WSACleanup();
		return false;
	}

	nret = bind(listeningSocket, (LPSOCKADDR)&socket_structure, sizeof(socket_structure));
	if (SOCKET_ERROR == nret)
	{
		printf("\n Create_Socket::Binding Error: %u", WSAGetLastError());
		WSACleanup();
		return false;
	}
  nret = listen(listeningSocket, 3);
	if (SOCKET_ERROR == nret)
	{
		printf("\n Listening Error: %d", WSAGetLastError());
		WSACleanup();
		return false;
	}
	printf("\nsocket created\n");

	char value = 1;
	setsockopt(listeningSocket, IPPROTO_TCP, TCP_NODELAY, &value, sizeof(value));

	int socket_struct_size = sizeof(socket_structure);
	theClient = accept(listeningSocket, (struct sockaddr*)&socket_structure, &socket_struct_size);
	if (INVALID_SOCKET == theClient)
	{
  printf("\n Accept_Socket Error: %u", WSAGetLastError());
		WSACleanup();
		return false;
	}
		do
		{
			socket_return_value = recv(theClient,recvbuf,recvbuflen,0);
			if (socket_return_value > 0) {
				socket_send_return_value = send(theClient, recvbuf, socket_return_value, 0);
				if (socket_send_return_value == SOCKET_ERROR) {
					printf("send failed with error: %d\n", WSAGetLastError());
					closesocket(theClient);
					WSACleanup();
					return 1;
				}
        printf("%s",(const char*)recvbuf);
			}
			else if (socket_return_value == 0)
				printf("Connection closing...\n");
			else {
				printf("recv failed with error: %d\n", WSAGetLastError());
				closesocket(theClient);
				WSACleanup();
				return 1;
			}
	}while (socket_return_value > 0);
  socket_return_value = shutdown(theClient, SD_SEND);
	if (socket_return_value == SOCKET_ERROR) {
		printf("shutdown failed with error: %d\n", WSAGetLastError());
		closesocket(theClient);
		WSACleanup();
		return 1;
	}
	closesocket(theClient);
	WSACleanup();

return 0;
}
