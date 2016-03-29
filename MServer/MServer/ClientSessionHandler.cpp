#include "util.h"

#include "ClientSession.h"
#include "p_client_server.h"
#include "p_server_client.h"

DECLARE_HANDLER(ClientSession, PC2S_Chat);

IMPLEMENT_INITIALIZE(ClientSession)
{
    REGISTER_HANDLER(PC2S_Chat);
}

IMPLEMENT_HANDLER(ClientSession, PC2S_Chat)
{
    static uint64 count = 0;

    PS2C_Chat out;
    out.set_message(fmt::format("{},{}", packet.message(), ++count));

    session.SendPacket(out);
    return true;
}
