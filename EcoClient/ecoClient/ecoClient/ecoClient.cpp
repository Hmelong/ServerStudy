// ecoClient.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "util.h"

int _tmain(int argc, _TCHAR* argv[])
{
    int32 retval = 0;

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

    // ������ ���
    std::array<char, MAX_BUF_SIZE + 1> buf = {0};

    while (1)
    {
        // ������ �Է�
        buf.fill('\0');
        LOG_INFO("[���� ������]");
        
        if (NULL == fgets(buf.data(), MAX_BUF_SIZE, stdin))
        {
            continue;
        }

        // enter ����
        int32 len = static_cast<int32>(strlen(buf.data()));
        buf[len - 1] = '\0';

        msg packet;
        packet.no = 0;
        packet.message = buf.data();

        // ������ ������
        buf.fill('\0');

        packet.WriteBuffer(buf.data());

        retval = send(sock, buf.data(), packet.GetPacketSize(), 0);
        if (SOCKET_ERROR == retval)
        {
            LOG_ERROR("send() error: {}", retval);
            continue;
        }

        LOG_INFO("[TCP Client] {} ����Ʈ�� ���½��ϴ�.", retval);

        while (1)
        {
            // ������ �ޱ�
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

            LOG_INFO("[TCP Client] {} ����Ʈ�� �޾ҽ��ϴ�.", retval);
            LOG_INFO("[���� ������] No.{}-{}",  packet.no, packet.message);

            // ������ ������
            retval = send(sock, buf.data(), packet.GetPacketSize(), 0);
            if (SOCKET_ERROR == retval)
            {
                LOG_ERROR("send() error: {}", retval);
                break;
            }

            LOG_INFO("[TCP Client] {} ����Ʈ�� ���½��ϴ�.", retval);
        }
    }

    // closesocket();
    closesocket(sock);

    // ���� ����
    WSACleanup();

    return 0;
}

