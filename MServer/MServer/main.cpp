#pragma once

#include "util.h"
#include "ServerSession.h"
#include "NetworkEngine.h"

#include "ClientSession.h"

#include "PacketHandlerManager.h"
#include "PacketBuffer.h"
#include "google\protobuf\message_lite.h"

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
            buf.assign(0);

            // recv
            int32 retval = recv(upClientSession->GetClientSocket(),
                buf.data(), static_cast<int32>(buf.size()), 0);
            if (SOCKET_ERROR == retval)
                break;

            upClientSession->OnRecv(buf.data(), retval);
        }

        upClientSession->CloseSession();
    }
    
    serverSession.CloseSession();

    NetworkEngine::Inst()->ReleaseWSA();

    return 0;
}