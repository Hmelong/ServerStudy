#include "util.h"
#include "Singleton.h"

#include "Session.h"
#include "PacketBuffer.h"

#include "PacketHandler.h"

PacketHandler::PacketHandler()
{
}

PacketHandler::~PacketHandler()
{
}

void PacketHandler::InitPacketFunctor()
{
    packetFunctor[NO_0] = new PACKET_NO_0;
    packetFunctor[NO_1] = new PACKET_NO_1;
}

bool PacketHandler::HandlePacket(Session* pSession, const PacketBuffer& packet)
{
    if (pSession == nullptr)
        return false;

    Session& session = *pSession;

    if (packet.no < 0 || packet.no + 1 > MAX_NO)
        return false;

    packetFunctor[packet.no];

    return true;
}

bool PACKET_NO_0::operator()(Session& session, const PacketBuffer& packet)
{
    PacketBuffer out;
    out.no = packet.no + 1;
    out.message = fmt::format("{}{}", packet.message, packet.no);

    session.SendPacket(out);

    return true;
}

bool PACKET_NO_1::operator()(Session& session, const PacketBuffer& packet)
{
    return true;
}
