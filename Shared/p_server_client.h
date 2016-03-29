#pragma once

#include "packet_defines.h"
#include "server_client.pb.h"

namespace PS2C
{
    enum ProtocolNumber : uint16
    {
        Chat,
    };
}

DECLARE_PACKET_TYPE(PS2C, Chat);
