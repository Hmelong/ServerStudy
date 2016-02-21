#pragma once

#include "util.h"
#include "NetworkSession.h"


NetworkSession::NetworkSession()
{
}

NetworkSession::~NetworkSession()
{
}

bool NetworkSession::InitSession()
{
	// WSA
	if (InitWSA() == false)
		return false;

	// socket
	if (InitSocket() == false)
		return false;

	// bind
	if (Bind() == false)
		return false;

	// listen
	if (Listen() == false)
		return false;

	return true;
}

void NetworkSession::CloseSession()
{
	// cloasesocket();
	closesocket(listen_sock);

	// 윈속 종료
	WSACleanup();
}

bool NetworkSession::InitWSA()
{
	if (WSAStartup(MAKEWORD(2, 2), &WSA) != 0)
	{
		printf("[ERROR] Initalize WSASock");
		return false;
	}
	printf("WSASock success.\n");
	
	return true;
}

bool NetworkSession::InitSocket()
{
	listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET)
	{
		printf("[ERROR] Initalize listen_sock");
		return false;
	}

	printf("socket success.\n");

	return true;
}

bool NetworkSession::Bind()
{
	SOCKADDR_IN serverAddr;
	ZeroMemory(&serverAddr, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(9000);
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	int retval = bind(listen_sock, (SOCKADDR*)&serverAddr, sizeof(serverAddr));
	if (retval == SOCKET_ERROR)
	{
		printf("[ERROR] bind()");
		return false;
	}

	printf("bind success.\n");

	return true;
}

bool NetworkSession::Listen()
{
	int retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR)
	{
		printf("[ERROR] listen()");
		return false;
	}

	printf("listen success.\n");

	return true;
}