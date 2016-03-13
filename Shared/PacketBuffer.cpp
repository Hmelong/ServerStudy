#include "util.h"
#include "PacketBuffer.h"

PacketBuffer::PacketBuffer()
{
}

PacketBuffer::~PacketBuffer()
{
}

void PacketBuffer::WriteBuffer(char* pBuffer)
{
    if (!pBuffer)
    {
        LOG_ERROR("msg pBuffer is nullptr.");
        return;
    }

    uint16 packetLength = GetPacketSize();
    if (packetLength > MAX_BUF_SIZE)
    {
        LOG_ERROR("packetLength({}) is bigger than MAX_BUF_SIZE({})",
            packetLength, MAX_BUF_SIZE);
        return;
    }

    CopyMemory(pBuffer, &packetLength, sizeof(packetLength));

    CopyMemory(pBuffer + sizeof(packetLength), &no, sizeof(no));

    CopyMemory(pBuffer + sizeof(packetLength) + sizeof(no), message.data(), message.size());
}

bool PacketBuffer::ParseBuffer(const char* pBuffer)
{
    if (!pBuffer)
        return false;

    uint16 packetLength = 0;
    CopyMemory(&packetLength, pBuffer, sizeof(uint16));

    if (packetLength == 0 || packetLength > MAX_BUF_SIZE)
        return false;

    CopyMemory(&no, pBuffer + sizeof(packetLength), sizeof(no));

    const char* pStartMessage = pBuffer + sizeof(packetLength) + sizeof(no);
    const char* pEndMessage = pBuffer + packetLength;
    message.assign(pStartMessage, pEndMessage);

    return true;
}

void PacketBuffer::CopyFrom(const PacketBuffer& rhs)
{
    if (this == &rhs)
        return;

    len = rhs.len;
    no = rhs.no;
    message = rhs.message;
}
