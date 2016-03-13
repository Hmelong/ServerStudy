#pragma once

#include "util.h"
#include "ServerSession.h"
#include "NetworkEngine.h"

#include "Session.h"

#include "PacketHandler.h"
#include "PacketBuffer.h"

int _tmain(int argc, _TCHAR* argv[])
{
    if (NetworkEngine::Inst()->InitWSA() == false)
        return -1;

    ServerSession serverSession;
    if (serverSession.InitSession() == false)
        return -1;

    PacketHandler::Inst()->InitPacketFunctor();


    Session* clientSession = new Session();

    std::array<char, MAX_BUF_SIZE + 1> buf = {0};

    while (1)
    {
        if (clientSession == nullptr)
            continue;

        if (clientSession->InitSession(serverSession.GetSocket()) == false)
            continue;

        LOG_INFO("client_sock success.");

        // 데이터 통신
        while (1)
        {
            // recv
            int32 retval = recv(clientSession->client_sock, buf.data(), buf.size(), 0);
            if (SOCKET_ERROR == retval || 0 == retval)
                break;

            PacketBuffer packet;
            if (!packet.ParseBuffer(buf.data()))
                continue;

            // handle packet
            PacketHandler::Inst()->HandlePacket(clientSession, packet);

        }

        clientSession->CloseSession();
    }

    serverSession.CloseSession();

    NetworkEngine::Inst()->ReleaseWSA();

    return 0;
}