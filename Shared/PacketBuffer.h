#pragma once

class PacketBuffer
{
public:
    PacketBuffer();
    ~PacketBuffer();

public:
    bool AppendBuffer(const char* pBuffer, uint16 size);
    void Clear();

public:
    // | Packet Header                         | Payload(packet data) |
    // | PacketSize (2byte) | PacketNo (2byte) |                      |
    uint16 GetHeaderSize() const { return sizeof(uint16) + sizeof(uint16); }
    uint16 GetPayloadSize() const { return GetPacketSize() - GetHeaderSize(); }
    uint16 GetRemainSize() const { return buffer.size() - writePos; }
    uint16 GetPacketNo() const;

    bool IsAbleToGetPacket() const;
    void ConsumePacket();

    const char* GetBuffer() const;
    uint16 GetBufferSize() const;

    const char* GetPacketBuffer() const;
    uint16 GetPacketSize() const;

    const char* GetPayloadBuffer() const;
    uint16 GetPayloadBufferSize() const;

    template<typename TPacket>
    bool SetPacket(const TPacket& packet)
    {
        const uint16 packetSize = GetHeaderSize() + packet.ByteSize();
        if (packetSize > GetRemainSize())
        {
            ReArrange();
        }

        if (packetSize > GetRemainSize())
        {
            LOG_ERROR("Not enough remain buffer.");
            return false;
        }

        if (!SetPacketSize(packetSize))
            return false;

        if (!SetPacketNo(TPacket::PROTOCOL_NUMBER))
            return false;

        std::array<char, MAX_BUF_SIZE> buffer = {0};
        if (!packet.SerializeToArray(buffer.data(), buffer.size()))
            return false;

        if (!AppendBuffer(buffer.data(), packet.ByteSize()))
            return false;

        return true;
    }

private:
    void ReArrange();
    void TruncateBuffer(uint16 size);

    bool SetPacketSize(uint16 packetSize);
    bool SetPacketNo(uint16 packetNo);

private:
    std::array<char, MAX_BUF_SIZE> buffer;
    uint16 readPos;
    uint16 writePos;
};
