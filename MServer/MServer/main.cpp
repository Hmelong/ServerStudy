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
    std::array<char, MAX_BUF_SIZE + 1> buf = {0};

    while (1)
    {
        int32 addrlen = sizeof(clientAddr);

        client_sock = accept(serverSession.listen_sock, (SOCKADDR*)&clientAddr, &addrlen);
        if (client_sock == INVALID_SOCKET)
        {
            LOG_ERROR("accept()");
            continue;
        }

        LOG_INFO("client_sock success.");

        // 데이터 통신
        while (1)
        {
            // recv
            int32 retval = recv(client_sock, buf.data(), buf.size(), 0);
            if (SOCKET_ERROR == retval || 0 == retval)
                break;

            msg packet;
            if (!packet.ParseBuffer(buf.data()))
                continue;

            // ++
            packet.no++;
            packet.message += std::to_string(packet.no);

            // send
            packet.WriteBuffer(buf.data());
            
            retval = send(client_sock, buf.data(), packet.GetPacketSize(), 0);
            if (SOCKET_ERROR == retval)
                break;
        }

        closesocket(client_sock);
    }

    serverSession.CloseSession();

    NetworkEngine::Inst()->ReleaseWSA();

    return 0;
}