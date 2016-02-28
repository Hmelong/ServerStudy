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
        LOG_ERROR("socket()");
    }

    // connect()
    SOCKADDR_IN serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(9000);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    retval = connect(sock, (SOCKADDR*)&serverAddr, sizeof(serverAddr));
    if (SOCKET_ERROR == retval)
    {
        LOG_ERROR("connect error: {}", retval);
    }

    // 데이터 통신
    char buf[MAX_BUF_SIZE + 1] = { 0, };

    while (1)
    {
        // 데이터 입력
        ZeroMemory(buf, sizeof(buf));
        LOG_INFO("[보낼 데이터]");
        
        if (NULL == fgets(buf, MAX_BUF_SIZE, stdin))
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

        char message[MAX_BUF_SIZE + 1] = { 0, };
        sprintf_s(message, packet.message.c_str(), packet.message.length());
        CopyMemory(buf + lenWrite, &message, packet.message.length());
        lenWrite += packet.message.length();


        retval = send(sock, buf, lenWrite, 0);
        if (SOCKET_ERROR == retval)
        {
            LOG_ERROR("send() error: {}", retval);
            continue;
        }

        LOG_INFO("[TCP Client] {} 바이트를 보냈습니다.", retval);

        while (1)
        {
            // 데이터 받기
            ZeroMemory(buf, sizeof(buf));
            retval = recv(sock, buf, MAX_BUF_SIZE, 0);
            if (SOCKET_ERROR == retval || retval == 0)
            {
                LOG_ERROR("recv error: {}", retval);
                break;
            }

            // 받은 데이터 출력
            int packetLength = 0;
            CopyMemory(&packetLength, buf, sizeof(short));

            msg packet;
            int lenRead = sizeof(short);
            CopyMemory(&packet.no, buf + lenRead, sizeof(int));
            lenRead += sizeof(int);

            char message[MAX_BUF_SIZE + 1] = { 0, };
            CopyMemory(&message, buf + lenRead, packetLength - lenRead);
            packet.message = message;

            LOG_INFO("[TCP Client] {} 바이트를 보냈습니다.", retval);
            LOG_INFO("[받은 데이터] No.{}-{}",  packet.no, packet.message);

            // 데이터 보내기
            retval = send(sock, buf, strlen(buf), 0);
            if (SOCKET_ERROR == retval)
            {
                LOG_ERROR("send() error: {}", retval);
                break;
            }

            LOG_INFO("[TCP Client] {} 바이트를 보냈습니다.", retval);
        }
    }

    // closesocket();
    closesocket(sock);

    // 윈속 종료
    WSACleanup();

    return 0;
}

