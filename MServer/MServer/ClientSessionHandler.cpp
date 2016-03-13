#include "util.h"

#include "ClientSession.h"
#include "PacketBuffer.h"

DECLARE_HANDLER(ClientSession, PACKET_NO_0);
DECLARE_HANDLER(ClientSession, PACKET_NO_1);

IMPLEMENT_INITIALIZE(ClientSession)
{
    REGISTER_HANDLER(PACKET_NO_0, NO_0);
    REGISTER_HANDLER(PACKET_NO_1, NO_1);
}

IMPLEMENT_HANDLER(ClientSession, PACKET_NO_0)
{
    PacketBuffer out;
    out.no = packet.no + 1;
    out.message = fmt::format("{}{}", packet.message, packet.no);

    session.SendPacket(out);

    return true;
}

IMPLEMENT_HANDLER(ClientSession, PACKET_NO_1)
{
    return true;
}
