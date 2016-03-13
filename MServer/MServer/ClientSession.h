#pragma once
#include "PacketHandlerManager.h"

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

    bool SendPacket(const PacketBuffer& packet)
    {
        PacketBuffer out(packet);

        std::array<char, MAX_BUF_SIZE + 1> buf = {0};

        // send
        out.WriteBuffer(buf.data());

        int32 retval = send(client_sock, buf.data(), out.GetPacketSize(), 0);
        if (SOCKET_ERROR == retval)
            return false;

        return true;
    }

    virtual void OnRecv(const PacketBuffer& packet)
    {
        if (!_s_packet_handler_manager.Handle(static_cast<TSession&>(*this), packet))
        {
            LOG_ERROR("OnRecv() HandlerPacket failed. packetNumber: {}", packet.no);
        }
    }

protected:
    SOCKET client_sock;
    SOCKADDR_IN clientAddr;
};

class ClientSession : public SessionBase<ClientSession>
{
public:
    explicit ClientSession() {};
    virtual ~ClientSession() {};
};