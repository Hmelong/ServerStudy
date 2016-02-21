// ecoClient.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"

#include "util.h"

int _tmain(int argc, _TCHAR* argv[])
{
	int retval;

	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return -1;
	}

	// socket()
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (INVALID_SOCKET == sock)
	{
		printf("[ERROR] socket() \n");
	}

	// connect()
	SOCKADDR_IN serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(9000);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	retval = connect(sock, (SOCKADDR*)&serverAddr, sizeof(serverAddr));
	if (SOCKET_ERROR == retval)
	{
		printf("[ERROR] connect() \n");
	}

	// 데이터 통신
	char buf[BUFSIZE + 1] = { 0, };

	while (1)
	{
		// 데이터 입력
		ZeroMemory(buf, sizeof(buf));
		printf("\n[보낼 데이터] ");
		
		if (NULL == fgets(buf, BUFSIZE, stdin))
		{
			continue;
		}

		// enter 제거
		int len = strlen(buf);
		buf[len - 1] = '\0';

		msg packet;
		packet.no = 0;
		packet.message = buf;

		// 데이터 보내기
		ZeroMemory(buf, sizeof(buf));

		short bufSize = sizeof(short) + packet.size();
		CopyMemory(buf, &bufSize, sizeof(short));

		int lenWrite = sizeof(short);

		CopyMemory(buf + lenWrite, &packet.no, sizeof(int));
		lenWrite += sizeof(int);

		char message[BUFSIZE + 1] = { 0, };
		sprintf_s(message, packet.message.c_str(), packet.message.length());
		CopyMemory(buf + lenWrite, &message, packet.message.length());
		lenWrite += packet.message.length();


		retval = send(sock, buf, lenWrite, 0);
		if (SOCKET_ERROR == retval)
		{
			printf("[ERROR] send() \n");
			continue;
		}
		printf("[TCP Client] %d 바이트를 보냈습니다.\n", retval);

		while (1)
		{
			// 데이터 받기
			ZeroMemory(buf, sizeof(buf));
			retval = recv(sock, buf, BUFSIZE, 0);
			if (SOCKET_ERROR == retval || retval == 0)
			{
				printf("[ERROR] resv() \n");
				break;
			}

			// 받은 데이터 출력
			int packetLength = 0;
			CopyMemory(&packetLength, buf, sizeof(short));

			msg packet;
			int lenRead = sizeof(short);
			CopyMemory(&packet.no, buf + lenRead, sizeof(int));
			lenRead += sizeof(int);

			char message[BUFSIZE + 1] = { 0, };
			CopyMemory(&message, buf + lenRead, packetLength - lenRead);
			packet.message = message;

			printf("[TCP Client] %d 바이트를 받았습니다.\n", retval);
			printf("[받은 데이터] No.%d-%s\n", packet.no, packet.message.c_str());

			// 데이터 보내기
			retval = send(sock, buf, strlen(buf), 0);
			if (SOCKET_ERROR == retval)
			{
				printf("[ERROR] send() \n");
				break;
			}
			printf("[TCP Client] %d 바이트를 보냈습니다.\n", retval);
		}
	}

	// closesocket();
	closesocket(sock);

	// 윈속 종료
	WSACleanup();

	return 0;
}

