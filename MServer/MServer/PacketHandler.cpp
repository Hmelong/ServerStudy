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


bool PacketHandler::HandlePacket(Session* pSession, const PacketBuffer& packet)
{
    if (pSession == nullptr)
        return false;

    Session& session = *pSession;

    switch (packet.no)
    {
    case NO_0:
        return PACKET_NO_0(session, packet);
        break;
    case NO_1:
        return PACKET_NO_1(session, packet);
        break;
    default:
        assert(false);
        break;
    }

    return true;
}

bool PacketHandler::PACKET_NO_0(Session& session, const PacketBuffer& packet)
{
    PacketBuffer out;
    out.no = packet.no + 1;
    out.message = fmt::format("{}{}", packet.message, packet.no);

    session.SendPacket(out);

    return true;
}

bool PacketHandler::PACKET_NO_1(Session& session, const PacketBuffer& packet)
{
    return true;
}
