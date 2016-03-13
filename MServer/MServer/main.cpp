#pragma once

#include "util.h"
#include "ServerSession.h"
#include "NetworkEngine.h"

#include "ClientSession.h"

#include "PacketHandlerManager.h"
#include "PacketBuffer.h"

int _tmain(int argc, _TCHAR* argv[])
{
    if (NetworkEngine::Inst()->InitWSA() == false)
        return -1;

    ServerSession serverSession;
    if (serverSession.InitSession() == false)
        return -1;

    std::unique_ptr<ClientSession> upClientSession = std::make_unique<ClientSession>();

    std::array<char, MAX_BUF_SIZE + 1> buf = {0};

    while (1)
    {
        if (!upClientSession)
            continue;

        if (upClientSession->InitSession(serverSession.GetSocket()) == false)
            continue;

        LOG_INFO("client_sock success.");

        // 데이터 통신
        while (1)
        {
            // recv
            int32 retval = recv(upClientSession->GetClientSocket(), buf.data(), buf.size(), 0);
            if (SOCKET_ERROR == retval || 0 == retval)
                break;

            PacketBuffer packet;
            if (!packet.ParseBuffer(buf.data()))
                continue;

            upClientSession->OnRecv(packet);
        }

        upClientSession->CloseSession();
    }
    
    serverSession.CloseSession();

    NetworkEngine::Inst()->ReleaseWSA();

    return 0;
}