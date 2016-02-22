#pragma once

#include "util.h"
#include "ServerSession.h"
#include "NetworkEngine.h"

int _tmain(int argc, _TCHAR* argv[])
{
	if (NetworkEngine::Inst()->InitWSA() == false)
		return -1;

	ServerSession serverSession;
	if (serverSession.InitSession() == false)
		return -1;

	SOCKET client_sock;
	SOCKADDR_IN clientAddr;
	char buf[BUFSIZE + 1] = { 0, };

	while (1)
	{
		int addrlen = sizeof(clientAddr);

		client_sock = accept(serverSession.listen_sock, (SOCKADDR*)&clientAddr, &addrlen);
		if (client_sock == INVALID_SOCKET)
		{
			printf("[ERROR] accept()");
			continue;
		}

		printf("client_sock success.\n");


		// 데이터 통신
		while (1)
		{
			// recv
			int retval = recv(client_sock, buf, BUFSIZE, 0);
			if (SOCKET_ERROR == retval || 0 == retval)
				break;

			// 버퍼 읽기
			int packetLength = 0;
			CopyMemory(&packetLength, buf, sizeof(short));
			
			// 이 이상 큰 패킷은 읽지 않는다
			if (packetLength > MAX_BUF_SIZE || packetLength <= 0)
				continue;

			msg packet;
			int len = sizeof(short);
			CopyMemory(&packet.no, buf + len, sizeof(int));
			len += sizeof(int);

			char message[BUFSIZE + 1] = { 0, };
			CopyMemory(&message, buf + len, packetLength - len);
			packet.message = message;

			// ++
			packet.addPacket();


			// send
			short bufSize = sizeof(short) + packet.size();
			CopyMemory(buf, &bufSize, sizeof(short));
			
			int lenWrite = sizeof(short);

			CopyMemory(buf + lenWrite, &packet.no, sizeof(int));
			lenWrite += sizeof(int);

			
			sprintf_s(message, packet.message.c_str(), packet.message.length());
			CopyMemory(buf + lenWrite, &message, packet.message.length());
			lenWrite += packet.message.length();

			
			retval = send(client_sock, buf, lenWrite, 0);
			if (SOCKET_ERROR == retval)
				break;

		}

		closesocket(client_sock);
	}

	serverSession.CloseSession();

	NetworkEngine::Inst()->ReleaseWSA();

	return 0;
}