#pragma once

class PacketBuffer;

class Session
{
public:
    Session();
    ~Session();

    bool InitSession(SOCKET listen_sock);
    void CloseSession();

    bool SendPacket(PacketBuffer packet);

    SOCKET client_sock;
    SOCKADDR_IN clientAddr;
};
