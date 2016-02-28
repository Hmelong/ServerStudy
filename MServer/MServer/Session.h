#pragma once

class PacketBuffer;

class Session
{
public:
    Session();
    ~Session();

    bool SendPacket(PacketBuffer packet);

private:
    SOCKET client_sock;
    SOCKADDR_IN clientAddr;
};
