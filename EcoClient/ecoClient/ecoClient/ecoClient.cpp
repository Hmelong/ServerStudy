// ecoClient.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"

#include "util.h"

int _tmain(int argc, _TCHAR* argv[])
{
	int retval;

	// ���� �ʱ�ȭ
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

	// ������ ���
	char buf[BUFSIZE + 1] = { 0, };

	while (1)
	{
		// ������ �Է�
		ZeroMemory(buf, sizeof(buf));
		printf("\n[���� ������] ");
		
		if (NULL == fgets(buf, BUFSIZE, stdin))
		{
			continue;
		}

		// enter ����
		int len = strlen(buf);
		buf[len - 1] = '\0';

		msg packet;
		packet.no = 0;
		packet.message = buf;

		// ������ ������
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
		printf("[TCP Client] %d ����Ʈ�� ���½��ϴ�.\n", retval);

		while (1)
		{
			// ������ �ޱ�
			ZeroMemory(buf, sizeof(buf));
			retval = recv(sock, buf, BUFSIZE, 0);
			if (SOCKET_ERROR == retval || retval == 0)
			{
				printf("[ERROR] resv() \n");
				break;
			}

			// ���� ������ ���
			int packetLength = 0;
			CopyMemory(&packetLength, buf, sizeof(short));

			msg packet;
			int lenRead = sizeof(short);
			CopyMemory(&packet.no, buf + lenRead, sizeof(int));
			lenRead += sizeof(int);

			char message[BUFSIZE + 1] = { 0, };
			CopyMemory(&message, buf + lenRead, packetLength - lenRead);
			packet.message = message;

			printf("[TCP Client] %d ����Ʈ�� �޾ҽ��ϴ�.\n", retval);
			printf("[���� ������] No.%d-%s\n", packet.no, packet.message.c_str());

			// ������ ������
			retval = send(sock, buf, strlen(buf), 0);
			if (SOCKET_ERROR == retval)
			{
				printf("[ERROR] send() \n");
				break;
			}
			printf("[TCP Client] %d ����Ʈ�� ���½��ϴ�.\n", retval);
		}
	}

	// closesocket();
	closesocket(sock);

	// ���� ����
	WSACleanup();

	return 0;
}

