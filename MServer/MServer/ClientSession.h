#pragma once
#include "PacketHandlerManager.h"
#include "PacketBuffer.h"

template<typename TSession>
class SessionBase
{
public:
    typedef PacketHandlerManager<TSession> TPacketHandlerManager;
    static TPacketHandlerManager _s_packet_handler_manager;

public:
    explicit SessionBase() {};
    virtual ~SessionBase() {};

    bool InitSession(SOCKET listen_sock)
    {
        int32 addrlen = sizeof(clientAddr);

        client_sock = accept(listen_sock, (SOCKADDR*)&clientAddr, &addrlen);
        if (client_sock == INVALID_SOCKET)
        {
            LOG_ERROR("accept()");
            return false;
        }

        LOG_INFO("client_sock success.");

        return true;
    }

    void CloseSession()
    {
        closesocket(client_sock);
    }

    SOCKET GetClientSocket() const
    {
        return client_sock;
    }

    template<typename T>
    bool SendPacket(const T& packet)
    {
        if (!sendBuffer.SetPacket(packet))
        {
            LOG_ERROR("Fail to SetPacket().");
            return false;
        }

        int32 retval = send(client_sock,
            sendBuffer.GetPacketBuffer(), sendBuffer.GetPacketSize(), 0);
        if (SOCKET_ERROR == retval)
            return false;

        sendBuffer.ConsumePacket();
        return true;
    }

    virtual void OnRecv(const char* pBuffer, uint32 recvSize)
    {
        if (!pBuffer)
        {
            LOG_ERROR("OnRecv() pBuffer is nullptr.");
            return;
        }

        if (recvSize > MAX_BUF_SIZE)
        {
            LOG_ERROR("OnRecv() recvSize is bigger than MAX_BUF_SIZE. recvSize: {}",
                recvSize);
            return;
        }

        if (!recvBuffer.AppendBuffer(pBuffer, recvSize))
        {
            LOG_ERROR("OnRecv() AppendBuffer error.");
            return;
        }

        for (;;)
        {
            if (!recvBuffer.IsAbleToGetPacket())
                return;

            if (!_s_packet_handler_manager.Handle(static_cast<TSession&>(*this), recvBuffer))
            {
                LOG_ERROR("OnRecv() HandlerPacket failed. packetNumber: {}",
                    recvBuffer.GetPacketNo());
                return;
            }
        }
    }

protected:
    SOCKET client_sock;
    SOCKADDR_IN clientAddr;

    PacketBuffer recvBuffer;
    PacketBuffer sendBuffer;
};

class ClientSession : public SessionBase<ClientSession>
{
public:
    explicit ClientSession() {};
    virtual ~ClientSession() {};
};