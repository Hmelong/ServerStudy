#pragma once

class PacketBUffer;

class PacketHandler : public Singleton<PacketHandler>
{
public:
    PacketHandler();
    ~PacketHandler();

    bool HandlePacket(Session* pSession, const PacketBuffer& packet);

    static bool PACKET_NO_0(Session& session, const PacketBuffer& packet);
    static bool PACKET_NO_1(Session& session, const PacketBuffer& packet);
};
