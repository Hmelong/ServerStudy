// ecoClient.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "util.h"

int _tmain(int argc, _TCHAR* argv[])
{
    int32 retval = 0;

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
    std::array<char, MAX_BUF_SIZE + 1> buf = {0};

    while (1)
    {
        // 데이터 입력
        buf.fill('\0');
        LOG_INFO("[보낼 데이터]");
        
        if (NULL == fgets(buf.data(), MAX_BUF_SIZE, stdin))
        {
            continue;
        }

        // enter 제거
        int32 len = static_cast<int32>(strlen(buf.data()));
        buf[len - 1] = '\0';

        msg packet;
        packet.no = 0;
        packet.message = buf.data();

        // 데이터 보내기
        buf.fill('\0');

        packet.WriteBuffer(buf.data());

        retval = send(sock, buf.data(), packet.GetPacketSize(), 0);
        if (SOCKET_ERROR == retval)
        {
            LOG_ERROR("send() error: {}", retval);
            continue;
        }

        LOG_INFO("[TCP Client] {} 바이트를 보냈습니다.", retval);

        while (1)
        {
            // 데이터 받기
            buf.fill('\0');

            retval = recv(sock, buf.data(), buf.size(), 0);
            if (SOCKET_ERROR == retval || retval == 0)
            {
                LOG_ERROR("recv error: {}", retval);
                break;
            }

            msg packet;
            if (!packet.ParseBuffer(buf.data()))
            {
                LOG_ERROR("msg parse failed.");
                break;
            }

            LOG_INFO("[TCP Client] {} 바이트를 받았습니다.", retval);
            LOG_INFO("[받은 데이터] No.{}-{}",  packet.no, packet.message);

            // 데이터 보내기
            retval = send(sock, buf.data(), packet.GetPacketSize(), 0);
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

