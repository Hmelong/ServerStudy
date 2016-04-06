#include "util.h"
#include "PacketBuffer.h"

PacketBuffer::PacketBuffer()
    : readPos(0)
    , writePos(0)
{
    buffer.assign(0);
}

PacketBuffer::~PacketBuffer()
{
}

bool PacketBuffer::AppendBuffer(const char* pBuffer, uint16 size)
{
    if (!pBuffer)
        return false;

    if (size == 0)
        return false;

    if (size > GetRemainSize())
    {
        ReArrange();
    }

    if (size > GetRemainSize())
    {
        LOG_ERROR("Not enough remain buffer.");
        return false;
    }

    CopyMemory(buffer.data() + writePos, pBuffer, size);
    writePos += size;

    return true;
}

void PacketBuffer::Clear()
{
    readPos = 0;
    writePos = 0;
    buffer.assign(0);
}

uint16 PacketBuffer::GetPacketNo() const
{
    if (writePos < GetHeaderSize())
    {
        LOG_ERROR("Not enough data.");
        return 0;
    }

    uint16 packetNo = 0;
    CopyMemory(&packetNo, buffer.data() + readPos + sizeof(uint16), sizeof(uint16));

    return packetNo;
}

bool PacketBuffer::IsAbleToGetPacket() const
{
    const uint16 packetSize = GetPacketSize();
    if (packetSize == 0)
        return false;

    const uint16 packetBufferSize = GetBufferSize();
    if (packetBufferSize == 0)
        return false;

    return (packetSize <= packetBufferSize);
}

void PacketBuffer::ConsumePacket()
{
    const uint16 packetSize = GetPacketSize();
    TruncateBuffer(packetSize);
}

const char* PacketBuffer::GetBuffer() const
{
    return buffer.data() + readPos;
}

uint16 PacketBuffer::GetBufferSize() const
{
    if (readPos > writePos)
    {
        LOG_ERROR("Not enough buffer size.");
        return 0;
    }

    return writePos - readPos;
}

const char* PacketBuffer::GetPacketBuffer() const
{
    return GetBuffer();
}

uint16 PacketBuffer::GetPacketSize() const
{
    const uint16 bufferSize = GetBufferSize();
    if (bufferSize < sizeof(uint16))
    {
        return 0;
    }

    uint16 packetSize = 0;
    CopyMemory(&packetSize, buffer.data() + readPos, sizeof(packetSize));

    return packetSize;
}

const char* PacketBuffer::GetPayloadBuffer() const
{
    return GetBuffer() + GetHeaderSize();
}

uint16 PacketBuffer::GetPayloadBufferSize() const
{
    return GetPacketSize() - GetHeaderSize();
}

void PacketBuffer::ReArrange()
{
    const uint16 bufferSize = GetBufferSize();
    MoveMemory(buffer.data(), GetBuffer(), bufferSize);
    readPos = 0;
    writePos = bufferSize;
}

void PacketBuffer::TruncateBuffer(uint16 size)
{
    if (size > GetBufferSize())
    {
        LOG_ERROR("Fail to truncate buffer. remain size: {}, truncate size: {}",
            writePos - readPos, size);
        return;
    }

    readPos += size;
}

bool PacketBuffer::SetPacketSize(uint16 packetSize)
{
    if (packetSize > MAX_BUF_SIZE)
    {
        LOG_ERROR("packet size is bigger than MAX_BUF_SIZE. packetSize: {}",
            packetSize);
        return false;
    }

    CopyMemory(buffer.data() + writePos, &packetSize, sizeof(packetSize));
    writePos += sizeof(packetSize);

    return true;
}

bool PacketBuffer::SetPacketNo(uint16 packetNo)
{
    CopyMemory(buffer.data() + writePos, &packetNo, sizeof(packetNo));
    writePos += sizeof(packetNo);

    return true;
}
