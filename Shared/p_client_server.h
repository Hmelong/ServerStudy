#pragma once

#include "packet_defines.h"
#include "client_server.pb.h"

namespace PC2S
{
    enum ProtocolNumber : uint16
    {
        Chat,
    };
}

DECLARE_PACKET_TYPE(PC2S, Chat);
